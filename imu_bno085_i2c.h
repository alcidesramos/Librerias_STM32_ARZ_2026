/*
 * imu_bno085_i2c.h
 *
 * Driver minimo BNO085/BNO080 por I2C para STM32 HAL.
 * Obtiene Rotation Vector y lo convierte a Roll, Pitch y Yaw.
 */

#ifndef IMU_BNO085_I2C_H
#define IMU_BNO085_I2C_H

#include "main.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t IMU_Init(void);
uint8_t IMU_ReadEuler(float *roll, float *pitch, float *yaw);
uint8_t IMU_GetAddress7bit(void);

#ifdef __cplusplus
}
#endif

#endif /* IMU_BNO085_I2C_H */
