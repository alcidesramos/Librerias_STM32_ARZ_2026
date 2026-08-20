/*
 * timer.h
 *
 *  Created on: Oct 30, 2025
 *      Author: ALCIDES_RAMOS
 */

#ifndef LIBRERIAS_TIMER_H_
#define LIBRERIAS_TIMER_H_

#include "main.h"
//incluya libreria de sensores usados en la interrpcion del timer


//DEFINA TIMER A USAR
#define TIM_INT TIM3
#define htim_INT htim3







extern uint32_t tiempo_imu;
extern TIM_HandleTypeDef htim_INT;

extern  uint32_t tmicros;
extern float tmilis;


void TIMER_INIT();
void TIMER_MIDE();
void TIMER_PROCEDIMIENTOS();


#endif /* LIBRERIAS_TIMER_H_ */
