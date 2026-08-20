/*
 * uart.c
 *
 *  Created on: 15/11/2021
 *      Author: Alcides Ramos
 */

#include "uart_H2.h"

//define los uart usados
#ifdef HAL2
#ifdef uart1_
hal_uart_handle_t *huart1;
#endif
#ifdef uart2_
hal_uart_handle_t *huart2;
#endif
#ifdef uart3
hal_uart_handle_t *huart3;
#endif
#ifdef uart4_
hal_uart_handle_t *huart4;
#endif
#ifdef uart5_
hal_uart_handle_t *huart5;
#endif
#ifdef uart6_
hal_uart_handle_t *huart6;
#endif
#ifdef uart7_
hal_uart_handle_t *huart7;
#endif
#ifdef uart8_
hal_uart_handle_t *huart8;
#endif
#ifdef uart9_
hal_uart_handle_t *huart9;
#endif
#ifdef uart10_
hal_uart_handle_t *huart10;
#endif

#endif

#ifdef HAL2
//inicia los uartt deseado
#ifdef uart1_
void uart1_init()
{
 huart1 = mx_usart1_uart_init();
}
#endif

#ifdef uart2_
void uart2_init()
{
 huart2 = mx_usart2_uart_init();
}
#endif

#ifdef uart3_
void uart3_init()
{
 huart3 = mx_usart3_uart_init();
}
#endif

#ifdef uart4_
void uart4_init()
{
 huart4 = mx_usart4_uart_init();
}
#endif

#ifdef uart5_
void uart5_init()
{
 huart5 = mx_usart5_uart_init();
}
#endif

#ifdef uart6_
void uart6_init()
{
 huart6 = mx_usart6_uart_init();
}
#endif

#ifdef uart7_
void uart7_init()
{
 huart7 = mx_usart7_uart_init();
}
#endif
#ifdef uart8_
void uart8_init()
{
 huart8 = mx_usart8_uart_init();
}
#endif
#ifdef uart9_
void uart9_init()
{
 huart9 = mx_usart9_uart_init();
}
#endif
#ifdef uart10_
void uart10_init()
{
 huart10 = mx_usart10_uart_init();
}
#endif

#endif



void uartx_write(UART_HandleTypeDef *huart,uint8_t ch)
{
HAL_UART_Transmit(huart, &ch, 1, 0xffff);

}

void uartx_write_text(UART_HandleTypeDef *huart,char *info)
{

while(*info)  uartx_write(huart,*info++);

}


char uartx_read(UART_HandleTypeDef *huart)
{
	uint8_t dato_rx_=0;
	HAL_UART_Receive(huart,&dato_rx_, 1,HAL_MAX_DELAY);
	return(dato_rx_);
}



void uartx_read_text(UART_HandleTypeDef *huart,char  *info,const char final,char cuanto)
{
char dato_rx=0;//  los datos leidos son de 8 bits
char i=0;
    memset(info,0,cuanto);//limpia el buffer
	while(dato_rx!=final)// lee hasta que llegue el final
	{
	dato_rx =uartx_read(huart);
	*info=dato_rx;//  va almacenado en el buffer
    info++;
	i++;// incrementa contador
	if (i>cuanto-1) break;// si llegan n elementosa sale
	}
}



