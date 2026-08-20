/*
 * UART1RX1.h
 *
 *  Created on: Apr 9, 2025
 *      Author: ALCIDES_RAMOS
 */

#ifndef LIBRERIAS_UARTRX_H_
#define LIBRERIAS_UARTRX_H_

#include "main.h"


typedef struct {
    UART_HandleTypeDef *huart;
    USART_TypeDef *usart_instance;  // Agregar este campo
    uint16_t sizeT;
    volatile uint32_t *DR;
    volatile uint32_t *SR;
    volatile uint32_t *RDR;
    volatile uint32_t *ICR;
    char flag_rx;
	uint16_t dato;
	uint16_t num_datos;
	uint8_t *trama_rx;
} UARTRXS;


extern UARTRXS   UARTRX1;
extern UARTRXS   UARTRX2;
extern UARTRXS   UARTRX3;
extern UARTRXS   UARTRX4;
extern UARTRXS   UARTRX5;
extern UARTRXS   UARTRX6;
extern UARTRXS   UARTRX7;
extern UARTRXS   UARTRX8;
extern UARTRXS   UARTNEXTION;
extern UARTRXS   GPS_UARTRX;

//definir en el  UART1RX1.c los uart a usar
/*
 //DEFINE LOS USART  A USAR
 extern UART_HandleTypeDef huart1;

//inciar uart de recepcion por interrupcion  y tamaño del buffer
UARTRXS UARTRX1 = {&huart1,USART1,100};

//en el procedimiento de interrupcion uartRX_INTERRUPT

 //colocar una por cada usart usado

	//colocar una por cada usart usado
	if ((UARTRX1.flag_rx==0)&& (huart->Instance == UARTRX1.usart_instance))//si es el uart de datos
		{
		HAL_UART_DMAStop(UARTRX1.huart);  //para la recepcion temporarmente
	     UARTRX1.num_datos=sizex;
	     UARTRX1.flag_rx=1;
		 }

	//si usa otro usart
	if ((UARTRX2.flag_rx==0)&& (huart->Instance == UARTRX2.usart_instance))//si es el uart de datos
			{
			HAL_UART_DMAStop(UARTRX2.huart);  //para la recepcion temporarmente
		     UARTRX2.num_datos=sizex;
		     UARTRX2.flag_rx=1;

		    }

*/
/*
// en el UART_INT_DMA_IDLE.c
//llamar interrupciones de usart
	 uartRX_INTERRUPT(huart,Size);


 */

//en el void main antes del while(1) poner
/*
 uartRX_it_idle_dma_init( &UARTRX1);
 */

//en el while poner un if por cada usart usado
/*
if (UARTRX1.flag_rx==1)
	  {
	     // uartx_write_text(&huart1, UARTRX1.trama_rx);
	      procesa_rx(); //procedimeinto donde se procesan los datos
		  uartRX_DMA_Re_init(&UARTRX1);
	  }

	  //si usa otro usart
	  if (UARTRX2.flag_rx==1)
	 	  {
	 	      PIN_BLINK(led);
	 		  uartx_write_text(&huart2, UARTRX2.trama_rx);
	 		 // UARTRX2.flag_rx=0;
	 		  uartRX_DMA_Re_init(&UARTRX2);
	 	  }

*/

void uartRX_it_idle_dma_init(UARTRXS *SERIAL);
void uartRX_DMA_Re_init(UARTRXS *SERIAL);
void  uartRX_DMA_STOP(UARTRXS *SERIAL);
void uartRX_INTERRUPT(UART_HandleTypeDef *huart,uint16_t sizex);

#endif /* LIBRERIAS_UARTRX_H_ */
