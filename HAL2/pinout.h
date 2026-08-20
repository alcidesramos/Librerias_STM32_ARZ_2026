/*
 * pinout.h
 *
 *  Created on: May 14, 2024
 *      Author: alcid
 */
#include "main.h"
#ifndef LIBRERIAS_PINOUT_H_
#define LIBRERIAS_PINOUT_H_

#define PIN_ON(pin)          HAL_GPIO_WritePin(pin##_GPIO_Port, pin##_Pin, HAL_GPIO_PIN_SET)
#define PIN_OFF(pin)         HAL_GPIO_WritePin(pin##_GPIO_Port, pin##_Pin, HAL_GPIO_PIN_RESET)
#define PIN_PUT(pin, value)  HAL_GPIO_WritePin(pin##_GPIO_Port, pin##_Pin, (value) ? HAL_GPIO_PIN_SET : HAL_GPIO_PIN_RESET)
#define PIN_BLINK(pin)       HAL_GPIO_TogglePin(pin##_GPIO_Port, pin##_Pin)
#define PIN_READ(pin)        HAL_GPIO_ReadPin(pin##_GPIO_Port, pin##_Pin)

#endif /* LIBRERIAS_PINOUT_H_ */