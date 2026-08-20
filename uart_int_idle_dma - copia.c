//COMO USAR


//en el void main antes del while(1) poner
/*
uartdat_it_idle_dma_init();
 */

//en el while poner
      /*
      if(flag_rx==1)
       {
		  procesa_rx();
		  flag_rx=0;
		 uartdat_it_idle_dma_init();
		 }

*/
// y debe haber un procedimiento llamado procesa_rx()  en el main
//que procesa los datos recibidos



/* USER CODE END Includes */
#include <uart_int_idle_dma.h>
#include "main.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"


uint8_t trama_rx[trama_size];
char flag_rx=0;
uint16_t num_datos;
char procesa[trama_size];



void  uartdat_it_idle_dma_init()
{
    memset(trama_rx,0,trama_size-1);//limpia el  buffer

// en STM32G y stmH
   // uint8_t dummy = uartdatosreg->RDR;
    //uartdatosreg->ICR = USART_ICR_ORECF;  // Escribir 1 en ORECF para limpiar ORE

   //  en STM32F
    uartdatosreg->SR &= ~( UART_FLAG_RXNE | UART_FLAG_ORE);
   	uint8_t  dummy = uartdatosreg->DR;

    HAL_UARTEx_ReceiveToIdle_DMA(&uartdatos, (uint8_t*)trama_rx, trama_size);//inica la recepcion por idle
}


//procedimiento de  idle para recepcion serial
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if ((flag_rx==0)&& (huart->Instance == uartdatosreg))//si es el uart de datos
	{
	HAL_UART_DMAStop(&uartdatos);  //para la recepcion temporarmente
	num_datos=Size;
	  flag_rx=1;
    	}
		
		

	//solo si se usa ubidots
	//#include "UBIDOTS.h"
//	Ubidots_Interrutp(huart);

}


