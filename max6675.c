/*
 * max6675.c
 *
 *  Created on: Apr 9, 2023
 *      Author: Alcides Ramos
 */


#include "max6675.h"

void MAX6675_init()
{
	  HAL_GPIO_WritePin(CK_MAX_GPIO_Port, CK_MAX_Pin,0);//SACA UN 0 reloj
	  HAL_GPIO_WritePin(CS_MAX_GPIO_Port, CS_MAX_Pin, 1); //APAGA el modulo
	   HAL_Delay(100);
}


uint8_t MAX6675_pulso()
{
uint8_t lei;
 HAL_GPIO_WritePin(CK_MAX_GPIO_Port, CK_MAX_Pin,1);//SACA UN 1 reloj
  Delay_us(10);
  lei=HAL_GPIO_ReadPin(DAT_MAX_GPIO_Port, DAT_MAX_Pin);// lee el pin
  HAL_GPIO_WritePin(CK_MAX_GPIO_Port, CK_MAX_Pin,0);//SACA UN 0 reloj
  Delay_us(10);
  return lei;
}

float MAX6675_lee()
{
uint16_t sensor_dato;

uint16_t max6675_dato=0;
int8_t cont;

HAL_GPIO_WritePin(CS_MAX_GPIO_Port, CS_MAX_Pin, 0); //Habilita el modulo

   for (cont=15;cont>=0;cont--)
    {
      if (MAX6675_pulso()==1)  max6675_dato=max6675_dato | 1<<cont ;

    }//fin for

   HAL_GPIO_WritePin(CS_MAX_GPIO_Port, CS_MAX_Pin, 1); //APAGA el modulo
    sensor_dato=(max6675_dato>>3 ); //desplaz 3 veces
    return(((float)sensor_dato*0.25));
}
