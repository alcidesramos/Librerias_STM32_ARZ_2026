
/*
 * timer.h
 *
 *  Created on: May 1, 2025
 *      Author: ALCIDES_RAMOS
 */
#include "timer.h"

uint32_t pasos1;



uint32_t tmicros;
float tmilis;

//LO QUE  SE REALIZA EN EL TIEMPO DE MEUSTTREO
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//mira si el timer desead0
	 if (htim->Instance == TIM_INT)
	 {
  	 	   //IMU
		 TIMER_PROCEDIMIENTOS();
	 }
			 /* Prevent unused argument(s) compilation warning */
		  UNUSED(htim);

		  /* NOTE : This function should not be modified, when the callback is needed,
		            the HAL_TIM_PeriodElapsedCallback could be implemented in the user file
		   */
}

void TIMER_PROCEDIMIENTOS()
{
	//COLOQUE LOS PROCEDIMIENTOS  USAR ACORDE  SENSORES
	         MPU6050_lee_datos();
	         MPU6050_calcula_flotantes();

	  	     MPU6050_Ace_angulos();
			  MPU6050_Giro_angulos();
			  Filtro_Complementario(0.98);
			  filtro_kalman();
}

void TIMER_INIT()
{
	HAL_TIM_Base_Start_IT(&htim_INT);
}


//MIDO   TIEMPO DE ESOS PROCEDIEMTOS
void TIMER_MIDE()
{
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
	    pasos1=(HAL_RCC_GetSysClockFreq()/1000000);//le el cristal pasa a us

	    // INICIA CONTADOR
	    	    DWT->CYCCNT=0;
	    	    TIMER_PROCEDIMIENTOS();
	    	    tmicros= (DWT->CYCCNT / pasos1);//retrona los us
                 tmilis=tmicros/1000.0;
}






