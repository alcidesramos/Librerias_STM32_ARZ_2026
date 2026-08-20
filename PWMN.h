/*
 * PWM.H
 *
 *  Created on: Apr 8, 2023
 *      Author: Alcides Ramos
 */
//en el main
/*
 PWM_init(&htim1, PWM1_CH);//timer y canal a configurar
 PWM_init(&htim1, PWM2_CH);

// en el while
/*
 PWM_valor(&PWM1, 50);//canal y valor
 PWM_valor(&PWM2, 100);

*/


#include "main.h"


#ifndef LIBRERIAS_PWM_H_
#define LIBRERIAS_PWM_H_


#define PWMT	 htim1   // timer usado
#define PWMTIM 	 TIM1//registro timar usado


//canales pwm a usar

///pwm1
#define PWM1_CH     TIM_CHANNEL_1   //canal de pwm1
#define PWM1 		TIM1->CCR1  //registro del timer usado y canal

//pwm2
#define PWM2_CH 	TIM_CHANNEL_2  //canal pwm2
#define PWM2 		TIM1->CCR2  //registro cilco util canal2 y


//pwm3
#define PWM3_CH 	TIM_CHANNEL_3  //canal pwm3
#define PWM3 		TIM1->CCR3  //registro cilco util canal3 y

void PWM_init(TIM_HandleTypeDef *serv,uint16_t canal);
void PWM_init_comple(TIM_HandleTypeDef *serv,uint16_t canal);

void PWM_valor(uint32_t *canal,float valor);



#endif /* LIBRERIAS_PWM_H_ */
