/*
 * UARTRX1.C
 *
 *  Created on: Apr 9, 2025
 *      Author: ALCIDES_RAMOS
 */

#ifndef LIBRERIAS_UARTRX1_C_
#define LIBRERIAS_UARTRX1_C_
#include "UARTRX.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

//DEFINE LOS USART  A USAR
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

//inciar uart de recepcion por interrupcion  y tamaño del buffer
UARTRXS UARTRX1 = {&huart1,USART1,100};



//*********************************************************
//se llama solo una vez para  crear el buffer trama rx del usart deseado
void uartRX_it_idle_dma_init(UARTRXS *SERIAL)
{
// Asigna memoria
SERIAL->trama_rx = malloc(SERIAL->sizeT);  //reserva memoria
 memset(SERIAL->trama_rx,0,SERIAL->sizeT-1);//limpia el  buffer
 SERIAL->flag_rx=0;// LIMPIA LA BANDERA
__HAL_UART_CLEAR_OREFLAG(SERIAL->huart); // Limpia Overrun Error
__HAL_UART_FLUSH_DRREGISTER(SERIAL->huart); // Limpia buffer de entrada
 HAL_UARTEx_ReceiveToIdle_DMA(SERIAL->huart, (uint8_t*)SERIAL->trama_rx, SERIAL->sizeT);//inica la recepcion por idle
__HAL_DMA_DISABLE_IT(SERIAL->huart->hdmarx, DMA_IT_HT); // deshabilita HT, deja activo IDLE + buffer lleno

}



//si ya se creo el buffer trama rx  usar solo este
void  uartRX_DMA_Re_init(UARTRXS *SERIAL)
{

	memset(SERIAL->trama_rx,0,SERIAL->sizeT-1);//limpia el  buffer

	SERIAL->flag_rx=0;// LIMPIA LA BANDERA
__HAL_UART_CLEAR_OREFLAG(SERIAL->huart); // Limpia Overrun Error
__HAL_UART_FLUSH_DRREGISTER(SERIAL->huart); // Limpia buffer de entrada
 HAL_UARTEx_ReceiveToIdle_DMA(SERIAL->huart, (uint8_t*)SERIAL->trama_rx, SERIAL->sizeT);//inica la recepcion por idle
__HAL_DMA_DISABLE_IT(SERIAL->huart->hdmarx, DMA_IT_HT); // deshabilita HT, deja activo IDLE + buffer lleno
}

void uartRX_INTERRUPT(UART_HandleTypeDef *huart,uint16_t sizex)
{

	//colocar una por cada usart usado
	if ((UARTRX1.flag_rx==0)&& (huart->Instance == UARTRX1.usart_instance))//si es el uart de datos
		{
	     UARTRX1.num_datos=sizex;
	     UARTRX1.flag_rx=1;
		 }

//colocar si usa mas uart
		 /*
	if ((UARTRX2.flag_rx==0)&& (huart->Instance == UARTRX2.usart_instance))//si es el uart de datos
		{
	 // __HAL_UART_DISABLE_IT(huart, UART_IT_IDLE);
       HAL_UART_DMAStop(UARTRX2.huart);  //para la recepcion temporarmente
	     UARTRX2.num_datos=sizex;
	     UARTRX2.flag_rx=1;
		 }
*/
}

 void uartRX_Errores(UART_HandleTypeDef *huart)
 {
//colocar una por cada usart usado
	if (huart->Instance == UARTRX1.usart_instance)//si es el uart de datos
		{
		__HAL_UART_CLEAR_OREFLAG(UARTRX1.huart); // Limpia Overrun Error
        __HAL_UART_FLUSH_DRREGISTER(UARTRX1.huart); // Limpia buffer de entrada

		  if(UARTRX1.flag_rx==0)
		{
		HAL_UART_DMAStop(UARTRX1.huart);  //para la recepcion temporarmente
		HAL_UARTEx_ReceiveToIdle_DMA(UARTRX1.huart, (uint8_t*)UARTRX1.trama_rx, UARTRX1.sizeT);//inica la recepcion por idle
		__HAL_DMA_DISABLE_IT(UARTRX1.huart->hdmarx, DMA_IT_HT); // deshabilita HT, deja activo IDLE + buffer lleno

		}
	  }
//si usa otro usart
/*
	 	if (huart->Instance == UARTRX2.usart_instance)//si es el uart de datos
		{
		__HAL_UART_CLEAR_OREFLAG(UARTRX2.huart); // Limpia Overrun Error
        __HAL_UART_FLUSH_DRREGISTER(UARTRX2.huart); // Limpia buffer de entrada

		  if(UARTRX2.flag_rx==0)
		{
		HAL_UART_DMAStop(UARTRX2.huart);  //para la recepcion temporarmente
		HAL_UARTEx_ReceiveToIdle_DMA(UARTRX2.huart, (uint8_t*)UARTRX2.trama_rx, UARTRX2	.sizeT);//inica la recepcion por idle
		__HAL_DMA_DISABLE_IT(UARTRX2.huart->hdmarx, DMA_IT_HT); // deshabilita HT, deja activo IDLE + buffer lleno

		}
	  } 
*/

 }

#endif /* LIBRERIAS_UARTRX1_C_ */

/*
void procesa_rx()
{
//variable para procesar inf
	char procesa[100];
	if (strstr(UARTRX1.trama_rx,"V00=1")) PIN_OFF(LED);
	else if (strstr(UARTRX1.trama_rx,"V00=0")) PIN_ON(LED);

	else if (strstr(UARTRX1.trama_rx,"V01="))
	    {
	        strcpy(procesa, strtok(UARTRX1.trama_rx, "="));  //inicia captura de tokens desde el =
	        strcpy(procesa, strtok(0, "$"));  //captura hasta el /
	        //pasa la cadena a  numero
	        int16_t ancho=atoi(procesa);
	       PWM_valor(&PWM1, ancho);

	    }

}

*/

