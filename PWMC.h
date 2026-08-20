/*
 * PWM.H
 *
  *  Created on: Junio 9, 2024
 *      Author: Alcides Ramos
*/

//en el main
/*

// en el while
/*

*/


#include "main.h"


#ifndef LIBRERIAS_PWMC_H_
#define LIBRERIAS_PWMC_H_

//estructura de pwm
typedef struct {
    TIM_HandleTypeDef *htim;
    volatile uint32_t *ccr;
    uint32_t channel;
} PWMSC;

//canales pwm a usar
extern PWMSC PWMC1;
extern PWMSC PWMC2;
extern PWMSC PWMC3;
extern PWMSC PWMC4;
extern PWMSC PWMC5;
extern PWMSC PWMC6;
extern PWMSC PWMC7;
extern PWMSC PWMC8;
extern PWMSC PWMC9;
extern PWMSC PWMC10;

void PWMC_init(PWMSC *pwm);
void  PWMCP_stop(PWMSC *pwm);
void PWMC_valor(PWMSC *pwm, float valor);
void PWMC_valor_escala(PWMSC *pwm, float valor,float escalar);


#endif /* LIBRERIAS_PWM_H_ */
