/*
 * timer.h
 *
 *  Created on: May 1, 2025
 *      Author: ALCIDES_RAMOS
 */

#ifndef LIBRERIAS_TIMER_H_
#define LIBRERIAS_TIMER_H_
extern uint32_t cuenta;

#include "GY85.h"
//#include "MPU6050.h"


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//mira si el timer desead
	 if (htim->Instance == TIMIMU)
	 {
		 //LEE LOS SENSORES

		 GY85_Read_ace();
	     GY85_Read_giro();
	   GY85_Read_bruj();
	     GY85_Ace_angulos();
	     GY85_Giro_angulos();
	     Filtro_Complementario();
         filtro_kalman();
	  /*  MPU6050_lee_datos();
	    MPU6050_Ace_angulos();
	    MPU6050_Giro_angulos();
	    filtro_kalman();
	    Filtro_Complementario();*/

	 }
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_PeriodElapsedCallback could be implemented in the user file
   */
}


#endif /* LIBRERIAS_TIMER_H_ */
