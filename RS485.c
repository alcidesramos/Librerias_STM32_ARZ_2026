/*
 * RS485.c
 *
 *  Created on: Jun 25, 2024
 *      Author: Alcides Ramos
 */

#include "RS485.h"
#include "delays.h"
#include "uart.h"
extern UART_HandleTypeDef huart2;
#define RS485 huart2


//Tramas a envier por RS485
const  uint8_t VIENTO_VEL []={0x01,0x03,0x00,0x00,0x00,0x01,0x84,0x0A};
const  uint8_t VIENTO_DIR []={0x02,0x03,0x00,0x00,0x00,0x01,0x84,0x39};


void RS485_WRITE( uint8_t *info, uint8_t Num_Datos)
{
//	uartdat_it_idle_dma_init485();
	PIN_ON(E_485);//Habilita Transmision
	HAL_UART_Transmit(&RS485, info,Num_Datos, 1000);

	while (__HAL_UART_GET_FLAG(&RS485, UART_FLAG_TC) == RESET) { }
	HAL_Delay(2);// ESPERA que se estabilice el bus

	//espera termine la transmision
	// while (__HAL_UART_GET_FLAG(&RS485, UART_FLAG_TC) == RESET) ;
	PIN_OFF(E_485);//Habilita Recepcion


}



void RS485_WRITE_TEXT( uint8_t *info)
{
//	uartdat_it_idle_dma_init485();
	PIN_ON(E_485);//Habilita Transmision
	//HAL_UART_Transmit(&RS485, info,Num_Datos, 1000);

	while(*info)  uartx_write(&RS485,*info++);

	while (__HAL_UART_GET_FLAG(&RS485, UART_FLAG_TC) == RESET) { }
	HAL_Delay(2);// ESPERA que se estabilice el bus

	//espera termine la transmision
	// while (__HAL_UART_GET_FLAG(&RS485, UART_FLAG_TC) == RESET) ;
		PIN_OFF(E_485);//Habilita Recepcion


}

//OJO AL LEER  a veces agrega un cero al iniico de la trama
uint8_t RS485_READ(uint8_t *info, uint8_t Num_Datos,uint16_t wait)
{
    //LIMPIA ERRORES DE RECEPCION
//	__HAL_UART_CLEAR_FLAG(&RS485, UART_CLEAR_OREF);
	//	__HAL_UART_CLEAR_FLAG(&RS485, UART_CLEAR_RTOF);
	     uint16_t son;
		if(HAL_UARTEx_ReceiveToIdle(&RS485,info,Num_Datos,&son,wait)==HAL_OK)
			return(1);
		else return(0);
}

