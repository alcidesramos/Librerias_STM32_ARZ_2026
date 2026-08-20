/*
 * uart_int_idle.h
 *
 *  Created on: Dec 25, 2023
 *      Author: Alcides Ramos
 */
#include "main.h"

#ifndef LIBRERIAS_UART_INT_IDLE_H_
#define LIBRERIAS_UART_INT_IDLE_H_

#define uartdatosreg USART2   //define puerto serial a analizar
#define uartdatos huart2
# define trama_size 100  //coloque el tamaño del buffer
extern UART_HandleTypeDef uartdatos;

void uartdat_it_idle_init();


#endif /* LIBRERIAS_UART_INT_IDLE_H_ */
