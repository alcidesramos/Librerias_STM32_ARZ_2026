/*
 * lidarvl53.c
 *
 *  Created on: Jul 20, 2023
 *      Author: alcid
 */


#include "lidarvl53.h"

VL53L0X_RangingMeasurementData_t RangingData;
VL53L0X_Dev_t  vl53l0x_c; // center module
VL53L0X_DEV    Dev = &vl53l0x_c;
uint32_t refSpadCount;
  uint8_t isApertureSpads;
  uint8_t VhvSettings;
  uint8_t PhaseCal;
static uint16_t ultima_medida_fast_mm = lidar_max_mm;



void lidar_init(uint8_t dir)
{
	  Dev->I2cHandle = & LIDAR_I2C;//el puerto i2c a usar
	  Dev->I2cDevAddr = dir;
      Dev->comms_type=1;
      Dev->comms_speed_khz=400;//  i2c a 400khz

      VL53L0X_WaitDeviceBooted( Dev );
	  VL53L0X_DataInit( Dev );
	  VL53L0X_StaticInit( Dev );
	  VL53L0X_PerformRefSpadManagement(Dev, &refSpadCount, &isApertureSpads);
	  VL53L0X_PerformRefCalibration(Dev, &VhvSettings, &PhaseCal);
	  VL53L0X_SetDeviceMode(Dev, VL53L0X_DEVICEMODE_SINGLE_RANGING);
	  // Enable/Disable Sigma and Signal check
	  VL53L0X_SetLimitCheckEnable(Dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);
	  VL53L0X_SetLimitCheckEnable(Dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);
	  VL53L0X_SetLimitCheckValue(Dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, (FixPoint1616_t)(0.1*65536));
	  VL53L0X_SetLimitCheckValue(Dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, (FixPoint1616_t)(60*65536));
	  // Perfil normal: prioriza alcance hasta 2 m en buenas condiciones.
	  VL53L0X_SetVcselPulsePeriod(Dev, VL53L0X_VCSEL_PERIOD_PRE_RANGE, 18);
	  VL53L0X_SetVcselPulsePeriod(Dev, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, 14);
	  VL53L0X_SetMeasurementTimingBudgetMicroSeconds(Dev, 200000);

}
void lidar_init_fast(uint8_t dir)
{
    Dev->I2cHandle = &LIDAR_I2C;       // Puerto I2C a usar
    Dev->I2cDevAddr = dir;             // Dirección del sensor
    Dev->comms_type = 1;
    Dev->comms_speed_khz = 400;        // I2C a 400kHz

    VL53L0X_WaitDeviceBooted(Dev);
    VL53L0X_DataInit(Dev);
    VL53L0X_StaticInit(Dev);
    VL53L0X_PerformRefSpadManagement(Dev, &refSpadCount, &isApertureSpads);
	VL53L0X_PerformRefCalibration(Dev, &VhvSettings, &PhaseCal);

    // === CONFIGURACIÓN DE MEDICIÓN ===
    VL53L0X_SetLimitCheckEnable(Dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);
    VL53L0X_SetLimitCheckEnable(Dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);
    VL53L0X_SetLimitCheckValue(Dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, (FixPoint1616_t)(0.1 * 65536));
    VL53L0X_SetLimitCheckValue(Dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, (FixPoint1616_t)(60 * 65536));

	// Perfil rapido con alcance util: 33 ms da una lectura cada 33 ms.
    VL53L0X_SetVcselPulsePeriod(Dev, VL53L0X_VCSEL_PERIOD_PRE_RANGE, 14);
    VL53L0X_SetVcselPulsePeriod(Dev, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, 10);
	VL53L0X_SetMeasurementTimingBudgetMicroSeconds(Dev, 33000);

	// Modo continuo temporizado para respetar el periodo configurado.
	VL53L0X_SetDeviceMode(Dev, VL53L0X_DEVICEMODE_CONTINUOUS_TIMED_RANGING);

    // Tiempo entre mediciones (igual o un poco mayor que el budget)
	VL53L0X_SetInterMeasurementPeriodMilliSeconds(Dev, 35);

    // Inicia las mediciones continuas
    VL53L0X_StartMeasurement(Dev);
}

int16_t lidar_lee_mm(uint8_t dir)
{
	VL53L0X_Error status;
	int16_t lidarmil;

	Dev->I2cDevAddr = dir;
 	status = VL53L0X_PerformSingleRangingMeasurement(Dev, &RangingData);
	if (status != VL53L0X_ERROR_NONE) return -1;
	if (RangingData.RangeStatus != 0) return lidar_max_mm;

    lidarmil = (int16_t)RangingData.RangeMilliMeter;
	if (lidarmil == 0) return lidar_max_mm;
    if (lidarmil > lidar_max_mm) lidarmil = lidar_max_mm;
 	return lidarmil;
}

int16_t lidar_lee_mm_fast(uint8_t dir)
{
	uint8_t dato_listo;
	VL53L0X_Error status;
	Dev->I2cDevAddr = dir;
	VL53L0X_RangingMeasurementData_t data;

	status = VL53L0X_GetMeasurementDataReady(Dev, &dato_listo);
	if (status != VL53L0X_ERROR_NONE) return -1;
	if (dato_listo == 0) return (int16_t)ultima_medida_fast_mm;

	status = VL53L0X_GetRangingMeasurementData(Dev, &data);
	if (status != VL53L0X_ERROR_NONE) return -1;

	if ((data.RangeStatus == 0) && (data.RangeMilliMeter > 0)) {
		ultima_medida_fast_mm = data.RangeMilliMeter;
		if (ultima_medida_fast_mm > lidar_max_mm) ultima_medida_fast_mm = lidar_max_mm;
	} else {
		ultima_medida_fast_mm = lidar_max_mm;
	}

	status = VL53L0X_ClearInterruptMask(Dev, VL53L0X_REG_SYSTEM_INTERRUPT_CLEAR);
	if (status != VL53L0X_ERROR_NONE) return -1;
	return (int16_t)ultima_medida_fast_mm;
}

float lidar_lee_cm(uint8_t dir)
{
	int16_t mm = lidar_lee_mm(dir);
	if (mm < 0) return -1.0f;
	return ((float)mm / 10.0f);
}

float lidar_lee_cm_fast(uint8_t dir)
{
	int16_t mm = lidar_lee_mm_fast(dir);
	if (mm < 0) return -1.0f;
	return ((float)mm / 10.0f);
}

VL53L0X_Error lidar_set_dir(uint8_t dir)
{
	VL53L0X_Error status = VL53L0X_SetDeviceAddress(Dev, dir);
	if (status == VL53L0X_ERROR_NONE) Dev->I2cDevAddr = dir;
	return status;

}

