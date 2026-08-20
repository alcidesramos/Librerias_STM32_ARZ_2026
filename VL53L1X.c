/*
 *VL53L1X.c
 *
 *  Created on: 19/04/2026
 *      Adaptada por: Alcides Ramos
 */


#include "VL53L1X.h"

VL53L1X_Dev_t LIDAR;


#define REG_SOFT_RESET                                          0x0000U
#define REG_I2C_SLAVE__DEVICE_ADDRESS                           0x0001U
#define REG_PAD_I2C_HV__EXTSUP_CONFIG                           0x002EU
#define REG_OSC_MEASURED__FAST_OSC__FREQUENCY                   0x0006U
#define REG_VHV_CONFIG__TIMEOUT_MACROP_LOOP_BOUND               0x0008U
#define REG_VHV_CONFIG__INIT                                    0x000BU
#define REG_ALGO__CROSSTALK_COMPENSATION_VALID_HEIGHT_MM        0x0039U
#define REG_ALGO__RANGE_IGNORE_VALID_HEIGHT_MM                  0x003EU
#define REG_ALGO__RANGE_MIN_CLIP                                0x003FU
#define REG_ALGO__CONSISTENCY_CHECK__TOLERANCE                  0x0040U
#define REG_DSS_CONFIG__TARGET_TOTAL_RATE_MCPS                  0x0024U
#define REG_GPIO__TIO_HV_STATUS                                 0x0031U
#define REG_SIGMA_ESTIMATOR__EFFECTIVE_PULSE_WIDTH_NS           0x0036U
#define REG_SIGMA_ESTIMATOR__EFFECTIVE_AMBIENT_WIDTH_NS         0x0037U
#define REG_SYSTEM__THRESH_RATE_HIGH                            0x0050U
#define REG_SYSTEM__THRESH_RATE_LOW                             0x0052U
#define REG_DSS_CONFIG__APERTURE_ATTENUATION                    0x0057U
#define REG_RANGE_CONFIG__SIGMA_THRESH                          0x0064U
#define REG_RANGE_CONFIG__MIN_COUNT_RATE_RTN_LIMIT_MCPS         0x0066U
#define REG_SYSTEM__GROUPED_PARAMETER_HOLD_0                    0x0071U
#define REG_SYSTEM__GROUPED_PARAMETER_HOLD_1                    0x007CU
#define REG_SD_CONFIG__QUANTIFIER                               0x007EU
#define REG_SYSTEM__GROUPED_PARAMETER_HOLD                      0x0082U
#define REG_SYSTEM__SEED_CONFIG                                 0x0077U
#define REG_SYSTEM__SEQUENCE_CONFIG                             0x0081U
#define REG_DSS_CONFIG__MANUAL_EFFECTIVE_SPADS_SELECT           0x0054U
#define REG_DSS_CONFIG__ROI_MODE_CONTROL                        0x004FU
#define REG_ALGO__PART_TO_PART_RANGE_OFFSET_MM                  0x001EU
#define REG_MM_CONFIG__OUTER_OFFSET_MM                          0x0022U
#define REG_RANGE_CONFIG__VCSEL_PERIOD_A                        0x0060U
#define REG_RANGE_CONFIG__VCSEL_PERIOD_B                        0x0063U
#define REG_RANGE_CONFIG__VALID_PHASE_HIGH                      0x0069U
#define REG_SD_CONFIG__WOI_SD0                                  0x0078U
#define REG_SD_CONFIG__WOI_SD1                                  0x0079U
#define REG_SD_CONFIG__INITIAL_PHASE_SD0                        0x007AU
#define REG_SD_CONFIG__INITIAL_PHASE_SD1                        0x007BU
#define REG_PHASECAL_CONFIG__TIMEOUT_MACROP                     0x004BU
#define REG_MM_CONFIG__TIMEOUT_MACROP_A                         0x005AU
#define REG_RANGE_CONFIG__TIMEOUT_MACROP_A                      0x005EU
#define REG_MM_CONFIG__TIMEOUT_MACROP_B                         0x005CU
#define REG_RANGE_CONFIG__TIMEOUT_MACROP_B                      0x0061U
#define REG_SYSTEM__INTERMEASUREMENT_PERIOD                     0x006CU
#define REG_SYSTEM__INTERRUPT_CLEAR                             0x0086U
#define REG_SYSTEM__MODE_START                                  0x0087U
#define REG_RESULT__RANGE_STATUS                                0x0089U
#define REG_RESULT__OSC_CALIBRATE_VAL                           0x00DEU
#define REG_FIRMWARE__SYSTEM_STATUS                             0x00E5U
#define REG_IDENTIFICATION__MODEL_ID                            0x010FU
#define REG_PHASECAL_CONFIG__OVERRIDE                           0x004DU
#define REG_PHASECAL_RESULT__VCSEL_START                        0x00D8U
#define REG_CAL_CONFIG__VCSEL_START                             0x0047U
#define REG_ROI_CONFIG__USER_ROI_CENTRE_SPAD                    0x007FU
#define REG_ROI_CONFIG__USER_ROI_REQUESTED_GLOBAL_XY_SIZE       0x0080U

//funciones i2c
void VL53L1X_WriteReg( uint16_t reg, uint8_t value)
{
    uint8_t buf[3] = {
        (uint8_t)(reg >> 8),
        (uint8_t)(reg),
        value
    };
    LIDAR.last_status = (uint8_t)HAL_I2C_Master_Transmit(
        LIDAR.hi2c, LIDAR.address, buf, 3, VL53L1X_I2C_TIMEOUT_MS);
}

void VL53L1X_WriteReg16( uint16_t reg, uint16_t value)
{
    uint8_t buf[4] = {
        (uint8_t)(reg >> 8),
        (uint8_t)(reg),
        (uint8_t)(value >> 8),
        (uint8_t)(value)
    };
    LIDAR.last_status = (uint8_t)HAL_I2C_Master_Transmit(
        LIDAR.hi2c, LIDAR.address, buf, 4, VL53L1X_I2C_TIMEOUT_MS);
}

void VL53L1X_WriteReg32( uint16_t reg, uint32_t value)
{
    uint8_t buf[6] = {
        (uint8_t)(reg >> 8),
        (uint8_t)(reg),
        (uint8_t)(value >> 24),
        (uint8_t)(value >> 16),
        (uint8_t)(value >>  8),
        (uint8_t)(value)
    };
    LIDAR.last_status = (uint8_t)HAL_I2C_Master_Transmit(
        LIDAR.hi2c, LIDAR.address, buf, 6, VL53L1X_I2C_TIMEOUT_MS);
}
uint8_t VL53L1X_ReadReg( uint16_t reg)
{
    uint8_t value = 0;
    LIDAR.last_status = (uint8_t)HAL_I2C_Mem_Read(
        LIDAR.hi2c,
        LIDAR.address,
        reg,
        I2C_MEMADD_SIZE_16BIT,
        &value, 1,
        VL53L1X_I2C_TIMEOUT_MS);
    return value;
}

uint16_t VL53L1X_ReadReg16( uint16_t reg)
{
    uint8_t buf[2] = {0, 0};
    LIDAR.last_status = (uint8_t)HAL_I2C_Mem_Read(
        LIDAR.hi2c,
        LIDAR.address,
        reg,
        I2C_MEMADD_SIZE_16BIT,
        buf, 2,
        VL53L1X_I2C_TIMEOUT_MS);
    return ((uint16_t)buf[0] << 8) | buf[1];
}

uint32_t VL53L1X_ReadReg32( uint16_t reg)
{
    uint8_t buf[4] = {0, 0, 0, 0};
    LIDAR.last_status = (uint8_t)HAL_I2C_Mem_Read(
        LIDAR.hi2c,
        LIDAR.address,
        reg,
        I2C_MEMADD_SIZE_16BIT,
        buf, 4,
        VL53L1X_I2C_TIMEOUT_MS);
    return ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) |
           ((uint32_t)buf[2] <<  8) |  (uint32_t)buf[3];
}

/* ═══════════════════════════════════════════════════════════════
 * Helpers internos de tiempo
 * ═══════════════════════════════════════════════════════════════ */

static void startTimeout()
{
    LIDAR.timeout_start_ms = HAL_GetTick();
}

static bool checkTimeoutExpired( )
{
    return (LIDAR.io_timeout > 0) &&
           ((uint32_t)(HAL_GetTick() - LIDAR.timeout_start_ms) > LIDAR.io_timeout);
}

/* ═══════════════════════════════════════════════════════════════
 * Funciones de cálculo internas (sin acceso I2C)
 * ═══════════════════════════════════════════════════════════════ */

static uint32_t decodeTimeout(uint16_t reg_val)
{
    return ((uint32_t)(reg_val & 0xFF) << (reg_val >> 8)) + 1;
}

static uint16_t encodeTimeout(uint32_t timeout_mclks)
{
    if (timeout_mclks == 0) { return 0; }

    uint32_t ls_byte = timeout_mclks - 1;
    uint16_t ms_byte = 0;

    while ((ls_byte & 0xFFFFFF00U) > 0) {
        ls_byte >>= 1;
        ms_byte++;
    }
    return (ms_byte << 8) | (ls_byte & 0xFF);
}

static uint32_t timeoutMclksToMicroseconds(uint32_t timeout_mclks, uint32_t macro_period_us)
{
    return ((uint64_t)timeout_mclks * macro_period_us + 0x800) >> 12;
}

static uint32_t timeoutMicrosecondsToMclks(uint32_t timeout_us, uint32_t macro_period_us)
{
    return (((uint32_t)timeout_us << 12) + (macro_period_us >> 1)) / macro_period_us;
}
static uint32_t calcMacroPeriod(const  uint8_t vcsel_period)
{
    uint32_t pll_period_us      = ((uint32_t)0x01 << 30) / LIDAR.fast_osc_frequency;
    uint8_t  vcsel_period_pclks = (vcsel_period + 1) << 1;
    uint32_t macro_period_us    = (uint32_t)2304 * pll_period_us;
    macro_period_us >>= 6;
    macro_period_us  *= vcsel_period_pclks;
    macro_period_us >>= 6;
    return macro_period_us;
}
static float countRateFixedToFloat(uint16_t count_rate_fixed)
{
    return (float)count_rate_fixed / (float)(1 << 7);
}

/* ═══════════════════════════════════════════════════════════════
 * Lectura masiva de resultados (equivale a readResults())
 * ═══════════════════════════════════════════════════════════════ */
static void readResults()
{
    uint8_t buf[17] = {0};

    LIDAR.last_status = (uint8_t)HAL_I2C_Mem_Read(
        LIDAR.hi2c,
        LIDAR.address,
        REG_RESULT__RANGE_STATUS,
        I2C_MEMADD_SIZE_16BIT,
        buf, 17,
        VL53L1X_I2C_TIMEOUT_MS);

    if (LIDAR.last_status != HAL_OK) { return; }

    LIDAR.results.range_status  = buf[0];
    /* buf[1] = report_status, ignorado */
    LIDAR.results.stream_count  = buf[2];

    LIDAR.results.dss_actual_effective_spads_sd0  = (uint16_t)buf[3] << 8;
    LIDAR.results.dss_actual_effective_spads_sd0 |= buf[4];

    /* buf[5..6] = peak_signal_count_rate_mcps_sd0, ignorado */

    LIDAR.results.ambient_count_rate_mcps_sd0  = (uint16_t)buf[7] << 8;
    LIDAR.results.ambient_count_rate_mcps_sd0 |= buf[8];

    /* buf[9..10]  = sigma_sd0, ignorado */
    /* buf[11..12] = phase_sd0, ignorado */

    LIDAR.results.final_crosstalk_corrected_range_mm_sd0  = (uint16_t)buf[13] << 8;
    LIDAR.results.final_crosstalk_corrected_range_mm_sd0 |= buf[14];

    LIDAR.results.peak_signal_count_rate_crosstalk_corrected_mcps_sd0  = (uint16_t)buf[15] << 8;
    LIDAR.results.peak_signal_count_rate_crosstalk_corrected_mcps_sd0 |= buf[16];
}

/* ═══════════════════════════════════════════════════════════════
 * Dynamic SPAD Selection update
 * ═══════════════════════════════════════════════════════════════ */
static void updateDSS()
{
    uint16_t spadCount = LIDAR.results.dss_actual_effective_spads_sd0;

    if (spadCount != 0) {
        uint32_t totalRatePerSpad =
            (uint32_t)LIDAR.results.peak_signal_count_rate_crosstalk_corrected_mcps_sd0 +
            LIDAR.results.ambient_count_rate_mcps_sd0;

        if (totalRatePerSpad > 0xFFFF) { totalRatePerSpad = 0xFFFF; }
        totalRatePerSpad <<= 16;
        totalRatePerSpad  /= spadCount;

        if (totalRatePerSpad != 0) {
            uint32_t requiredSpads =
                ((uint32_t)VL53L1X_TARGET_RATE << 16) / totalRatePerSpad;
            if (requiredSpads > 0xFFFF) { requiredSpads = 0xFFFF; }
            VL53L1X_WriteReg16( REG_DSS_CONFIG__MANUAL_EFFECTIVE_SPADS_SELECT,
                               (uint16_t)requiredSpads);
            return;
        }
    }
    /* fallback: punto medio */
    VL53L1X_WriteReg16( REG_DSS_CONFIG__MANUAL_EFFECTIVE_SPADS_SELECT, 0x8000);
}

// convierte el buffer en ranging_data
static void getRangingData()
{
    uint16_t range = LIDAR.results.final_crosstalk_corrected_range_mm_sd0;
    LIDAR.ranging_data.range_mm =
        (uint16_t)(((uint32_t)range * 2011 + 0x0400) / 0x0800);

    switch (LIDAR.results.range_status) {
        case 17: case 2: case 1: case 3:
            LIDAR.ranging_data.range_status = VL53L1X_HARDWARE_FAIL;          break;
        case 13:
            LIDAR.ranging_data.range_status = VL53L1X_MIN_RANGE_FAIL;         break;
        case 18:
            LIDAR.ranging_data.range_status = VL53L1X_SYNCHRONIZATION_INT;    break;
        case 5:
            LIDAR.ranging_data.range_status = VL53L1X_OUT_OF_BOUNDS_FAIL;     break;
        case 4:
            LIDAR.ranging_data.range_status = VL53L1X_SIGNAL_FAIL;            break;
        case 6:
            LIDAR.ranging_data.range_status = VL53L1X_SIGMA_FAIL;             break;
        case 7:
            LIDAR.ranging_data.range_status = VL53L1X_WRAP_TARGET_FAIL;       break;
        case 12:
            LIDAR.ranging_data.range_status = VL53L1X_XTALK_SIGNAL_FAIL;      break;
        case 8:
            LIDAR.ranging_data.range_status = VL53L1X_RANGE_VALID_MIN_RANGE_CLIPPED; break;
        case 9:
            LIDAR.ranging_data.range_status =
                (LIDAR.results.stream_count == 0)
                ? VL53L1X_RANGE_VALID_NO_WRAP_CHECK_FAIL
                : VL53L1X_RANGE_VALID;
            break;
        default:
            LIDAR.ranging_data.range_status = VL53L1X_NONE;
    }

    LIDAR.ranging_data.peak_signal_count_rate_MCPS =
        countRateFixedToFloat(
            LIDAR.results.peak_signal_count_rate_crosstalk_corrected_mcps_sd0);
    LIDAR.ranging_data.ambient_count_rate_MCPS =
        countRateFixedToFloat(LIDAR.results.ambient_count_rate_mcps_sd0);
}

/* ═══════════════════════════════════════════════════════════════
 * setupManualCalibration
 * ═══════════════════════════════════════════════════════════════ */
static void setupManualCalibration()
{
    LIDAR.saved_vhv_init    = VL53L1X_ReadReg( REG_VHV_CONFIG__INIT);
    LIDAR.saved_vhv_timeout = VL53L1X_ReadReg( REG_VHV_CONFIG__TIMEOUT_MACROP_LOOP_BOUND);

    VL53L1X_WriteReg( REG_VHV_CONFIG__INIT,
                     LIDAR.saved_vhv_init & 0x7F);
    VL53L1X_WriteReg( REG_VHV_CONFIG__TIMEOUT_MACROP_LOOP_BOUND,
                     (LIDAR.saved_vhv_timeout & 0x03) | (3 << 2));
    VL53L1X_WriteReg( REG_PHASECAL_CONFIG__OVERRIDE, 0x01);
    VL53L1X_WriteReg( REG_CAL_CONFIG__VCSEL_START,
                     VL53L1X_ReadReg( REG_PHASECAL_RESULT__VCSEL_START));
}

bool VL53L1X_Init( )
{
	 bool io_2v8=true;

    /* Inicializa el descriptor */
    LIDAR.hi2c              = &LIDARI2C;
    LIDAR.address           = VL53L1X_ADDRESS ;
    LIDAR.io_timeout        = 0;
    LIDAR.did_timeout       = false;
    LIDAR.calibrated        = false;
    LIDAR.saved_vhv_init    = 0;
    LIDAR.saved_vhv_timeout = 0;
    LIDAR.distance_mode     = VL53L1X_DISTANCE_UNKNOWN;
    LIDAR.last_status       = 0;
    LIDAR.timeout_start_ms  = 0;

    /* Verifica modelo del sensor */
    if (VL53L1X_ReadReg16( REG_IDENTIFICATION__MODEL_ID) != 0xEACC) {
        return false;
    }

    /* Reset software */
    VL53L1X_WriteReg( REG_SOFT_RESET, 0x00);
    HAL_Delay(1);
    VL53L1X_WriteReg( REG_SOFT_RESET, 0x01);
    HAL_Delay(50);

    /* Espera boot con timeout de 500 ms */
    LIDAR.io_timeout = 500;
    startTimeout();
    while ((VL53L1X_ReadReg( REG_FIRMWARE__SYSTEM_STATUS) & 0x01) == 0
           || LIDAR.last_status != 0)
    {
        if (checkTimeoutExpired()) {
            LIDAR.did_timeout = true;
            LIDAR.io_timeout  = 0;
            return false;
        }
    }
    LIDAR.io_timeout = 0;  /* restaurar: sin timeout por defecto */

    /* Modo I/O */
    if (io_2v8) {
        VL53L1X_WriteReg( REG_PAD_I2C_HV__EXTSUP_CONFIG,
            VL53L1X_ReadReg( REG_PAD_I2C_HV__EXTSUP_CONFIG) | 0x01);
    }

    /* Guarda oscilador */
    LIDAR.fast_osc_frequency = VL53L1X_ReadReg16( REG_OSC_MEASURED__FAST_OSC__FREQUENCY);
    LIDAR.osc_calibrate_val  = VL53L1X_ReadReg16( REG_RESULT__OSC_CALIBRATE_VAL);

    /* Configuración estática */
    VL53L1X_WriteReg16( REG_DSS_CONFIG__TARGET_TOTAL_RATE_MCPS,           VL53L1X_TARGET_RATE);
    VL53L1X_WriteReg( REG_GPIO__TIO_HV_STATUS,                          0x02);
    VL53L1X_WriteReg(   REG_SIGMA_ESTIMATOR__EFFECTIVE_PULSE_WIDTH_NS,    8);
    VL53L1X_WriteReg(   REG_SIGMA_ESTIMATOR__EFFECTIVE_AMBIENT_WIDTH_NS,  16);
    VL53L1X_WriteReg(   REG_ALGO__CROSSTALK_COMPENSATION_VALID_HEIGHT_MM, 0x01);
    VL53L1X_WriteReg(   REG_ALGO__RANGE_IGNORE_VALID_HEIGHT_MM,           0xFF);
    VL53L1X_WriteReg(   REG_ALGO__RANGE_MIN_CLIP,                         0);
    VL53L1X_WriteReg(   REG_ALGO__CONSISTENCY_CHECK__TOLERANCE,           2);

    /* Configuración general */
    VL53L1X_WriteReg16( REG_SYSTEM__THRESH_RATE_HIGH,          0x0000);
    VL53L1X_WriteReg16( REG_SYSTEM__THRESH_RATE_LOW,           0x0000);
    VL53L1X_WriteReg(   REG_DSS_CONFIG__APERTURE_ATTENUATION,  0x38);

    /* Timing config */
    VL53L1X_WriteReg16( REG_RANGE_CONFIG__SIGMA_THRESH,                   360);
    VL53L1X_WriteReg16( REG_RANGE_CONFIG__MIN_COUNT_RATE_RTN_LIMIT_MCPS,  192);

    /* Dynamic config */
    VL53L1X_WriteReg( REG_SYSTEM__GROUPED_PARAMETER_HOLD_0,  0x01);
    VL53L1X_WriteReg( REG_SYSTEM__GROUPED_PARAMETER_HOLD_1,  0x01);
    VL53L1X_WriteReg( REG_SD_CONFIG__QUANTIFIER,             2);
    VL53L1X_WriteReg( REG_SYSTEM__GROUPED_PARAMETER_HOLD,    0x00);
    VL53L1X_WriteReg( REG_SYSTEM__SEED_CONFIG,               1);

    /* Low power auto */
    VL53L1X_WriteReg(   REG_SYSTEM__SEQUENCE_CONFIG,                   0x8B);
    VL53L1X_WriteReg16( REG_DSS_CONFIG__MANUAL_EFFECTIVE_SPADS_SELECT, 200 << 8);
    VL53L1X_WriteReg(   REG_DSS_CONFIG__ROI_MODE_CONTROL,              2);

    /* Defaults: long range, 50 ms */
    VL53L1X_SetDistanceMode( VL53L1X_LONG);
    VL53L1X_SetMeasurementTimingBudget( 50000);

    /* Offset part-to-part */
    VL53L1X_WriteReg16( REG_ALGO__PART_TO_PART_RANGE_OFFSET_MM,
        VL53L1X_ReadReg16( REG_MM_CONFIG__OUTER_OFFSET_MM) * 4);

    return true;
}

void VL53L1X_SetAddress( uint8_t new_addr)
{
    /* El sensor espera la dirección 7 bits en el registro */
    VL53L1X_WriteReg( REG_I2C_SLAVE__DEVICE_ADDRESS, (new_addr >> 1) & 0x7F);
    LIDAR.address = new_addr;
}

bool VL53L1X_SetDistanceMode( VL53L1X_DistanceMode_t mode)
{
    uint32_t budget_us = VL53L1X_GetMeasurementTimingBudget();

    switch (mode) {
        case VL53L1X_SHORT:
            VL53L1X_WriteReg( REG_RANGE_CONFIG__VCSEL_PERIOD_A,  0x07);
            VL53L1X_WriteReg( REG_RANGE_CONFIG__VCSEL_PERIOD_B,  0x05);
            VL53L1X_WriteReg( REG_RANGE_CONFIG__VALID_PHASE_HIGH, 0x38);
            VL53L1X_WriteReg( REG_SD_CONFIG__WOI_SD0,             0x07);
            VL53L1X_WriteReg( REG_SD_CONFIG__WOI_SD1,             0x05);
            VL53L1X_WriteReg( REG_SD_CONFIG__INITIAL_PHASE_SD0,   6);
            VL53L1X_WriteReg( REG_SD_CONFIG__INITIAL_PHASE_SD1,   6);
            break;

        case VL53L1X_MEDIUM:
            VL53L1X_WriteReg( REG_RANGE_CONFIG__VCSEL_PERIOD_A,  0x0B);
            VL53L1X_WriteReg( REG_RANGE_CONFIG__VCSEL_PERIOD_B,  0x09);
            VL53L1X_WriteReg( REG_RANGE_CONFIG__VALID_PHASE_HIGH, 0x78);
            VL53L1X_WriteReg( REG_SD_CONFIG__WOI_SD0,             0x0B);
            VL53L1X_WriteReg( REG_SD_CONFIG__WOI_SD1,             0x09);
            VL53L1X_WriteReg( REG_SD_CONFIG__INITIAL_PHASE_SD0,   10);
            VL53L1X_WriteReg( REG_SD_CONFIG__INITIAL_PHASE_SD1,   10);
            break;

        case VL53L1X_LONG:
            VL53L1X_WriteReg( REG_RANGE_CONFIG__VCSEL_PERIOD_A,  0x0F);
            VL53L1X_WriteReg( REG_RANGE_CONFIG__VCSEL_PERIOD_B,  0x0D);
            VL53L1X_WriteReg( REG_RANGE_CONFIG__VALID_PHASE_HIGH, 0xB8);
            VL53L1X_WriteReg( REG_SD_CONFIG__WOI_SD0,             0x0F);
            VL53L1X_WriteReg( REG_SD_CONFIG__WOI_SD1,             0x0D);
            VL53L1X_WriteReg( REG_SD_CONFIG__INITIAL_PHASE_SD0,   14);
            VL53L1X_WriteReg( REG_SD_CONFIG__INITIAL_PHASE_SD1,   14);
            break;

        default:
            return false;
    }

    VL53L1X_SetMeasurementTimingBudget( budget_us);
    LIDAR.distance_mode = mode;
    LIDAR.calibrated = false;
    return true;
}

VL53L1X_DistanceMode_t VL53L1X_GetDistanceMode( )
{
    return LIDAR.distance_mode;
}

bool VL53L1X_SetMeasurementTimingBudget( uint32_t budget_us)
{
    if (budget_us <= VL53L1X_TIMING_GUARD) { return false; }

    uint32_t range_config_timeout_us = budget_us - VL53L1X_TIMING_GUARD;
    if (range_config_timeout_us > 1100000UL) { return false; }

    range_config_timeout_us /= 2;

    uint32_t macro_period_us;

    macro_period_us = calcMacroPeriod(
        VL53L1X_ReadReg( REG_RANGE_CONFIG__VCSEL_PERIOD_A));

    uint32_t phasecal_timeout_mclks =
        timeoutMicrosecondsToMclks(1000, macro_period_us);
    if (phasecal_timeout_mclks > 0xFF) { phasecal_timeout_mclks = 0xFF; }
    VL53L1X_WriteReg( REG_PHASECAL_CONFIG__TIMEOUT_MACROP,
                     (uint8_t)phasecal_timeout_mclks);

    VL53L1X_WriteReg16( REG_MM_CONFIG__TIMEOUT_MACROP_A,
        encodeTimeout(timeoutMicrosecondsToMclks(1, macro_period_us)));

    VL53L1X_WriteReg16( REG_RANGE_CONFIG__TIMEOUT_MACROP_A,
        encodeTimeout(timeoutMicrosecondsToMclks(range_config_timeout_us, macro_period_us)));

    macro_period_us = calcMacroPeriod(
        VL53L1X_ReadReg( REG_RANGE_CONFIG__VCSEL_PERIOD_B));

    VL53L1X_WriteReg16( REG_MM_CONFIG__TIMEOUT_MACROP_B,
        encodeTimeout(timeoutMicrosecondsToMclks(1, macro_period_us)));

    VL53L1X_WriteReg16( REG_RANGE_CONFIG__TIMEOUT_MACROP_B,
        encodeTimeout(timeoutMicrosecondsToMclks(range_config_timeout_us, macro_period_us)));

    return true;
}

uint32_t VL53L1X_GetMeasurementTimingBudget()
{
    uint32_t macro_period_us = calcMacroPeriod(
        VL53L1X_ReadReg( REG_RANGE_CONFIG__VCSEL_PERIOD_A));

    uint32_t range_config_timeout_us = timeoutMclksToMicroseconds(
        decodeTimeout(VL53L1X_ReadReg16( REG_RANGE_CONFIG__TIMEOUT_MACROP_A)),
        macro_period_us);

    return 2 * range_config_timeout_us + VL53L1X_TIMING_GUARD;
}

void VL53L1X_SetROISize( uint8_t width, uint8_t height)
{
    if (width  > 16) { width  = 16; }
    if (height > 16) { height = 16; }

    if (width > 10 || height > 10) {
        VL53L1X_WriteReg( REG_ROI_CONFIG__USER_ROI_CENTRE_SPAD, 199);
    }
    VL53L1X_WriteReg( REG_ROI_CONFIG__USER_ROI_REQUESTED_GLOBAL_XY_SIZE,
                     (uint8_t)(((height - 1) << 4) | (width - 1)));
}

void VL53L1X_GetROISize( uint8_t *width, uint8_t *height)
{
    uint8_t reg_val = VL53L1X_ReadReg( REG_ROI_CONFIG__USER_ROI_REQUESTED_GLOBAL_XY_SIZE);
    *width  = (reg_val & 0x0F) + 1;
    *height = (reg_val >> 4)   + 1;
}

void VL53L1X_SetROICenter( uint8_t spadNumber)
{
    VL53L1X_WriteReg( REG_ROI_CONFIG__USER_ROI_CENTRE_SPAD, spadNumber);
}

uint8_t VL53L1X_GetROICenter()
{
    return VL53L1X_ReadReg( REG_ROI_CONFIG__USER_ROI_CENTRE_SPAD);
}

void VL53L1X_StartContinuous( uint32_t period_ms)
{
    VL53L1X_WriteReg32( REG_SYSTEM__INTERMEASUREMENT_PERIOD,
                       period_ms * LIDAR.osc_calibrate_val);
    VL53L1X_WriteReg( REG_SYSTEM__INTERRUPT_CLEAR, 0x01);
    VL53L1X_WriteReg( REG_SYSTEM__MODE_START,      0x40);
}

void VL53L1X_StopContinuous()
{
    VL53L1X_WriteReg( REG_SYSTEM__MODE_START, 0x80);

    LIDAR.calibrated = false;

    if (LIDAR.saved_vhv_init != 0) {
        VL53L1X_WriteReg( REG_VHV_CONFIG__INIT, LIDAR.saved_vhv_init);
    }
    if (LIDAR.saved_vhv_timeout != 0) {
        VL53L1X_WriteReg( REG_VHV_CONFIG__TIMEOUT_MACROP_LOOP_BOUND,
                         LIDAR.saved_vhv_timeout);
    }
    VL53L1X_WriteReg( REG_PHASECAL_CONFIG__OVERRIDE, 0x00);
}

bool VL53L1X_DataReady()
{
    return (VL53L1X_ReadReg( REG_GPIO__TIO_HV_STATUS) & 0x01) == 0;
}

uint16_t VL53L1X_Read( )
{
	bool blocking=true;
    if (blocking) {
        startTimeout();
        while (!VL53L1X_DataReady()) {
            if (checkTimeoutExpired()){
                LIDAR.did_timeout = true;
                return 0;
            }
        }
    }

    readResults();

    if (!LIDAR.calibrated) {
        setupManualCalibration();
        LIDAR.calibrated = true;
    }

    updateDSS();
    getRangingData();

    VL53L1X_WriteReg( REG_SYSTEM__INTERRUPT_CLEAR, 0x01);
    return LIDAR.ranging_data.range_mm;
}

uint16_t VL53L1X_ReadSingle( bool blocking)
{
    VL53L1X_WriteReg( REG_SYSTEM__INTERRUPT_CLEAR, 0x01);
    VL53L1X_WriteReg( REG_SYSTEM__MODE_START,      0x10);

    if (blocking) {
        return VL53L1X_Read( true);
    }
    return 0;
}

void VL53L1X_SetTimeout( uint16_t timeout_ms)
{
    LIDAR.io_timeout = timeout_ms;
}

uint16_t VL53L1X_GetTimeout()
{
    return LIDAR.io_timeout;
}

bool VL53L1X_TimeoutOccurred()
{
    bool tmp = LIDAR.did_timeout;
    LIDAR.did_timeout = false;
    return tmp;
}

const char *VL53L1X_RangeStatusToString(VL53L1X_RangeStatus_t status)
{
    switch (status) {
        case VL53L1X_RANGE_VALID:                    return "range valid";
        case VL53L1X_SIGMA_FAIL:                     return "sigma fail";
        case VL53L1X_SIGNAL_FAIL:                    return "signal fail";
        case VL53L1X_RANGE_VALID_MIN_RANGE_CLIPPED:  return "range valid, min range clipped";
        case VL53L1X_OUT_OF_BOUNDS_FAIL:             return "out of bounds fail";
        case VL53L1X_HARDWARE_FAIL:                  return "hardware fail";
        case VL53L1X_RANGE_VALID_NO_WRAP_CHECK_FAIL: return "range valid, no wrap check fail";
        case VL53L1X_WRAP_TARGET_FAIL:               return "wrap target fail";
        case VL53L1X_XTALK_SIGNAL_FAIL:              return "xtalk signal fail";
        case VL53L1X_SYNCHRONIZATION_INT:            return "synchronization int";
        case VL53L1X_MIN_RANGE_FAIL:                 return "min range fail";
        case VL53L1X_NONE:                           return "no update";
        default:                                     return "unknown status";
    }
}
