/*
 * PWM.C
 *
 *  Created on: Apr 8, 2023
 *      Author: Alcides Ramos
 */

//como se usa



#include "PWMN.h"

float escala=100;//  define la escala a trabajar el pwm
//float escala=10000;//  define la escala a trabajar el pwm

void  PWM_init(TIM_HandleTypeDef *PWM,uint16_t canal)
{

HAL_TIM_PWM_Start(PWM, canal);

}


void PWM_valor(uint32_t *canal,float valor)
{
	//PWMTIM1->ARR   =ESCALA
	 //    ?         =valor

*canal=valor*(float)PWMTIM->ARR/escala;  // lo escala de 0 a 100%

}
