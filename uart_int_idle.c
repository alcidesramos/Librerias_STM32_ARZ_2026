//COMO USAR

//DECLARAR EN EL MAIN.C

//variables recepcion por idle
/*
//variables recepcion por idle
uint8_t trama_rx[trama_size];
char flag_rx=0;
uint16_t num_datos;
*/



//en el void main antes del while(1) poner
/*
uartdat_it_idle_init();
 */

//en el while poner
      /*
      if(flag_rx==1)
       {
		  procesarx();
		  flag_rx=0;
		 uartdat_it_idle_init();
		 }

*/
// y debe haber un procedimiento llamado procesarx()  en el main
//que procesa los datos recibidos



/* USER CODE END Includes */
#include "main.h"
#include "string.h"
#include "uart_int_idle.h"

extern char flag_rx;
extern uint16_t num_datos;
extern uint8_t trama_rx[];



void uartdat_it_idle_init()
{
    memset(trama_rx,0,trama_size);//limpia el  buffer
   //limpia banderas asociadas erroes de recepcion
__HAL_UART_CLEAR_OREFLAG(&uartdatos); // Limpia Overrun Error
__HAL_UART_FLUSH_DRREGISTER(&uartdatos); // Limpia buffer de entrada


	 HAL_UARTEx_ReceiveToIdle_IT(&uartdatos, (uint8_t*)trama_rx, trama_size);//inica la recepcion por idle


}


//procedimiento de  idle para recepcion serial
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{


		if (huart->Instance == uartdatosreg)//si es uuartdatps sino cambiar
	{
	  num_datos=Size;
	  flag_rx=1;

}



}
