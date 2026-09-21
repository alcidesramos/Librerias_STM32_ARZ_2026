/*
 * UART_INT_DMA_IDLE.c
 *
 *  Created on: Apr 9, 2025
 *      Author: ALCIDES_RAMOS
 */

#ifndef LIBRERIAS_UART_INT_DMA_IDLE_C_
#define LIBRERIAS_UART_INT_DMA_IDLE_C_

#include "main.h"
#include "UARTRX.h"
#include "gps.h"

//incluir librerias de  interrpcines d euart usados

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
//llamar interrupciones de usart
GPS_Interrupt(huart, Size);
	 uartRX_INTERRUPT(huart,Size);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  //Manejar errores de UART 
  GPS_Error(huart);
  uartRX_Errores(huart);
}


#endif 












/* LIBRERIAS_UART_INT_DMA_IDLE_C_ */



