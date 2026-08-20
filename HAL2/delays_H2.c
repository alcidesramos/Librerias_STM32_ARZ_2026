/*
 * retardos.c
 *
 *  Created on: 12/03/2023
 *      Author: Alcides Ramos
 */

#include "delays.h"
#include "HAL2_PERIFERICOS.h"

uint32_t pasos;

//RETARDOS POR TIMER DE DEBUG


void delay_us_dwt_init()
{
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    pasos=(HAL_RCC_GetSysClockFreq()/1000000);//le el cristal pasa a us

}

//retardo por debbuger
void delay_us_dwt(uint32_t reta)
{

DWT->CYCCNT=0;
while( DWT->CYCCNT<=pasos*reta);//multiplica por us
}






#ifdef TIMDEL

void delay_us_tim_init()
{
   // HAL_TIM_Base_Start(&TIMERDEL); // CON HAL inicia el timer 1


	//SIN HAL
	TIMDEL->CR1|=1>>0;// HABILITA EL TIMER BIT 0 EN 1 EN

}


void delay_us_tim (uint32_t us)
{
	//CON HAL
	//__HAL_TIM_SET_COUNTER(&TIMERDEL,0);  // CON HAL  Carga el timer con 0
//	while ((uint16_t)__HAL_TIM_GET_COUNTER(&TIMERDEL) < us);// CON HAL espera los us deseados ojo si es 32 bit poner uint32_t

	//SIN HAL
     TIMDEL->CNT=0;// Resetea el timer
	while (TIMDEL->CNT < us);// ESPERA LOS US DESEADOS

}

#endif



void Delay_ms(uint32_t ms)
{
    uint32_t us = ms * 1000;
  Delay_us(us);  //el que fue definido
}




