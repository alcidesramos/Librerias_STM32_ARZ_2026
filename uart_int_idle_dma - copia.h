/*
 * uart_int_idle.h
 *
 *  Created on: Dec 25, 2023
 *      Author: Alcides Ramos
 */
#include "main.h"

#define trama_rx  trama_rx // nombre de la trama a usar
#define uartdatosreg USART1   //define puerto serial a analizar
#define uartdatos huart1
# define trama_size 100  //coloque el tamaño del buffer



extern char flag_rx;
extern uint16_t num_datos;
extern uint8_t trama_rx[];
extern  char procesa[];

/*
void procesa_rx()
{
	if (strstr(trama_rx,"V00=1")) PIN_OFF(LED);
	else if (strstr(trama_rx,"V00=0")) PIN_ON(LED);

	else if (strstr(trama_rx,"V01="))
	    {
	        strcpy(procesa, strtok(trama_rx, "="));  //inicia captura de tokens desde el =
	        strcpy(procesa, strtok(0, "$"));  //captura hasta el /
	        //pasa la cadena a  numero
	       uint16_t ancho=atoi(procesa);
	       PWM_valor(&PWM1, ancho);

	    }

}

*/



#ifndef LIBRERIAS_UART_INT_IDLE_H_
#define LIBRERIAS_UART_INT_IDLE_H_

extern UART_HandleTypeDef uartdatos;

void uartdat_it_idle_dma_init();


#endif /* LIBRERIAS_UART_INT_IDLE_H_ */
