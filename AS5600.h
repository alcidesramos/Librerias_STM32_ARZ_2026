/*
 * AS5600.h
 *
 *  Created on: 2/04/2026
 *      Author: jjrom
 */

#ifndef LIBRERIAS_AS5600_H_
#define LIBRERIAS_AS5600_H_

#ifndef AS5600_H_
#define AS5600_H_

#include "main.h"

// Dirección I2C del sensor desplazada 1 bit a la izquierda para STM32 HAL
#define AS5600_I2C_ADDR (0x36 << 1)
#define AS5600_RAW_ANGLE_REG 0x0C

// Funciones de la librería
uint16_t AS5600_GetRawAngle(I2C_HandleTypeDef *hi2c);
float AS5600_GetDegrees(I2C_HandleTypeDef *hi2c);

#endif /* AS5600_H_ */

#endif /* LIBRERIAS_AS5600_H_ */
