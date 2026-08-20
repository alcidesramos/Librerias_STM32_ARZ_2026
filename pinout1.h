/*
 * pinout.h
 *
 *  Created on: May 14, 2024
 *      Author: alcid
 */
#include "main.h"
#ifndef LIBRERIAS_PINOUT_H_
#define LIBRERIAS_PINOUT_H_

#define PIN_ON(pin)  pin##_GPIO_Port->BSRR=pin##_Pin
#define PIN_OFF(pin) pin##_GPIO_Port->BSRR=pin##_Pin<<16
#define PIN_PUT(pin, value) ((value) ? PIN_ON(pin) : PIN_OFF(pin))
#define PIN_BLINK(pin) HAL_GPIO_TogglePin(pin##_GPIO_Port,pin##_Pin)
#define PIN_READ(pin) HAL_GPIO_ReadPin(pin##_GPIO_Port,pin##_Pin)


#endif /* LIBRERIAS_PINOUT_H_ */
