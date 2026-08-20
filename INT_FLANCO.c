/*
 * INT_FLANCO.c
 *
 *  Created on: Oct 6, 2024
 *      Author: ALCIDES_RAMOS
 */
#include "INT_FLANCO.h"



extern uint8_t flag_int;

void interrupcion_flanco()
{
	//analiza que sea la del pin del suiche
	if (__HAL_GPIO_EXTI_GET_IT(SW1_Pin) != RESET)
	{
     flag_int=1; //activa la bandera para avidarle al problama proncipal

	}



}
