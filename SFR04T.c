/*
 * SFR04.c
 *
 *  Created on: Apr 9, 2023
 *      Author: Alcides Ramos
 */

#include "SFR04T.h"





float SFR04_READ (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,const uint32_t tiempo)
{
uint32_t ciclos;
float valor=0;
float eco;

delay_init();

 //*1000 porque es milisegundos
ciclos=tiempo*1000;//calcula  el tiempo limite de salida si no detecta pulso,//realiza un disparo

//ulso disparo
 HAL_GPIO_WritePin(TR_GPIO_Port, TR_Pin, 1);
  delay_us(10);
  HAL_GPIO_WritePin(TR_GPIO_Port, TR_Pin, 0);

  //limpia el contador
//  DWT->CYCCNT=0;// Resetea el timer

    TIMDEL->CNT=0;// Resetea el timer
	while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin)==0)
	{
	if(TIMDEL->CNT >=ciclos) return(0);//si se desborda el contador sale

	}

	TIMDEL->CNT=0;// Resetea el timer

	while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin)==1)
	{
	  if(TIMDEL->CNT >=ciclos) return(0);
	  }
	        eco=TIMDEL->CNT ;
			valor=eco;
			valor=valor/58.0;

  return(valor);

}
