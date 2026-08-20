/*
 * IMU_UART.h
 *
 *  Created on: Aug 1, 2025
 *      Author: ALCIDES_RAMOS
 */

#ifndef LIBRERIAS_IMU7_UART_H_
#define LIBRERIAS_IMU7_UART_H_

#include "main.h"

#include "main.h"
#include "uart.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "UARTRX.h"

void IMU7_Init();
void IMU7_Reinit();
void  IMU7_interrup(UART_HandleTypeDef *huart,uint16_t sizex);

#endif /* LIBRERIAS_IMU7_UART_H_ */
