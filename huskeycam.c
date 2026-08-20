/*
 * huskeycam.c
 *
 *  Created on: Dec 9, 2023
 *      Author: alcid
 */

#ifndef LIBRERIAS_HUSKEYCAM_C_
#define LIBRERIAS_HUSKEYCAM_C_

#include "huskeycam.h"
#include "uart.h"
extern UART_HandleTypeDef uart_huskey;

const  uint8_t  bloque[]={0x55,0xAA,0x11,0x00,0x21,0x31};


#define huskey_buff 100 //tamaño del buffer

uint8_t trama_huskey[huskey_buff];
extern volatile char flag_rx;

void Huskey_init()
{
     memset(trama_huskey,0,huskey_buff);//limpia trama de recepcion por idle
	  huskeycam->SR &= ~(USART_SR_RXNE | USART_SR_ORE);
	// Limpiar la bandera RXNE leyendo el registro DR
	uint8_t dummy = huskeycam->DR;
    HAL_UARTEx_ReceiveToIdle_DMA(&uart_huskey, (uint8_t*)trama_huskey, huskey_buff);//inica la recepcion por idle

}


void Huskeylen_Block()
{
 //envia el comando para consultar bloques;
	for (uint8_t huscont=0;huscont<6;huscont++) uartx_write(&uart_huskey, bloque[huscont]);
}


#endif /* LIBRERIAS_HUSKEYCAM_C_ */
