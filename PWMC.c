/*
 * PWM.C
 *
 *  Created on: Junio 9, 2024
 *      Author: Alcides Ramos
 */

#include "PWMC.h"

float escalac=800;//  define la escala a trabajar el pwm
//float escala=10000;//  define la escala a trabajar el pwm

//incluir los timer usados
extern TIM_HandleTypeDef htim1;


//configurar cvada salida pwm usada
PWMSC PWMC1 = {&htim1, &(TIM1->CCR1),TIM_CHANNEL_1};

//inicia pwm
void PWMC_init(PWMSC *pwm)
{
	//innica el timer  y el canal en modo complementario
HAL_TIM_PWM_Start(pwm->htim, pwm->channel);
HAL_TIMEx_PWMN_Start(pwm->htim, pwm->channel);
 }
void  PWMCP_stop(PWMSC *pwm)
{

HAL_TIM_PWM_Stop(pwm->htim, pwm->channel);
HAL_TIMEx_PWMN_Stop(pwm->htim, pwm->channel);
}

//saca un valor
void PWMC_valor(PWMSC *pwm, float valor)
{
//carga valor en el registor ciclo util con la escala
	*pwm->ccr = (uint32_t)(valor * (float)(pwm->htim->Instance->ARR) / escalac);
}

void PWMC_valor_escala(PWMSC *pwm, float valor,float escalar)
{
//carga valor en el registor ciclo util con la escala
	*pwm->ccr = (uint32_t)(valor * (float)(pwm->htim->Instance->ARR) / escalar);
}
