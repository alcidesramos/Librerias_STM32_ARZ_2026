/*
 * PWM.C
 *
 *  Created on: Junio 9, 2024
 *      Author: Alcides Ramos
 */

#include "PWMB.h"

float escala=5667;//  define la escala a trabajar el pwm
//float escala=10000;//  define la escala a trabajar el pwm

//incluir los timer usados
extern TIM_HandleTypeDef htim3;

//configurar cvada salida pwm usada
PWMS PWM1 = {&htim3, &(TIM3->CCR1),TIM_CHANNEL_1};
PWMS PWM2 = {&htim3, &(TIM3->CCR2),TIM_CHANNEL_2};
PWMS PWM3 = {&htim3, &(TIM3->CCR3),TIM_CHANNEL_3};
PWMS PWM4 = {&htim3, &(TIM3->CCR4),TIM_CHANNEL_4};



//inicia pwm
void PWM_init(PWMS *pwm)
{
	//innica el timer  y el canl en modo pwm
HAL_TIM_PWM_Start(pwm->htim, pwm->channel);

 }


//apaga, para el  pwm
void PWM_stop(PWMS *pwm)
{
HAL_TIM_PWM_Stop(pwm->htim, pwm->channel);

 }


//saca un valor
void PWM_valor(PWMS *pwm, float valor)
{
//carga valor en el registor ciclo util con la escala
	*pwm->ccr = (uint32_t)(valor * (float)(pwm->htim->Instance->ARR) / escala);
}


void PWM_RAW(PWMS *pwm, uint32_t valor)
{
//carga valor en el registor ciclo util con la escala
	*pwm->ccr = (uint32_t)(valor);
}


void PWM_valor_escala(PWMS *pwm, float valor,float escalar)
{
//carga valor en el registor ciclo util con la escala
	*pwm->ccr = (uint32_t)(valor * (float)(pwm->htim->Instance->ARR) / escalar);
}
