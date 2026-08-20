/*
 * I2C_SCAN_BUS.h
 *
 *  Created on: 31/03/2026
 *      Author: Alcides Ramos
 */

#ifndef LIBRERIAS_I2C_SCAN_BUS_H_
#define LIBRERIAS_I2C_SCAN_BUS_H_
#include "main.h"
#include "uart.h"
//defina puerto serial usado para comunicacion
#define uart_i2c  huart1


extern UART_HandleTypeDef uart_i2c;

void I2C_SCAN(I2C_HandleTypeDef I2CMIO);

void  I2C_RESPONDE(I2C_HandleTypeDef I2CMIO,uint8_t dir);


#endif /* LIBRERIAS_I2C_SCAN_BUS_H_ */
