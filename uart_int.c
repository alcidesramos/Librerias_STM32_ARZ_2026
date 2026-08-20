/*
 * uart_int.c
 *
 *  Created on: Nov 4, 2023
 *      Author: Alcides Ramos
 */


//COLOCAR EN EL MAIN.C
//VARIABLES UART
/*
char dato_rx;
char flag_rx=0;
uint16_t contador_rx=0;
uint8_t trama_rx[trama_size];
uint8_t procesa[30];
*/


//en void main
/*
  uartdat_it_init();

*/

// en el while
/*
  if (flag_rx==1)
	  {

		  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
		//Limpia bandera
		  flag_rx=0;
	     contador_rx=0;
	    // procesarx();   //procedimiento de procesar
		 HAL_Delay(retardo);
		 uartdat_it_init();//reinicia la interrpcion serial

	  }



 */


#include "main.h"
#include "string.h"
#include "uart_int.h"

extern uint16_t contador_rx;
extern char flag_rx;
extern char dato_rx;
extern uint8_t trama_rx[];


void uartdat_it_init()
{
    memset(trama_rx,0,trama_size);//limpia el  buffer
    //el error de sobreflujo se borra leyendo sr y dr

    uartdatosreg->SR &= ~( UART_FLAG_RXNE | UART_FLAG_ORE);
	 // Limpiar las banderas RXNE y ORE de la UART
	// huart2.Instance->SR &= ~(USART_SR_RXNE | USART_SR_ORE);
    uint8_t  dummy = uartdatosreg->DR;
     HAL_UART_Receive_IT(&uartdatos, &dato_rx, 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	char dato_leido;
	if (huart->Instance == uartdatosreg)//si es int pro el uartdatos
	{

			dato_leido=dato_rx;
			trama_rx[contador_rx]=dato_leido;
			contador_rx++;
				if(contador_rx>=trama_size)
				{
					contador_rx=0;
					flag_rx=0;
				}


					if(dato_leido=='*')
					{
						contador_rx=0;
						flag_rx=1;

					// HAL_UART_AbortReceive_IT(&uartdatos);
					}
					else HAL_UART_Receive_IT(&uartdatos, &dato_rx, 1);//habilita la int
	}//int por uartdatos

	/* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_RxCpltCallback could be implemented in the user file
   */
}
