/*
 * CRUCE_CERO.h
 *
 *  Created on: Jan 28, 2025
 *      Author: ALCIDES_RAMOS
 */

#ifndef SRC_LIBRERIAS_CRUCE_CERO_H_
#define SRC_LIBRERIAS_CRUCE_CERO_H_
#include "main.h"
#include "pinout1.h"
#include "delays.h"


//PINES ASOCIADOS AL CRUCE CERO
//DISPARO
//#define DISP_Pin GPIO_PIN_12
//#define DISP_GPIO_Port GPIOB

//FLANCO  ES MEJOR LLAMAR ESTE PIN  C_CERO EN CUBEMX
//Y ASI TOMA LA INTERRPCION ASOCIADA

//#define C_CERO_Pin GPIO_PIN_13
//#define C_CERO_GPIO_Port GPIOB
//#define C_CERO_EXTI_IRQn EXTI15_10_IRQn //INT USADA

//TIMER USADO
#define  TIM_CERO     htim5
#define  TIM_CERO_V   TIM5

//define limites en microsegundos minimo y maximo para disparo
#define LIM_INF 200   //no inductiva
//#define LIM_INF 1500    //inductiva
#define LIM_SUP 8000    //no inductiva
//#define LIM_SUP 7000    //inductiva




#define LIM_BASE 100//ofsset de encendido y apagado


#define INT_CERO  EXTI_



//si el bombillo titilea usba el limite inferior o disminuya el superior
//y por un if controle cuando apaga y cuando enciende total el bombillo


void cruce_init();
void flanco_cero_int();
void timer_cero_int();
void Cruce_set(float *valor_tiempo);


extern TIM_HandleTypeDef  TIM_CERO;


#endif /* SRC_LIBRERIAS_CRUCE_CERO_H_ */
