/*
 * uart_int.h
 *
 *  Created on: Dec 25, 2023
 *      Author: alcides ramos
 */

#ifndef LIBRERIAS_UART_INT_H_
#define LIBRERIAS_UART_INT_H_

//defina que puerto serial va a usar
#define uartdatosreg USART2   //define puerto serial a analizar
#define uartdatos huart2
# define trama_size 100  //coloque el tamaño del buffer

extern UART_HandleTypeDef uartdatos;
void uartdat_it_init();

#endif /* LIBRERIAS_UART_INT_H_ */
