/*
 * uart.h
 *
 *  Created on: 15/11/2021
 *      Author: Alcides Ramos
 */

#ifndef LIBRERIAS_UART_H_
#define LIBRERIAS_UART_H_


#include "main.h"
#include "stdio.h"
#include "string.h"


#ifdef __cplusplus
extern "C" {
#endif


void uartx_write(UART_HandleTypeDef *huart,uint8_t ch);
void uartx_write_text(UART_HandleTypeDef *huart, char *info);
char uartx_read(UART_HandleTypeDef *huart);
void uartx_read_text(UART_HandleTypeDef *huart,char  *info,const char final,char cuanto);


#ifdef __cplusplus
}
#endif


#endif /* LIBRERIAS_UART_H_ */
