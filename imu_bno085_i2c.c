/*
 * imu_bno085_i2c.c
 *
 * Driver BNO085/BNO080 por I2C para STM32 HAL.
 *
 * Objetivo:
 * - detectar automaticamente 0x4A / 0x4B
 * - habilitar Rotation Vector (0x05) a 20 Hz
 * - recibir paquetes SHTP de forma tolerante a paquetes pendientes
 * - convertir el cuaternion Q14 a Roll, Pitch y Yaw en grados
 *
 * Esta version evita rechazar una lectura por un unico paquete vacio o
 * de control: revisa varios paquetes antes de declarar que no hay dato.
 */

#include "imu_bno085_i2c.h"

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

extern I2C_HandleTypeDef hi2c1;

/* ------------------------------------------------------------------------- */
/* Configuracion                                                             */
/* ------------------------------------------------------------------------- */

#define BNO085_ADDR_4A_7BIT              0x4AU
#define BNO085_ADDR_4B_7BIT              0x4BU
#define BNO085_ADDR_HAL(addr7)           ((uint16_t)((addr7) << 1U))

#define SHTP_CHANNEL_EXECUTABLE          1U
#define SHTP_CHANNEL_CONTROL             2U
#define SHTP_CHANNEL_REPORTS             3U

#define SHTP_REPORT_PRODUCT_ID_RESPONSE  0xF8U
#define SHTP_REPORT_PRODUCT_ID_REQUEST   0xF9U
#define SHTP_REPORT_SET_FEATURE_COMMAND  0xFDU

#define REPORT_ID_ROTATION_VECTOR        0x05U

/* 50 000 us = 50 ms = 20 Hz */
#define IMU_REPORT_INTERVAL_US           50000UL

#define IMU_I2C_TIMEOUT_MS               200U
#define IMU_PAYLOAD_BUFFER_SIZE          128U
#define IMU_I2C_DATA_CHUNK               28U
#define IMU_MAX_PACKET_LENGTH            1024U
#define IMU_MAX_PACKETS_PER_READ         12U

/* ------------------------------------------------------------------------- */
/* Estado interno                                                            */
/* ------------------------------------------------------------------------- */

static uint16_t bno085_addr_hal = 0U;
static uint8_t shtp_sequence[6] = {0U, 0U, 0U, 0U, 0U, 0U};
static uint8_t shtp_payload[IMU_PAYLOAD_BUFFER_SIZE];
static uint8_t imu_initialized = 0U;

/* ------------------------------------------------------------------------- */
/* Utilidades SHTP                                                           */
/* ------------------------------------------------------------------------- */

static uint8_t IMU_SendPacket(
        uint8_t channel,
        const uint8_t *payload,
        uint16_t payload_length)
{
    uint8_t tx_buffer[4U + 32U];
    uint16_t packet_length;

    if ((bno085_addr_hal == 0U) ||
        (channel >= 6U) ||
        (payload == NULL) ||
        (payload_length > 32U))
    {
        return 0U;
    }

    packet_length = payload_length + 4U;

    tx_buffer[0] = (uint8_t)(packet_length & 0xFFU);
    tx_buffer[1] = (uint8_t)((packet_length >> 8U) & 0x7FU);
    tx_buffer[2] = channel;
    tx_buffer[3] = shtp_sequence[channel]++;

    memcpy(&tx_buffer[4], payload, payload_length);

    return (HAL_I2C_Master_Transmit(
                &hi2c1,
                bno085_addr_hal,
                tx_buffer,
                packet_length,
                IMU_I2C_TIMEOUT_MS) == HAL_OK) ? 1U : 0U;
}

/*
 * Lee un paquete SHTP.
 *
 * En I2C el BNO08x repite los cuatro bytes de cabecera al iniciar cada
 * nueva transaccion. Primero se consulta la cabecera y luego se lee el
 * payload en bloques de hasta 28 bytes, descartando la cabecera repetida.
 */
static uint8_t IMU_ReceivePacket(
        uint8_t *channel,
        uint8_t *payload,
        uint16_t capacity,
        uint16_t *payload_length)
{
    uint8_t header[4];
    uint8_t block[4U + IMU_I2C_DATA_CHUNK];
    uint16_t packet_length;
    uint16_t total_payload;
    uint16_t remaining;
    uint16_t chunk;
    uint16_t stored = 0U;
    uint16_t copy_length;

    if ((bno085_addr_hal == 0U) ||
        (channel == NULL) ||
        (payload_length == NULL))
    {
        return 0U;
    }

    if (HAL_I2C_Master_Receive(
            &hi2c1,
            bno085_addr_hal,
            header,
            sizeof(header),
            IMU_I2C_TIMEOUT_MS) != HAL_OK)
    {
        return 0U;
    }

    packet_length =
        ((uint16_t)header[1] << 8U) |
        (uint16_t)header[0];

    /* Bit 15: paquete continuado. Para esta aplicacion se ignora. */
    packet_length &= 0x7FFFU;

    if ((packet_length < 4U) ||
        (packet_length > IMU_MAX_PACKET_LENGTH))
    {
        return 0U;
    }

    total_payload = packet_length - 4U;

    *channel = header[2];
    *payload_length = total_payload;

    if (total_payload == 0U)
    {
        return 1U;
    }

    remaining = total_payload;

    while (remaining > 0U)
    {
        chunk = remaining;
        if (chunk > IMU_I2C_DATA_CHUNK)
        {
            chunk = IMU_I2C_DATA_CHUNK;
        }

        if (HAL_I2C_Master_Receive(
                &hi2c1,
                bno085_addr_hal,
                block,
                chunk + 4U,
                IMU_I2C_TIMEOUT_MS) != HAL_OK)
        {
            return 0U;
        }

        if ((payload != NULL) && (stored < capacity))
        {
            copy_length = chunk;
            if ((stored + copy_length) > capacity)
            {
                copy_length = capacity - stored;
            }

            memcpy(&payload[stored], &block[4], copy_length);
            stored += copy_length;
        }

        remaining -= chunk;
    }

    return 1U;
}

static int16_t IMU_ReadInt16LE(const uint8_t *data)
{
    return (int16_t)(
        (uint16_t)data[0] |
        ((uint16_t)data[1] << 8U));
}

/* ------------------------------------------------------------------------- */
/* Inicializacion                                                            */
/* ------------------------------------------------------------------------- */

static uint8_t IMU_CheckProductID(void)
{
    const uint8_t request[2] =
    {
        SHTP_REPORT_PRODUCT_ID_REQUEST,
        0x00U
    };

    uint8_t channel;
    uint16_t payload_length;
    uint8_t attempt;

    if (IMU_SendPacket(
            SHTP_CHANNEL_CONTROL,
            request,
            sizeof(request)) == 0U)
    {
        return 0U;
    }

    for (attempt = 0U; attempt < 20U; attempt++)
    {
        if (IMU_ReceivePacket(
                &channel,
                shtp_payload,
                sizeof(shtp_payload),
                &payload_length) != 0U)
        {
            if ((channel == SHTP_CHANNEL_CONTROL) &&
                (payload_length >= 1U) &&
                (shtp_payload[0] == SHTP_REPORT_PRODUCT_ID_RESPONSE))
            {
                return 1U;
            }
        }

        HAL_Delay(5U);
    }

    return 0U;
}

static uint8_t IMU_EnableRotationVector(void)
{
    uint8_t payload[17];
    uint32_t interval_us = IMU_REPORT_INTERVAL_US;

    memset(payload, 0, sizeof(payload));

    payload[0] = SHTP_REPORT_SET_FEATURE_COMMAND;
    payload[1] = REPORT_ID_ROTATION_VECTOR;

    /* Report interval little-endian, en microsegundos. */
    payload[5] = (uint8_t)(interval_us & 0xFFU);
    payload[6] = (uint8_t)((interval_us >> 8U) & 0xFFU);
    payload[7] = (uint8_t)((interval_us >> 16U) & 0xFFU);
    payload[8] = (uint8_t)((interval_us >> 24U) & 0xFFU);

    return IMU_SendPacket(
        SHTP_CHANNEL_CONTROL,
        payload,
        sizeof(payload));
}

/* ------------------------------------------------------------------------- */
/* Cuaternion -> Euler                                                       */
/* ------------------------------------------------------------------------- */

static uint8_t IMU_QuaternionToEuler(
        float qi,
        float qj,
        float qk,
        float qr,
        float *roll,
        float *pitch,
        float *yaw)
{
    float norm;
    float sin_pitch;

    if ((roll == NULL) ||
        (pitch == NULL) ||
        (yaw == NULL))
    {
        return 0U;
    }

    norm = sqrtf(
        (qi * qi) +
        (qj * qj) +
        (qk * qk) +
        (qr * qr));

    if (norm < 0.0001f)
    {
        return 0U;
    }

    qi /= norm;
    qj /= norm;
    qk /= norm;
    qr /= norm;

    *roll = atan2f(
        2.0f * ((qr * qi) + (qj * qk)),
        1.0f - 2.0f * ((qi * qi) + (qj * qj)));

    sin_pitch = 2.0f * ((qr * qj) - (qk * qi));

    if (sin_pitch > 1.0f)
    {
        sin_pitch = 1.0f;
    }
    else if (sin_pitch < -1.0f)
    {
        sin_pitch = -1.0f;
    }

    *pitch = asinf(sin_pitch);

    *yaw = atan2f(
        2.0f * ((qr * qk) + (qi * qj)),
        1.0f - 2.0f * ((qj * qj) + (qk * qk)));

    *roll  *= 57.2957795f;
    *pitch *= 57.2957795f;
    *yaw   *= 57.2957795f;

    return 1U;
}

/* ------------------------------------------------------------------------- */
/* API publica                                                               */
/* ------------------------------------------------------------------------- */

uint8_t IMU_Init(void)
{
    imu_initialized = 0U;
    bno085_addr_hal = 0U;
    memset(shtp_sequence, 0, sizeof(shtp_sequence));

    if (HAL_I2C_IsDeviceReady(
            &hi2c1,
            BNO085_ADDR_HAL(BNO085_ADDR_4A_7BIT),
            3U,
            100U) == HAL_OK)
    {
        bno085_addr_hal = BNO085_ADDR_HAL(BNO085_ADDR_4A_7BIT);
    }
    else if (HAL_I2C_IsDeviceReady(
                 &hi2c1,
                 BNO085_ADDR_HAL(BNO085_ADDR_4B_7BIT),
                 3U,
                 100U) == HAL_OK)
    {
        bno085_addr_hal = BNO085_ADDR_HAL(BNO085_ADDR_4B_7BIT);
    }
    else
    {
        return 0U;
    }

    /*
     * Se evita un soft-reset adicional aqui. El sensor ya esta operativo
     * cuando responde a IsDeviceReady y un reset extra puede dejar paquetes
     * de arranque pendientes delante de los reportes de orientacion.
     */
    HAL_Delay(50U);

    if (IMU_CheckProductID() == 0U)
    {
        return 0U;
    }

    if (IMU_EnableRotationVector() == 0U)
    {
        return 0U;
    }

    /* Dar tiempo para que aparezca el primer reporte de 20 Hz. */
    HAL_Delay(100U);

    imu_initialized = 1U;
    return 1U;
}

uint8_t IMU_ReadEuler(
        float *roll,
        float *pitch,
        float *yaw)
{
    uint8_t channel = 0U;
    uint16_t payload_length = 0U;
    uint8_t attempt;
    int16_t raw_qi;
    int16_t raw_qj;
    int16_t raw_qk;
    int16_t raw_qr;
    float qi;
    float qj;
    float qk;
    float qr;
    const float q14_scale = 1.0f / 16384.0f;

    if ((imu_initialized == 0U) ||
        (roll == NULL) ||
        (pitch == NULL) ||
        (yaw == NULL))
    {
        return 0U;
    }

    /*
     * Puede haber paquetes de control o paquetes vacios delante del dato de
     * orientacion. No se abandona al primer intento fallido: se revisan varios
     * paquetes antes de devolver 0.
     */
    for (attempt = 0U; attempt < IMU_MAX_PACKETS_PER_READ; attempt++)
    {
        if (IMU_ReceivePacket(
                &channel,
                shtp_payload,
                sizeof(shtp_payload),
                &payload_length) == 0U)
        {
            HAL_Delay(2U);
            continue;
        }

        if (channel != SHTP_CHANNEL_REPORTS)
        {
            continue;
        }

        /*
         * Entrada SH-2 normal:
         * payload[0..4]  = Base Timestamp
         * payload[5]     = Feature Report ID
         * payload[9..10] = Quaternion I
         * payload[11..12]= Quaternion J
         * payload[13..14]= Quaternion K
         * payload[15..16]= Quaternion Real
         */
        if (payload_length < 17U)
        {
            continue;
        }

        if (shtp_payload[5] != REPORT_ID_ROTATION_VECTOR)
        {
            continue;
        }

        raw_qi = IMU_ReadInt16LE(&shtp_payload[9]);
        raw_qj = IMU_ReadInt16LE(&shtp_payload[11]);
        raw_qk = IMU_ReadInt16LE(&shtp_payload[13]);
        raw_qr = IMU_ReadInt16LE(&shtp_payload[15]);

        qi = (float)raw_qi * q14_scale;
        qj = (float)raw_qj * q14_scale;
        qk = (float)raw_qk * q14_scale;
        qr = (float)raw_qr * q14_scale;

        return IMU_QuaternionToEuler(
            qi,
            qj,
            qk,
            qr,
            roll,
            pitch,
            yaw);
    }

    return 0U;
}

uint8_t IMU_GetAddress7bit(void)
{
    if (bno085_addr_hal == 0U)
    {
        return 0U;
    }

    return (uint8_t)(bno085_addr_hal >> 1U);
}
