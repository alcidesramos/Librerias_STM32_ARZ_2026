/*
 * adc_dma.h
 *
 *  Created on: Oct 21, 2024
 *      Author: ALCIDES_RAMOS
 */

#ifndef LIBRERIAS_ADC_DMA_H_
#define LIBRERIAS_ADC_DMA_H_
#include "main.h"


//ADC UTILIZADO
extern ADC_HandleTypeDef hadc1;//ADC Utilizado
//Si usa otro adc debe colocarlo tambien aqui

//como usar
/*
 en el main, antes del while solo usar llamar
  ADC_DMA_INIT(ADCDMA1);//  LLAMAR  ADC_DMA DESEADO


  EN EL WHILE SOLO LEA LOS DATOS
   VAR1=ADCDMA1.adc_buff[0];
 */



//estructura de pwm
typedef struct {
	ADC_HandleTypeDef *adc_handle;
	uint16_t *adc_buff;
     uint8_t adc_cuantos;
} ADC_DMA_;

//posibles adc a usar
extern ADC_DMA_ ADCDMA1;
//si desea usar mas adc quedan disponible para declaracion
extern ADC_DMA_ ADCDMA2;
extern ADC_DMA_ ADCDMA3;
extern ADC_DMA_ ADCDMA4;
extern ADC_DMA_ ADCDMA5;

extern uint16_t adc_buff1[]; // Buffer para almacenar los datos
extern uint16_t adc_buff2[]; // Buffer para almacenar los datos
extern uint16_t adc_buff3[]; // Buffer para almacenar los datos
extern uint16_t adc_buff4[]; // Buffer para almacenar los datos
extern uint16_t adc_buff5[]; // Buffer para almacenar los datos

void ADC_DMA_INIT(ADC_DMA_ ADC_INFO);


#endif /* LIBRERIAS_ADC_DMA_H_ */
