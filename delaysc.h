/*
 * retardos.h
 *
 *  Created on: 12/03/2023
 *      Author: Alcides Ramos
 */

#ifndef LIBRERIAS_RETARDOS_H_
#define LIBRERIAS_RETARDOS_H_

#include "main.h"

//DEFINIR TIMER A USAR HABILITAR SOLO SI USAR TIMERS
//ojo usar timer de 32 bits 2  ó 5
//#define TIMX TIM5   //USA EL TIMER 5
//#define TIMERX htim5 //USA EL TIMER 5

//Define cual se usa como delay init
#define delay_init delay_us_dwt_init// la funcion usada para inicializar los delay
//#define delay_init  delay_us_tim_init //si usa timer

//DEFINE CUAL SERA USADO COMO DELAY_US
#define Delay_us delay_us_dwt  //timer de debug
//#define Delay_us delay_us_tim   //timer deseado

#define delay_us Delay_us //para aceptar en mayuscula o minuscula
#define delay_ms Delay_ms

#define Delay_init delay_init
#define Delay_Init delay_init
#define delay_Init delay_init

void delay_us_C (uint32_t reta);
void delay_us_ASM(uint32_t us);
void delay_us_ASM_IT(uint32_t us);
void delay_us_tim_init();
void delay_us_tim (uint32_t us);
void delay_us_dwt_init();
void delay_us_dwt(uint32_t reta);
void Delay_ms(uint32_t ms);



#endif /* LIBRERIAS_RETARDOS_H_ */
