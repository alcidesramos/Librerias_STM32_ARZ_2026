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

extern I2C_HandleTypeDef hi2c1;


void lidar_init(uint8_t dir)
{
	  Dev->I2cHandle = &hi2c1;//el puerto i2c a usar
	  Dev->I2cDevAddr = dir;
      Dev->comms_type=1;
      Dev->comms_speed_khz=400;//  i2c a 400khz

      VL53L0X_WaitDeviceBooted( Dev );
	  VL53L0X_DataInit( Dev );
	  VL53L0X_StaticInit( Dev );
	  /* RefSpadManagement debe ejecutarse antes de RefCalibration */
	  VL53L0X_PerformRefSpadManagement(Dev, &refSpadCount, &isApertureSpads);
	  VL53L0X_PerformRefCalibration(Dev, &VhvSettings, &PhaseCal);
	  VL53L0X_SetDeviceMode(Dev, VL53L0X_DEVICEMODE_SINGLE_RANGING);
	  // Enable/Disable Sigma and Signal check
	  VL53L0X_SetLimitCheckEnable(Dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);
	  VL53L0X_SetLimitCheckEnable(Dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);
	  VL53L0X_SetLimitCheckValue(Dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, (FixPoint1616_t)(0.1*65536));
	  VL53L0X_SetLimitCheckValue(Dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, (FixPoint1616_t)(60*65536));
	  /* Los periodos VCSEL deben configurarse ANTES del timing budget */
	  VL53L0X_SetVcselPulsePeriod(Dev, VL53L0X_VCSEL_PERIOD_PRE_RANGE, 18);
	  VL53L0X_SetVcselPulsePeriod(Dev, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, 14);
	  /* 200 ms de timing budget para alcanzar ~2 m en condiciones favorables */
	  VL53L0X_SetMeasurementTimingBudgetMicroSeconds(Dev, 200000);

}

int16_t lidar_lee_mm(uint8_t dir)
{
	int16_t lidarmil;
	VL53L0X_Error status;

	Dev->I2cDevAddr = dir;
	status = VL53L0X_PerformSingleRangingMeasurement(Dev, &RangingData);

	/* Solo errores de comunicacion hacen que parezca desconectado */
	if (status != VL53L0X_ERROR_NONE)
	{
		return -1;
	}

	lidarmil = (int16_t)RangingData.RangeMilliMeter;

	/* Si el sensor reporta fuera de rango (valores altos como 8190/8191)
	   o 0 mm con error de rango, saturamos al maximo configurado (200 cm) */
	if (lidarmil > lidar_max_mm)
	{
		lidarmil = lidar_max_mm;
	}
	else if ((RangingData.RangeStatus != 0) && (lidarmil == 0))
	{
		lidarmil = lidar_max_mm;
	}
	return lidarmil;
}


float lidar_lee_cm(uint8_t dir)
{
	int16_t mm = lidar_lee_mm(dir);
	if (mm < 0) return -1.0f;
	return ((float)mm / 10.0f);
}

uint8_t lidar_set_dir(uint8_t dir)
{
	uint8_t status = VL53L0X_SetDeviceAddress(Dev, dir << 1);
	return(status);

}

