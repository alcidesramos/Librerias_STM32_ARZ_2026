/*
 * PWM_IN.H
 *
 *  Created on: Jul 20, 2023
 *      Author: alcid
 */

#ifndef LIBRERIAS_PWM_IN_H_
#define LIBRERIAS_PWM_IN_H_

#include "main.h"

#define rojo_min  30000
#define verde_min 12800
#define azul_min  30000

//estructura de colors
typedef struct {
    TIM_HandleTypeDef *htim;
    volatile uint32_t *ccr;
    uint32_t channel;
    uint32_t flag_co;

} COLORS;


extern COLORS TCS3200_SEN;


void  TCS3200_Init();
void TCS3200_Interrupt();



#endif /* LIBRERIAS_PWM_IN_H_ */
