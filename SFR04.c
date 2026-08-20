/*
 * SFR04.c
 *
 *  Created on: Apr 9, 2023
 *      Author: Alcides Ramos
 */

#include "SFR04.h"


extern uint32_t pasos;


float SFR04_READ (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,const uint32_t tiempo)
{
uint32_t ciclos;
float valor=0;
float eco;

delay_us_dwt_init();

 //*1000 porque es milisegundos
ciclos=pasos*tiempo*1000;//calcula  el tiempo limite de salida si no detecta pulso,//realiza un disparo

//ulso disparo
 HAL_GPIO_WritePin(TR_GPIO_Port, TR_Pin, 1);
  delay_us_dwt(10);
  HAL_GPIO_WritePin(TR_GPIO_Port, TR_Pin, 0);

  //limpia el contador
  DWT->CYCCNT=0;// Resetea el timer

	while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin)==0)
	{
	if(DWT->CYCCNT>=ciclos) return(0);//si se desborda el contador sale

	}

	DWT->CYCCNT=0;// Resetea el timer
	while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin)==1)
	{
	  if(DWT->CYCCNT>=ciclos) return(0);
	  }
	        eco=DWT->CYCCNT;
			valor=eco/(float)pasos;
			valor=valor/58.0;

  return(valor);

}
