/*
 * adc.c
 *
 *  Created on: 15/11/2021
 *      Author: Alcides Ramos
 */


#include "adc.h"

extern ADC_HandleTypeDef adc1;
uint16_t adc_codigo[ adc_canales];

uint16_t ADC_Read()
{
	HAL_ADC_Start(&adc1);
	HAL_ADC_PollForConversion(&adc1,1000);
	HAL_ADC_Stop(&adc1);
	return (HAL_ADC_GetValue(&adc1));
}

void ADC_Read_DMA()
{
HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_codigo, adc_canales);
   HAL_Delay(1);
}

