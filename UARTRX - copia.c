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


//inciar uart de recepcion por interrupcion  y tamaño del buffer
UARTRXS UARTRX1 = {&huart1,USART1,100};




//se llama solo una vez para  crear el buffer trama rx del usart deseado
void uartRX_it_idle_dma_init(UARTRXS *SERIAL)
{
// Asigna memoria
SERIAL->trama_rx = malloc(SERIAL->sizeT);  //reserva memoria
 memset(SERIAL->trama_rx,0,SERIAL->sizeT-1);//limpia el  buffer
 SERIAL->flag_rx=0;// LIMPIA LA BANDERA

 //  en STM32F
 *SERIAL->SR &= ~( UART_FLAG_RXNE | UART_FLAG_ORE);
 uint8_t  dummy = *SERIAL->DR;

 // en STM32G y stmH
    // uint8_t dummy = *SERIAL->RDR;
    // *SERIAL->ICR = USART_ICR_ORECF;  // Escribir 1 en ORECF para limpiar ORE


 HAL_UARTEx_ReceiveToIdle_DMA(SERIAL->huart, (uint8_t*)SERIAL->trama_rx, SERIAL->sizeT);//inica la recepcion por idle

}

void  uartRX_DMA_STOP(UARTRXS *SERIAL)
{
	memset(SERIAL->trama_rx,0,SERIAL->sizeT-1);//limpia el  buffer
	SERIAL->flag_rx=0;// LIMPIA LA BANDERA
	HAL_UART_DMAStop(SERIAL->huart);  //para la recepcion temporarmente
}

//si ya se creo el buffer trama rx  usar solo este
void  uartRX_DMA_Re_init(UARTRXS *SERIAL)
{

	memset(SERIAL->trama_rx,0,SERIAL->sizeT-1);//limpia el  buffer

	SERIAL->flag_rx=0;// LIMPIA LA BANDERA
 //  en STM32F
 *SERIAL->SR &= ~( UART_FLAG_RXNE | UART_FLAG_ORE);
 uint8_t  dummy = *SERIAL->DR;

 // en STM32G y stmH
     // uint8_t dummy = *SERIAL->RDR;
     // *SERIAL->ICR = USART_ICR_ORECF;  // Escribir 1 en ORECF para limpiar ORE


 HAL_UARTEx_ReceiveToIdle_DMA(SERIAL->huart, (uint8_t*)SERIAL->trama_rx, SERIAL->sizeT);//inica la recepcion por idle

}
 void uartRX_INTERRUPT(UART_HandleTypeDef *huart,uint16_t sizex)
{

	//colocar una por cada usart usado
	if ((UARTRX1.flag_rx==0)&& (huart->Instance == UARTRX1.usart_instance))//si es el uart de datos
		{
		HAL_UART_DMAStop(UARTRX1.huart);  //para la recepcion temporarmente
	     UARTRX1.num_datos=sizex;
	     UARTRX1.flag_rx=1;
		 }

/*
	if ((UARTRX2.flag_rx==0)&& (huart->Instance == UARTRX2.usart_instance))//si es el uart de datos
		{
		HAL_UART_DMAStop(UARTRX2.huart);  //para la recepcion temporarmente
	     UARTRX2.num_datos=sizex;
	     UARTRX2.flag_rx=1;
		 }
*/
}

#endif /* LIBRERIAS_UARTRX1_C_ */

/*
void procesa_rx()
{
	if (strstr(UARTRX1.trama_rx,"V00=1")) PIN_OFF(LED);
	else if (strstr(UARTRX1.trama_rx,"V00=0")) PIN_ON(LED);

	else if (strstr(UARTRX1.trama_rx,"V01="))
	    {
	        strcpy(procesa, strtok(UARTRX1.trama_rx, "="));  //inicia captura de tokens desde el =
	        strcpy(procesa, strtok(0, "$"));  //captura hasta el /
	        //pasa la cadena a  numero
	       uint16_t ancho=atoi(procesa);
	       PWM_valor(&PWM1, ancho);

	    }

}

*/

