/*
 * adc.c
 *
 *  Created on: 01/08/2026
 *      Author: Alcides Ramos
 */


#include "adc_x.h"
#include <stdint.h>

//defina los adc a usar
uint16_t adc1_codigo[ adc1_canales];
uint8_t adc1_flag = 0; //flag de conversion del adc

uint16_t ADC_Read(ADC_HandleTypeDef  *adc_n)
{
	HAL_ADC_Start(adc_n);
	HAL_ADC_PollForConversion(adc_n,1000);
	HAL_ADC_Stop(adc_n);
	return (HAL_ADC_GetValue(adc_n));
}

void ADC_Read_DMA (ADC_HandleTypeDef  *adc_n,uint8_t adc_num,uint16_t *adc_codigo)
{

HAL_ADC_Start_DMA(adc_n, (uint32_t*)adc_codigo, adc_num);
 // HAL_Delay(10);//retardo de conversion si necesita
}



void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	//Si es el adc1
    if (hadc->Instance == ADC1) 
    {
        adc1_flag = 1;
    }
//si es otro adce colocar la bandera

}




