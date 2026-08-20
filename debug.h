/*
 * debug.h
 *
 *  Created on: 24/12/2021
 *      Author: Alcides Ramos
 */

#ifndef LIBRERIAS_DEBUG_H_
#define LIBRERIAS_DEBUG_H_

#define uartx huart2   //   colocar el puerto serial a usar


#include "string.h"
#include "stdio.h"


extern UART_HandleTypeDef uartx;

//para medir tiempos en debbuger
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))


int __io_putchar(int ch)
{
 //ITM_SendChar(ch);// al debbuger
 HAL_UART_Transmit(&uartx, (uint8_t *)&ch, 1, 0xFFFF);// a un puerto uart
 return(ch);
}


#endif /* LIBRERIAS_DEBUG_H_ */
