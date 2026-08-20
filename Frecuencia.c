/*
 * FRECUENCIA.C
 *
 *  Created on: Dec 25, 2024
 *      Author: ALCIDES_RAMOS
 */


#include "Frecuencia.h"

//incluir los timer usados
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

//configura entradas de frecuencia a usar
FRECS FREC1 = {&htim1, &(TIM1->CCR1),TIM_CHANNEL_1,TIM_FLAG_CC1};
FRECS FREC2 = {&htim1, &(TIM1->CCR4),TIM_CHANNEL_4,TIM_FLAG_CC4};
FRECS FREC3 = {&htim2, &(TIM2->CCR1),TIM_CHANNEL_1,TIM_FLAG_CC1};


void Frec_init(FRECS *frec)
{
	frec->frec_leida=0;//inicia promera lectira en cero
	//calcula la frec de muestreo
	frec->frec_muestreo=SystemCoreClock/((frec->htim->Instance->PSC)+1);
 	  //((TIM1->PSC)+1);
   HAL_TIM_IC_Start_IT(frec->htim, frec->channel);
}

//analiza si la interrpcion d efrecuancia deseada se da
void Frec_INT_Analiza(FRECS *frec)
{
	//  si la bandera del canal x del timer seleccionado esta activo
		if (__HAL_TIM_GET_FLAG(frec->htim, frec->frec_flag) ==1)
			{

	    	if (frec->cap_f1 == 0)
			{
				//CONFIGURA FLANCO DE SUBIDA
				frec->cap_f1=1;
				frec->frec1_1 = *frec->ccr;//lee el contador

			}

			else if (frec->cap_f1 == 1)
			{
						//CONFIGURA FLANCO DE SUBIDA
						frec->cap_f1= 0;
						frec->frec1_2 =   *frec->ccr;

						if (frec->frec1_2 > frec->frec1_1)
						frec->frec_leida = (frec->frec1_2 - frec->frec1_1);
						else
						frec->frec_leida =(frec->htim->Instance->ARR - frec->frec1_1) + frec->frec1_2;
			}

	   	__HAL_TIM_CLEAR_FLAG(frec->htim, frec->frec_flag);//limpia la bandera


			}//fin if


}
void Frec_Interrupt()
{
	//llama las frecuancia deseadas a analizar
	 Frec_INT_Analiza(&FREC1);
	 Frec_INT_Analiza(&FREC2);


}


