
/*
 *VL53L1X.h
 *
 *  Created on: 19/04/2026
 *      Adaptada por: Alcides Ramos
 */


#ifndef VL53L1X_H
#define VL53L1X_H

#include "main.h"
#include <stdint.h>
#include <stdbool.h>


//DIRECCION I2C
#define VL53L1X_ADDRESS   0x29<<1
//i2c a usar
#define  LIDARI2C hi2c1


// SHORT      MEDIUM    LARGE
//    50MS       100MS    200MS
 //    1.3M       3M       4M


/*
//SHORT
VL53L1X_SetMeasurementTimingBudget(&LIDAR, 50000);
VL53L1X_StartContinuous(&LIDAR, 50);

//MEDIUM
VL53L1X_SetMeasurementTimingBudget(&LIDAR, 50000);
VL53L1X_StartContinuous(&LIDAR, 100);

//LARGE
VL53L1X_SetMeasurementTimingBudget(&LIDAR, 140000);
VL53L1X_StartContinuous(&LIDAR, 150);

*/
/* ───────────────────────────────────────────────
 * Timeout I2C HAL (ms)
 * ─────────────────────────────────────────────── */
#define VL53L1X_I2C_TIMEOUT_MS    50U

/* ───────────────────────────────────────────────
 * Constantes internas
 * ─────────────────────────────────────────────── */
#define VL53L1X_TIMING_GUARD      4528UL
#define VL53L1X_TARGET_RATE       0x0A00U

/* ───────────────────────────────────────────────
 * Modos de distancia
 * ─────────────────────────────────────────────── */
typedef enum {
    VL53L1X_DISTANCE_UNKNOWN = 0,
    VL53L1X_SHORT,
    VL53L1X_MEDIUM,
    VL53L1X_LONG
} VL53L1X_DistanceMode_t;

extern I2C_HandleTypeDef  LIDARI2C;

/* ───────────────────────────────────────────────
 * Estado del rango
 * ─────────────────────────────────────────────── */
typedef enum {
    VL53L1X_RANGE_VALID                    = 0,
    VL53L1X_SIGMA_FAIL                     = 1,
    VL53L1X_SIGNAL_FAIL                    = 2,
    VL53L1X_RANGE_VALID_MIN_RANGE_CLIPPED  = 3,
    VL53L1X_OUT_OF_BOUNDS_FAIL             = 4,
    VL53L1X_HARDWARE_FAIL                  = 5,
    VL53L1X_RANGE_VALID_NO_WRAP_CHECK_FAIL = 6,
    VL53L1X_WRAP_TARGET_FAIL               = 7,
    VL53L1X_XTALK_SIGNAL_FAIL              = 8,
    VL53L1X_SYNCHRONIZATION_INT            = 9,
    VL53L1X_MIN_RANGE_FAIL                 = 10,
    VL53L1X_NONE                           = 11
} VL53L1X_RangeStatus_t;

/* ───────────────────────────────────────────────
 * Datos de la última medición
 * ─────────────────────────────────────────────── */
typedef struct {
    uint16_t              range_mm;
    VL53L1X_RangeStatus_t range_status;
    float                 peak_signal_count_rate_MCPS;
    float                 ambient_count_rate_MCPS;
} VL53L1X_RangingData_t;

/* ───────────────────────────────────────────────
 * Buffer de resultados internos
 * ─────────────────────────────────────────────── */
typedef struct {
    uint8_t  range_status;
    uint8_t  stream_count;
    uint16_t dss_actual_effective_spads_sd0;
    uint16_t ambient_count_rate_mcps_sd0;
    uint16_t final_crosstalk_corrected_range_mm_sd0;
    uint16_t peak_signal_count_rate_crosstalk_corrected_mcps_sd0;
} VL53L1X_ResultBuffer_t;

/* ───────────────────────────────────────────────
 * Descriptor del sensor (equivale al objeto C++)
 * ─────────────────────────────────────────────── */
typedef struct {
    I2C_HandleTypeDef     *hi2c;          /* Handle I2C de HAL                   */
    uint8_t                address;       /* Dirección I2C (formato HAL: 8 bits) */

    uint16_t               io_timeout;    /* Timeout en ms (0 = sin timeout)     */
    bool                   did_timeout;

    uint16_t               fast_osc_frequency;
    uint16_t               osc_calibrate_val;

    bool                   calibrated;
    uint8_t                saved_vhv_init;
    uint8_t                saved_vhv_timeout;

    VL53L1X_DistanceMode_t distance_mode;
    VL53L1X_RangingData_t  ranging_data;
    VL53L1X_ResultBuffer_t results;

    uint8_t                last_status;   /* Último estado I2C HAL               */
    uint32_t               timeout_start_ms;
} VL53L1X_Dev_t;

/* ═══════════════════════════════════════════════
 * API pública
 * ═══════════════════════════════════════════════ */

/**
 * Inicializa el descriptor con valores por defecto.
 * hi2c    : puntero al handle I2C de HAL (e.g. &hi2c1)
 * address : dirección 8 bits para HAL  (default: VL53L1X_ADDRESS_DEFAULT = 0x52)
 * io_2v8  : true = modo 2V8 (lo normal), false = 1V8
 * Retorna true si el sensor responde y se configura correctamente.
 */
bool VL53L1X_Init();

/** Cambia la dirección I2C del sensor (se escribe en el registro del sensor). */
void VL53L1X_SetAddress( uint8_t new_addr);

/* ── Modos de distancia ── */
bool                   VL53L1X_SetDistanceMode( VL53L1X_DistanceMode_t mode);
VL53L1X_DistanceMode_t VL53L1X_GetDistanceMode( );

/* ── Timing budget ── */
bool     VL53L1X_SetMeasurementTimingBudget( uint32_t budget_us);
uint32_t VL53L1X_GetMeasurementTimingBudget();

/* ── ROI ── */
void    VL53L1X_SetROISize( uint8_t width, uint8_t height);
void    VL53L1X_GetROISize( uint8_t *width, uint8_t *height);
void    VL53L1X_SetROICenter( uint8_t spadNumber);
uint8_t VL53L1X_GetROICenter();

/* ── Medición continua ── */
void VL53L1X_StartContinuous( uint32_t period_ms);
void VL53L1X_StopContinuous();

/**
 * Lee la distancia en modo continuo.
 * blocking = true  → espera hasta que haya dato nuevo (o timeout).
 * blocking = false → intenta retornar inmediatamente (puede retornar 0).
 */
uint16_t VL53L1X_Read( );

/**
 * Dispara una medición individual y retorna la distancia (mm).
 * blocking = true  → espera el resultado.
 * blocking = false → retorna 0 inmediatamente.
 */
uint16_t VL53L1X_ReadSingle( bool blocking);

/** Verifica si hay nuevo dato disponible (GPIO_TIO_HV_STATUS bit 0 == 0). */
bool VL53L1X_DataReady();

/* ── Timeout ── */
void VL53L1X_SetTimeout( uint16_t timeout_ms);
uint16_t VL53L1X_GetTimeout( );
bool VL53L1X_TimeoutOccurred();


const char *VL53L1X_RangeStatusToString(VL53L1X_RangeStatus_t status);


void     VL53L1X_WriteReg( uint16_t reg, uint8_t value);
void     VL53L1X_WriteReg16( uint16_t reg, uint16_t value);
void     VL53L1X_WriteReg32( uint16_t reg, uint32_t value);
uint8_t  VL53L1X_ReadReg( uint16_t reg);
uint16_t VL53L1X_ReadReg16( uint16_t reg);
uint32_t VL53L1X_ReadReg32( uint16_t reg);
extern VL53L1X_Dev_t LIDAR;

#endif /* VL53L1X_H */
