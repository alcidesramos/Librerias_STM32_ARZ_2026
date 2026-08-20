/*
 * uart.h
 *
 *  Created on: 15/11/2021
 *      Author: Alcides Ramos
 */

#ifndef LIBRERIAS_UART_H_
#define LIBRERIAS_UART_H_
#define HAL2

//definir los uart usados
#define uart1_




#include "main.h"
#include "stdio.h"
#include "string.h"

#ifdef HAL2
#define UART_HandleTypeDef hal_uart_handle_t
#ifdef uart1_
extern hal_uart_handle_t *huart1;
#endif
#ifdef uart2_
extern hal_uart_handle_t *huart2;
#endif
#ifdef uart3_
extern hal_uart_handle_t *huart3;
#endif
#ifdef uart4_
extern hal_uart_handle_t *huart4;
#endif
#ifdef uart5_
extern hal_uart_handle_t *huart5;
#endif
#ifdef uart6_
extern hal_uart_handle_t *huart6;
#endif
#ifdef uart7_
extern hal_uart_handle_t *huart7;
#endif
#ifdef uart8_
extern hal_uart_handle_t *huart8;
#endif
#ifdef uart9_
extern hal_uart_handle_t *huart9;
#endif
#ifdef uart10_
extern hal_uart_handle_t *huart10;
#endif

#endif


#ifdef __cplusplus
extern "C" {
#endif


void uartx_write(UART_HandleTypeDef *huart,uint8_t ch);
void uartx_write_text(UART_HandleTypeDef *huart, char *info);
char uartx_read(UART_HandleTypeDef *huart);
void uartx_read_text(UART_HandleTypeDef *huart,char  *info,const char final,char cuanto);

#ifdef HAL2
void uart1_init();
void uart2_init();
void uart3_init();
void uart4_init();
void uart5_init();
void uart6_init();
void uart7_init();
void uart8_init();
void uart9_init();
void uart10_init();

#endif


#ifdef __cplusplus
}
#endif


#endif /* LIBRERIAS_UART_H_ */
