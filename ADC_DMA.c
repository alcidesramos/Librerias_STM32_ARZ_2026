/*
 * ADC_DMA.c
 *
 *  Created on: Oct 21, 2024
 *      Author: ALCIDES_RAMOS
 */

#include "ADC_DMA.h"

//CONFIGURA QUE ADC VA USAR

//Declaración del buffer  para el DMA1
#define buff1_cont 4  //numero de datos a leer 1 adc
uint16_t adc_buff1[buff1_cont]; // Buffer para almacenar los datos
//define la variable dma
ADC_DMA_ ADCDMA1 = {&hadc1,adc_buff1,buff1_cont};


//SI EL MICRO TIENE MAS ADC Y LOS USA CON OTRO DMA

//#define buff2_cont 2  //numero de datos a leer 1 adc
//uint16_t adc_buff2[buff2_cont]; // Buffer para almacenar los datos
//ADC_DMA_ ADCDMA2 = {&hadc2,adc_buff2,buff2_cont};



//INICIA EL ADC CON DMA DESEADO

void ADC_DMA_INIT(ADC_DMA_ ADC_INFO)
{
HAL_ADC_Start_DMA(ADC_INFO.adc_handle, (uint32_t*)ADC_INFO.adc_buff, ADC_INFO.adc_cuantos);

}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{

	 if (hadc->Instance == ADC1)
	 {
	HAL_ADC_Start_DMA(ADCDMA1.adc_handle, (uint32_t*)ADCDMA1.adc_buff, ADCDMA1.adc_cuantos);

	 }
	/*
        si tuene otros adc con dma declare aca
	 if (hadc->Instance == ADC2)
	 {
	HAL_ADC_Start_DMA(ADCDMA2.adc_handle, (uint32_t*)ADCDMA2.adc_buff, ADCDMA2.adc_cuantos);

	 }


	 */


	UNUSED(hadc);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_ADC_ConvCpltCallback could be implemented in the user file
   */
}

