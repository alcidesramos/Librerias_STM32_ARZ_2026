/*
 * retardos.c
 *
 *  Created on: 12/03/2023
 *      Author: Alcides Ramos
 */

#include "delays.h"

uint32_t pasos;

#pragma GCC push_options   //guardar la optimizacion

#pragma GCC optimize ("O0")//sin optimizacion

//realizar primero poniendo reta*1
void delay_us_C (uint32_t reta)// reta us deseados
{
uint32_t calibra;
//calibra=reta*8;  //ecuacion de calibracion  72 MHZ BLUE *8
calibra=reta*12-4;  //ECUACION CALIBRACION F401 84MHZ
//calibra=reta*14-4; //ECUACION CALIBRACION F411 100MHZ
while(calibra--);
}
#pragma GCC pop_options// recuera la optimizacion guardaa


//DELAY CON ENSAMBLADOR MODO RUTINA 84MHX   21  F401   25 F411
#pragma GCC push_options   //guardar la optimizacion
#pragma GCC optimize ("O0")//sin optimizacion

//12 PARA LA BLUPILL A 72MH  *12
//21 PARA LA 401 A 84MH
//25  f411  a 100 MHZ
void delay_us_ASM(uint32_t us) {
    asm volatile ("MOV R1, #21\n"
                  "MUL R1, R1, %[ciclos]\n"
            	  "MOV R0, R1\n"
                  "1:\n"
                  "SUB R0, #1\n"
                  "CMP R0, #0\n"
                  "BNE 1b\t"
                  :
                  : [ciclos] "r" (us)
                  : "memory");
}
#pragma GCC pop_options// recuera la optimizacion guardaa


//delay en ensamblador deshabilitando las interrpciones
//12 PARA LA BLUPILL A 72MH  *12
//21 PARA LA 401 A 84MH
//25 f411 a 100 MHZ


//DELAY CON ENSAMBLADOR MODO RUTINA
#pragma GCC push_options   //guardar la optimizacion
#pragma GCC optimize ("O0")//sin optimizacion

void delay_us_ASM_IT(uint32_t us) {
    asm volatile ("CPSID i\n" \
    		      "MOV R1, #21\n"
                  "MUL R1, R1, %[ciclos]\n"
            	  "MOV R0, R1\n"
                  "1:\n"
                  "SUB R0, #1\n"
                  "CMP R0, #0\n"
                  "BNE 1b\n"
    	          "CPSIE i\t"
    		      :
                  : [ciclos] "r" (us)
                  : "memory");
}
#pragma GCC pop_options// recuera la optimizacion guardaa


#ifdef TIMX

void delay_us_tim_init()
{
   // HAL_TIM_Base_Start(&TIMERX); // CON HAL inicia el timer 1


	//SIN HAL
	TIMX->CR1|=1>>0;// HABILITA EL TIMER BIT 0 EN 1 EN

}


void delay_us_tim (uint32_t us)
{
	//CON HAL
	//__HAL_TIM_SET_COUNTER(&TIMERX,0);  // CON HAL  Carga el timer con 0
//	while ((uint16_t)__HAL_TIM_GET_COUNTER(&TIMERX) < us);// CON HAL espera los us deseados ojo si es 32 bit poner uint32_t

	//SIN HAL
     TIMX->CNT=0;// Resetea el timer
	while (TIMX->CNT < us);// ESPERA LOS US DESEADOS

}

#endif


#if __CORTEX_M !=0

void delay_us_dwt_init()
{
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    pasos=( HAL_RCC_GetHCLKFreq()/1000000);//le el cristal pasa a us

}

//retardo por debbuger
void delay_us_dwt(uint32_t reta)
{

DWT->CYCCNT=0;
while( DWT->CYCCNT<=pasos*reta);//multiplica por us
}
#endif

void Delay_ms(uint32_t ms)
{
    uint32_t us = ms * 1000;
  Delay_us(us);  //el que fue definido
}




