/*
 * servos.c
 *
 *  Created on: Apr 8, 2023
 *      Author: Alcides Ramos
 */


#include "servos.h"


//incluir los timer usados
extern TIM_HandleTypeDef htim1;



//Configurar Servos Usados
SERVOS SERVO1 = {&htim1, &(TIM1->CCR3),TIM_CHANNEL_3};





void   SERVO_init(SERVOS *servo)
{
HAL_TIM_PWM_Start(servo->htim, servo->channel);

}


void SERVO_ANG(SERVOS *servo,float posi)

{
     float calcu;
      calcu=(ser_lim_sup_ms - ser_lim_inf_ms) / (ser_sup - ser_inf);
	  calcu=calcu*(posi-ser_inf);
	  calcu=(calcu+ser_lim_inf_ms)*1000.0;// para pasarlo a microsegundo
	  *servo->ccr = calcu;

}


void SERVO_MICRO(SERVOS *servo,float micro)

{
     *servo->ccr = micro;
}


void SERVO_MILI(SERVOS *servo,float milis)

{
     float calcu;
      calcu=milis*1000.0;// para pasarlo a microsegundo
	  *servo->ccr = calcu;
}


void SERVO_MUEVE(SERVOS *servo,float ini, float final,float paso,float ret)
{
if (final>ini)
{               //-90    90
  for	(float c=ini; c<final;c+=paso)
  {
	SERVO_ANG(servo, c);
	HAL_Delay(ret) ;
  }

}
else
{                  //90     -90
	  for	(float c=ini; c>final;c-=paso)
	  {
		SERVO_ANG(servo, c);
		HAL_Delay(ret) ;
	  }

}
}
