/*
 * encoder.c
 *
 *  Created on: Mar 18, 2025
 *      Author: ALCIDES_RAMOS
 */

#include  "encoder.h"

//incluir los timer usados
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;

//configura encoder a usar
ENCODERS ENCODER1 = {&htim3,TIM3};
ENCODERS ENCODER2 = {&htim1,TIM1};



uint16_t encoder_velo=0;
void Encoder_init(ENCODERS *encoder,uint16_t resol)
{
  encoder->posicion=0;
  encoder->pulsos=0;
  encoder->resolucion=resol;
  encoder->velo=0;
  encoder->velo_ant=0;
  HAL_TIM_Encoder_Start_IT(encoder->htim,TIM_CHANNEL_ALL);
  }



//interrpcion de encoder usados
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */

	//agragar por cada encoder usado
	if (htim->Instance == ENCODER1.TIMX)
	{
		ENCODER1.pulsos=__HAL_TIM_GET_COUNTER(htim);
		ENCODER1.posicion=(int16_t)ENCODER1.pulsos/ENCODER1.resolucion;
		ENCODER1.velo=ENCODER1.posicion;

	}

//AGRAGAR POR CADA ENCODER USADO
	//AGRGAGR Y CAMBIAR AL ENCODER CORRESPONDIENTE
	if (htim->Instance == ENCODER2.TIMX)
	{
		ENCODER2.pulsos=__HAL_TIM_GET_COUNTER(htim);
		ENCODER2.posicion=(int16_t)ENCODER2.pulsos/ENCODER2.resolucion;
		ENCODER2.velo=ENCODER2.posicion;

 }

	UNUSED(htim);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_IC_CaptureCallback could be implemented in the user file
   */
}

void Encoder_velo()
		{
  	encoder_velo++;

	if (encoder_velo>500)
	{
		//repetir por cada encoder que se quiera saber velocidad
		ENCODER1.velo=ENCODER1.velo-ENCODER1.velo_ant;
		ENCODER1.velo_ant=ENCODER1.velo;
		ENCODER1.velocidad=ENCODER1.velo/0.5; //2200ms 0.2seg

	    //colocar otro encoder si se desea
			ENCODER2.velo=ENCODER2.velo-ENCODER2.velo_ant;
			ENCODER2.velo_ant=ENCODER2.velo;
			ENCODER2.velocidad=ENCODER2.velo/0.5; //500ms  0.5seg


			//va al final
			encoder_velo=0;//limpia el encoder
	}


		}
