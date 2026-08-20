/*
 * IMU7_UART.c
 *
 *  Created on: Aug 1, 2025
 *      Author: ALCIDES_RAMOS
 */


#include "IMU7_UART.h"


extern UART_HandleTypeDef huart6;
//defina uart y tamaño para la nexton
UARTRXS UART_IMU= {&huart6,USART6,100};


//si se quieren leer mas o menos  texto poner tamaño
char IMU7_RX_text[200];


void IMU7_Init()
{
uartRX_it_idle_dma_init( &UART_IMU);
}

void IMU7_Reinit()
{
uartRX_DMA_Re_init(&UART_IMU);
}


void  IMU7_interrup(UART_HandleTypeDef *huart,uint16_t sizex)
{

	//colocar una por cada usart usado
	if ((UART_IMU.flag_rx==0)&& (huart->Instance == UART_IMU.usart_instance))//si es el uart de datos
		{
	//	HAL_UART_DMAStop(UART_IMU.huart);  //para la recepcion temporarmente
		UART_IMU.num_datos=sizex;
		UART_IMU.flag_rx=1;
		 }

}
