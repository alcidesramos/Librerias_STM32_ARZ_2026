
/*
 * max30102.h.c
 *
 *  Created on: 28/04/2026
 *      Author: Alcides Ramos
 */

#include <float.h>
#include <math.h>
#include <max30102.h>
#include <string.h>

//variables estructura dels en sor
        MAX30102_t sensor;
        MAX30102_Config_t config;
        MAX30102_Data_t data;


#define MAX30102_REG_INTR_STATUS_1   0x00U
#define MAX30102_REG_INTR_STATUS_2   0x01U
#define MAX30102_REG_INTR_ENABLE_1   0x02U
#define MAX30102_REG_INTR_ENABLE_2   0x03U
#define MAX30102_REG_FIFO_WR_PTR     0x04U
#define MAX30102_REG_OVF_COUNTER     0x05U
#define MAX30102_REG_FIFO_RD_PTR     0x06U
#define MAX30102_REG_FIFO_DATA       0x07U
#define MAX30102_REG_FIFO_CONFIG     0x08U
#define MAX30102_REG_MODE_CONFIG     0x09U
#define MAX30102_REG_SPO2_CONFIG     0x0AU
#define MAX30102_REG_LED1_PA         0x0CU
#define MAX30102_REG_LED2_PA         0x0DU
#define MAX30102_REG_TEMP_INT        0x1FU
#define MAX30102_REG_TEMP_FRAC       0x20U
#define MAX30102_REG_TEMP_CONFIG     0x21U
#define MAX30102_REG_PART_ID         0xFFU

#define MAX30102_MODE_RESET          0x40U
#define MAX30102_MODE_SPO2           0x03U
#define MAX30102_TEMP_EN             0x01U

#define MAX30102_FIFO_DEPTH          32U
#define MAX30102_FIFO_MASK           0x3FFFFUL
#define MAX30102_I2C_TIMEOUT         100U
#define MAX30102_RESET_TIMEOUT_MS    100U
#define MAX30102_TEMP_TIMEOUT_MS     100U

#define MAX30102_MIN_SAMPLES         64U
#define MAX30102_MAX_FIFO_SAMPLES    32U
#define MAX30102_EPSILON             0.0001f
#define MAX30102_MIN_AC_RMS          20.0f

typedef struct
{
    uint32_t red;
    uint32_t ir;
} max30102_raw_sample_t;

static HAL_StatusTypeDef max30102_read_reg(MAX30102_t *dev, uint8_t reg, uint8_t *value);
static HAL_StatusTypeDef max30102_write_reg(MAX30102_t *dev, uint8_t reg, uint8_t value);
static HAL_StatusTypeDef max30102_read_multi(MAX30102_t *dev, uint8_t reg, uint8_t *data, uint16_t length);
static HAL_StatusTypeDef max30102_soft_reset(MAX30102_t *dev);
static HAL_StatusTypeDef max30102_read_part_id(MAX30102_t *dev, uint8_t *part_id);
static HAL_StatusTypeDef max30102_configure(MAX30102_t *dev, const MAX30102_Config_t *config);
static HAL_StatusTypeDef max30102_read_status(MAX30102_t *dev);
static HAL_StatusTypeDef max30102_available_samples(MAX30102_t *dev, uint8_t *available);
static HAL_StatusTypeDef max30102_read_samples(MAX30102_t *dev,
                                               max30102_raw_sample_t *samples,
                                               uint8_t max_samples,
                                               uint8_t *read_count);
static void max30102_push_sample(MAX30102_t *dev, const max30102_raw_sample_t *sample);
static uint16_t max30102_buffer_start(const MAX30102_t *dev);
static uint16_t max30102_buffer_index(const MAX30102_t *dev, uint16_t logical_index);
static uint8_t max30102_process(const MAX30102_t *dev, MAX30102_Data_t *data);
static float max30102_clampf(float value, float min_value, float max_value);
static uint8_t max30102_sample_rate_bits(uint16_t sample_rate_hz, uint16_t *real_rate_hz);
//configuracion por defecto si no se colocan
void MAX30102_DefaultConfig(MAX30102_Config_t *config)
{
    if (config == NULL)
    {
        return;
    }

    config->sample_rate_hz = 100U;
    config->led_red_current = 0x24U;
    config->led_ir_current = 0x24U;
    config->finger_threshold_ir = 15000U;
    config->min_bpm = 45U;
    config->max_bpm = 180U;
    config->temperature_period_ms = 1000U;
}

HAL_StatusTypeDef MAX30102_Init()
{
    MAX30102_Config_t default_config;
    HAL_StatusTypeDef status;
    uint8_t part_id = 0U;


    memset(&sensor, 0, sizeof(sensor));
    sensor.hi2c = &max30102_i2c;
    sensor.i2c_addr = MAX30102_I2C_ADDR;

    status = max30102_soft_reset(&sensor);
    if (status != HAL_OK)
    {
        return status;
    }

    status = max30102_read_part_id(&sensor, &part_id);
    if (status != HAL_OK)
    {
        return status;
    }

    if (part_id != MAX30102_PART_ID_VALUE)
    {
        return HAL_ERROR;
    }

    return max30102_configure(&sensor, &config);
}

HAL_StatusTypeDef MAX30102_Update( )
{
    HAL_StatusTypeDef status;
    max30102_raw_sample_t samples[MAX30102_MAX_FIFO_SAMPLES];
    uint8_t read_count = 0U;
    uint16_t required_samples;
    uint8_t i;

    if ((&sensor == NULL) || (&data == NULL))
    {
        return HAL_ERROR;
    }

    memset(&data, 0, sizeof(data));
    data.temperature_c = sensor.last_temperature_c;

    if ((sensor.last_temperature_tick == 0U) ||
        ((HAL_GetTick() - sensor.last_temperature_tick) >= sensor.temperature_period_ms))
    {
        float temp_c = 0.0f;

        status = MAX30102_ReadTemperature( &temp_c);
        if (status == HAL_OK)
        {
            sensor.last_temperature_c = temp_c;
            sensor.last_temperature_tick = HAL_GetTick();
            data.temperature_c = temp_c;
        }
    }

    status = max30102_read_samples(&sensor, samples, MAX30102_MAX_FIFO_SAMPLES, &read_count);
    if (status != HAL_OK)
    {
        return status;
    }

    for (i = 0U; i < read_count; i++)
    {
        max30102_push_sample(&sensor, &samples[i]);
    }

    required_samples = (uint16_t)(sensor.sample_rate_hz * 2U);
    if (required_samples < MAX30102_MIN_SAMPLES)
    {
        required_samples = MAX30102_MIN_SAMPLES;
    }

    if (required_samples > MAX30102_BUFFER_SIZE)
    {
        required_samples = MAX30102_BUFFER_SIZE;
    }

    if (sensor.count < required_samples)
    {
        return HAL_OK;
    }

    data.valid = max30102_process(&sensor, &data);
    data.temperature_c = sensor.last_temperature_c;

    return HAL_OK;
}

HAL_StatusTypeDef MAX30102_ReadTemperature( float *temperature_c)
{
    HAL_StatusTypeDef status;
    uint8_t temp_cfg = 0U;
    uint8_t temp_int = 0U;
    uint8_t temp_frac = 0U;
    uint32_t tick_start;

    if ((&sensor == NULL) || (temperature_c == NULL))
    {
        return HAL_ERROR;
    }

    status = max30102_write_reg(&sensor, MAX30102_REG_TEMP_CONFIG, MAX30102_TEMP_EN);
    if (status != HAL_OK)
    {
        return status;
    }

    tick_start = HAL_GetTick();

    do
    {
        status = max30102_read_reg(&sensor, MAX30102_REG_TEMP_CONFIG, &temp_cfg);
        if (status != HAL_OK)
        {
            return status;
        }

        if ((temp_cfg & MAX30102_TEMP_EN) == 0U)
        {
            break;
        }

        HAL_Delay(1U);
    } while ((HAL_GetTick() - tick_start) < MAX30102_TEMP_TIMEOUT_MS);

    if ((temp_cfg & MAX30102_TEMP_EN) != 0U)
    {
        return HAL_TIMEOUT;
    }

    status = max30102_read_reg(&sensor, MAX30102_REG_TEMP_INT, &temp_int);
    if (status != HAL_OK)
    {
        return status;
    }

    status = max30102_read_reg(&sensor, MAX30102_REG_TEMP_FRAC, &temp_frac);
    if (status != HAL_OK)
    {
        return status;
    }

    *temperature_c = (float)((int8_t)temp_int) + ((float)(temp_frac & 0x0FU) * 0.0625f);
    return HAL_OK;
}

HAL_StatusTypeDef MAX30102_ClearFIFO()
{
    HAL_StatusTypeDef status;

    if (&sensor == NULL)
    {
        return HAL_ERROR;
    }

    status = max30102_write_reg(&sensor, MAX30102_REG_FIFO_WR_PTR, 0x00U);
    if (status != HAL_OK)
    {
        return status;
    }

    status = max30102_write_reg(&sensor, MAX30102_REG_OVF_COUNTER, 0x00U);
    if (status != HAL_OK)
    {
        return status;
    }

    return max30102_write_reg(&sensor, MAX30102_REG_FIFO_RD_PTR, 0x00U);
}

static HAL_StatusTypeDef max30102_read_reg(MAX30102_t *dev, uint8_t reg, uint8_t *value)
{
    if ((&sensor == NULL) || (sensor.hi2c == NULL) || (value == NULL))
    {
        return HAL_ERROR;
    }

    return HAL_I2C_Mem_Read(sensor.hi2c,
                            sensor.i2c_addr,
                            reg,
                            I2C_MEMADD_SIZE_8BIT,
                            value,
                            1U,
                            MAX30102_I2C_TIMEOUT);
}

static HAL_StatusTypeDef max30102_write_reg(MAX30102_t *dev, uint8_t reg, uint8_t value)
{
    if ((&sensor == NULL) || (sensor.hi2c == NULL))
    {
        return HAL_ERROR;
    }

    return HAL_I2C_Mem_Write(sensor.hi2c,
                             sensor.i2c_addr,
                             reg,
                             I2C_MEMADD_SIZE_8BIT,
                             &value,
                             1U,
                             MAX30102_I2C_TIMEOUT);
}

static HAL_StatusTypeDef max30102_read_multi(MAX30102_t *dev, uint8_t reg, uint8_t *data, uint16_t length)
{
    if ((&sensor == NULL) || (sensor.hi2c == NULL) || (data == NULL) || (length == 0U))
    {
        return HAL_ERROR;
    }

    return HAL_I2C_Mem_Read(sensor.hi2c,
                            sensor.i2c_addr,
                            reg,
                            I2C_MEMADD_SIZE_8BIT,
                            data,
                            length,
                            MAX30102_I2C_TIMEOUT);
}

static HAL_StatusTypeDef max30102_soft_reset(MAX30102_t *dev)
{
    HAL_StatusTypeDef status;
    uint8_t mode = 0U;
    uint32_t tick_start;

    status = max30102_write_reg(&sensor, MAX30102_REG_MODE_CONFIG, MAX30102_MODE_RESET);
    if (status != HAL_OK)
    {
        return status;
    }

    tick_start = HAL_GetTick();

    do
    {
        status = max30102_read_reg(&sensor, MAX30102_REG_MODE_CONFIG, &mode);
        if (status != HAL_OK)
        {
            return status;
        }

        if ((mode & MAX30102_MODE_RESET) == 0U)
        {
            return HAL_OK;
        }

        HAL_Delay(1U);
    } while ((HAL_GetTick() - tick_start) < MAX30102_RESET_TIMEOUT_MS);

    return HAL_TIMEOUT;
}

static HAL_StatusTypeDef max30102_read_part_id(MAX30102_t *dev, uint8_t *part_id)
{
    return max30102_read_reg(&sensor, MAX30102_REG_PART_ID, part_id);
}

static HAL_StatusTypeDef max30102_configure(MAX30102_t *dev, const MAX30102_Config_t *config)
{
    HAL_StatusTypeDef status;
    uint8_t fifo_cfg = 0U;
    uint8_t spo2_cfg = 0U;
    uint8_t sample_rate_bits;
    uint16_t real_sample_rate_hz;

    if ((&sensor == NULL) || (config == NULL))
    {
        return HAL_ERROR;
    }

    sample_rate_bits = max30102_sample_rate_bits(config->sample_rate_hz, &real_sample_rate_hz);

    status = max30102_write_reg(&sensor, MAX30102_REG_INTR_ENABLE_1, 0x00U);
    if (status != HAL_OK)
    {
        return status;
    }

    status = max30102_write_reg(&sensor, MAX30102_REG_INTR_ENABLE_2, 0x00U);
    if (status != HAL_OK)
    {
        return status;
    }

    status = MAX30102_ClearFIFO(&sensor);
    if (status != HAL_OK)
    {
        return status;
    }

    fifo_cfg = (uint8_t)((0x02U << 5) | (1U << 4) | 0x0FU);
    status = max30102_write_reg(&sensor, MAX30102_REG_FIFO_CONFIG, fifo_cfg);
    if (status != HAL_OK)
    {
        return status;
    }

    spo2_cfg = (uint8_t)((0x01U << 5) | (sample_rate_bits << 2) | 0x03U);
    status = max30102_write_reg(&sensor, MAX30102_REG_SPO2_CONFIG, spo2_cfg);
    if (status != HAL_OK)
    {
        return status;
    }

    status = max30102_write_reg(&sensor, MAX30102_REG_LED1_PA, config->led_red_current);
    if (status != HAL_OK)
    {
        return status;
    }

    status = max30102_write_reg(&sensor, MAX30102_REG_LED2_PA, config->led_ir_current);
    if (status != HAL_OK)
    {
        return status;
    }

    status = max30102_write_reg(&sensor, MAX30102_REG_MODE_CONFIG, MAX30102_MODE_SPO2);
    if (status != HAL_OK)
    {
        return status;
    }

    status = MAX30102_ClearFIFO(&sensor);
    if (status != HAL_OK)
    {
        return status;
    }

    status = max30102_read_status(&sensor);
    if (status != HAL_OK)
    {
        return status;
    }

    sensor.sample_rate_hz = real_sample_rate_hz;
    sensor.finger_threshold_ir = config->finger_threshold_ir;
    sensor.min_bpm = config->min_bpm;
    sensor.max_bpm = config->max_bpm;
    sensor.temperature_period_ms = config->temperature_period_ms;
    sensor.last_temperature_tick = 0U;
    sensor.last_temperature_c = 0.0f;
    sensor.head = 0U;
    sensor.count = 0U;

    return HAL_OK;
}

static HAL_StatusTypeDef max30102_read_status(MAX30102_t *dev)
{
    uint8_t status_regs[2];
    return max30102_read_multi(&sensor, MAX30102_REG_INTR_STATUS_1, status_regs, 2U);
}

static HAL_StatusTypeDef max30102_available_samples(MAX30102_t *dev, uint8_t *available)
{
    HAL_StatusTypeDef status;
    uint8_t wr_ptr = 0U;
    uint8_t rd_ptr = 0U;

    if ((&sensor == NULL) || (available == NULL))
    {
        return HAL_ERROR;
    }

    status = max30102_read_reg(&sensor, MAX30102_REG_FIFO_WR_PTR, &wr_ptr);
    if (status != HAL_OK)
    {
        return status;
    }

    status = max30102_read_reg(&sensor, MAX30102_REG_FIFO_RD_PTR, &rd_ptr);
    if (status != HAL_OK)
    {
        return status;
    }

    wr_ptr &= 0x1FU;
    rd_ptr &= 0x1FU;

    if (wr_ptr >= rd_ptr)
    {
        *available = (uint8_t)(wr_ptr - rd_ptr);
    }
    else
    {
        *available = (uint8_t)(MAX30102_FIFO_DEPTH - rd_ptr + wr_ptr);
    }

    return HAL_OK;
}

static HAL_StatusTypeDef max30102_read_samples(MAX30102_t *dev,
                                               max30102_raw_sample_t *samples,
                                               uint8_t max_samples,
                                               uint8_t *read_count)
{
    HAL_StatusTypeDef status;
    uint8_t available = 0U;
    uint8_t sample_count;
    uint8_t raw[MAX30102_MAX_FIFO_SAMPLES * 6U];
    uint8_t i;

    if ((&sensor == NULL) || (samples == NULL))
    {
        return HAL_ERROR;
    }

    if (read_count != NULL)
    {
        *read_count = 0U;
    }

    if (max_samples == 0U)
    {
        return HAL_OK;
    }

    status = max30102_available_samples(&sensor, &available);
    if (status != HAL_OK)
    {
        return status;
    }

    sample_count = (available < max_samples) ? available : max_samples;
    if (sample_count == 0U)
    {
        return HAL_OK;
    }

    status = max30102_read_multi(&sensor, MAX30102_REG_FIFO_DATA, raw, (uint16_t)sample_count * 6U);
    if (status != HAL_OK)
    {
        return status;
    }

    for (i = 0U; i < sample_count; i++)
    {
        uint16_t offset = (uint16_t)i * 6U;
        samples[i].red = (((uint32_t)raw[offset] << 16) |
                          ((uint32_t)raw[offset + 1U] << 8) |
                          ((uint32_t)raw[offset + 2U])) & MAX30102_FIFO_MASK;
        samples[i].ir = (((uint32_t)raw[offset + 3U] << 16) |
                         ((uint32_t)raw[offset + 4U] << 8) |
                         ((uint32_t)raw[offset + 5U])) & MAX30102_FIFO_MASK;
    }

    if (read_count != NULL)
    {
        *read_count = sample_count;
    }

    return HAL_OK;
}

static void max30102_push_sample(MAX30102_t *dev, const max30102_raw_sample_t *sample)
{
    if ((&sensor == NULL) || (sample == NULL))
    {
        return;
    }

    sensor.red_buffer[sensor.head] = sample->red;
    sensor.ir_buffer[sensor.head] = sample->ir;
    sensor.head = (uint16_t)((sensor.head + 1U) % MAX30102_BUFFER_SIZE);

    if (sensor.count < MAX30102_BUFFER_SIZE)
    {
        sensor.count++;
    }
}

static uint16_t max30102_buffer_start(const MAX30102_t *dev)
{
    if ((&sensor == NULL) || (sensor.count < MAX30102_BUFFER_SIZE))
    {
        return 0U;
    }

    return sensor.head;
}

static uint16_t max30102_buffer_index(const MAX30102_t *dev, uint16_t logical_index)
{
    uint16_t start = max30102_buffer_start(&sensor);
    return (uint16_t)((start + logical_index) % MAX30102_BUFFER_SIZE);
}

static uint8_t max30102_process(const MAX30102_t *dev, MAX30102_Data_t *data)
{
    float red_mean = 0.0f;
    float ir_mean = 0.0f;
    float red_var_sum = 0.0f;
    float ir_var_sum = 0.0f;
    float signal[MAX30102_BUFFER_SIZE];
    float max_value = -FLT_MAX;
    float min_value = FLT_MAX;
    float red_ac_rms;
    float ir_ac_rms;
    float ratio;
    float threshold;
    uint16_t min_distance;
    uint16_t max_distance;
    uint16_t prev_peak = 0U;
    uint8_t has_prev_peak = 0U;
    uint16_t interval_count = 0U;
    float interval_sum = 0.0f;
    uint16_t i;
    uint16_t sample_count;

    if ((&sensor == NULL) || (data == NULL))
    {
        return 0U;
    }

    sample_count = sensor.count;
    if (sample_count > MAX30102_BUFFER_SIZE)
    {
        sample_count = MAX30102_BUFFER_SIZE;
    }

    for (i = 0U; i < sample_count; i++)
    {
        uint16_t idx = max30102_buffer_index(&sensor, i);
        red_mean += (float)sensor.red_buffer[idx];
        ir_mean += (float)sensor.ir_buffer[idx];
    }

    red_mean /= (float)sample_count;
    ir_mean /= (float)sample_count;

    if ((red_mean < 1.0f) || (ir_mean < 1.0f))
    {
        return 0U;
    }

    for (i = 0U; i < sample_count; i++)
    {
        uint16_t idx = max30102_buffer_index(&sensor, i);
        float red_delta = (float)sensor.red_buffer[idx] - red_mean;
        float ir_delta = (float)sensor.ir_buffer[idx] - ir_mean;

        red_var_sum += red_delta * red_delta;
        ir_var_sum += ir_delta * ir_delta;
        signal[i] = ir_delta;

        if (signal[i] > max_value)
        {
            max_value = signal[i];
        }

        if (signal[i] < min_value)
        {
            min_value = signal[i];
        }
    }

    red_ac_rms = sqrtf(red_var_sum / (float)sample_count);
    ir_ac_rms = sqrtf(ir_var_sum / (float)sample_count);

    data->finger_present = (ir_mean >= (float)sensor.finger_threshold_ir) &&
                           (ir_ac_rms >= MAX30102_MIN_AC_RMS);

    if (data->finger_present == 0U)
    {
        return 0U;
    }

    if ((-min_value) > max_value)
    {
        for (i = 0U; i < sample_count; i++)
        {
            signal[i] = -signal[i];
        }
    }

    if ((red_ac_rms < MAX30102_EPSILON) ||
        (ir_ac_rms < MAX30102_EPSILON) ||
        (red_mean < MAX30102_EPSILON) ||
        (ir_mean < MAX30102_EPSILON))
    {
        return 0U;
    }

    ratio = (red_ac_rms / red_mean) / (ir_ac_rms / ir_mean);
    data->spo2 = -45.060f * ratio * ratio + 30.354f * ratio + 94.845f;
    data->spo2 = max30102_clampf(data->spo2, 0.0f, 100.0f);

    if ((sensor.sample_rate_hz == 0U) ||
        (sensor.min_bpm == 0U) ||
        (sensor.max_bpm == 0U) ||
        (sensor.min_bpm >= sensor.max_bpm))
    {
        return 0U;
    }

    min_distance = (uint16_t)(((uint32_t)sensor.sample_rate_hz * 60U) / sensor.max_bpm);
    max_distance = (uint16_t)(((uint32_t)sensor.sample_rate_hz * 60U) / sensor.min_bpm);

    if (min_distance == 0U)
    {
        min_distance = 1U;
    }

    threshold = ir_ac_rms * 0.6f;
    if (threshold < MAX30102_MIN_AC_RMS)
    {
        threshold = MAX30102_MIN_AC_RMS;
    }

    if (sample_count < 5U)
    {
        return 0U;
    }

    for (i = 2U; i < (uint16_t)(sample_count - 2U); i++)
    {
        float s_prev = (signal[i - 2U] + signal[i - 1U] + signal[i]) / 3.0f;
        float s_curr = (signal[i - 1U] + signal[i] + signal[i + 1U]) / 3.0f;
        float s_next = (signal[i] + signal[i + 1U] + signal[i + 2U]) / 3.0f;

        if ((s_curr > threshold) && (s_curr > s_prev) && (s_curr >= s_next))
        {
            if ((has_prev_peak == 0U) || ((i - prev_peak) >= min_distance))
            {
                if (has_prev_peak != 0U)
                {
                    uint16_t interval = (uint16_t)(i - prev_peak);

                    if ((interval >= min_distance) && (interval <= max_distance))
                    {
                        interval_sum += (float)interval;
                        interval_count++;
                    }
                }

                prev_peak = i;
                has_prev_peak = 1U;
            }
        }
    }

    if (interval_count == 0U)
    {
        return 0U;
    }

    data->bpm = (60.0f * (float)sensor.sample_rate_hz) / (interval_sum / (float)interval_count);

    if ((data->bpm < (float)sensor.min_bpm) || (data->bpm > (float)sensor.max_bpm))
    {
        return 0U;
    }

    return 1U;
}

static float max30102_clampf(float value, float min_value, float max_value)
{
    if (value < min_value)
    {
        return min_value;
    }

    if (value > max_value)
    {
        return max_value;
    }

    return value;
}

static uint8_t max30102_sample_rate_bits(uint16_t sample_rate_hz, uint16_t *real_rate_hz)
{
    uint8_t bits = 0x01U;
    uint16_t rate = 100U;

    switch (sample_rate_hz)
    {
        case 50U:
            bits = 0x00U;
            rate = 50U;
            break;

        case 100U:
            bits = 0x01U;
            rate = 100U;
            break;

        case 200U:
            bits = 0x02U;
            rate = 200U;
            break;

        case 400U:
            bits = 0x03U;
            rate = 400U;
            break;

        case 800U:
            bits = 0x04U;
            rate = 800U;
            break;

        case 1000U:
            bits = 0x05U;
            rate = 1000U;
            break;

        case 1600U:
            bits = 0x06U;
            rate = 1600U;
            break;

        case 3200U:
            bits = 0x07U;
            rate = 3200U;
            break;

        default:
            bits = 0x01U;
            rate = 100U;
            break;
    }

    if (real_rate_hz != NULL)
    {
        *real_rate_hz = rate;
    }

    return bits;
}
