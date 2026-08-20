/*
 * adc.h
 *
 *  Created on: 01/08/2026
 *      Author: Alcides Ramos
 */

#ifndef LIBRERIAS_ADC_H_
#define LIBRERIAS_ADC_H_
//configurar el DMA en modo circulo si se usa dma , mas de un canal


//en el main si usa DMA

  //ADC_Read_DMA(&hadc1, adc1_canales, adc1_codigo);



#include "main.h"

//habilite los adc a usar
extern ADC_HandleTypeDef hadc1;

#define adc1_canales  4 //  coloque numero de canales del ADC1 a usar


//defina para los adc usado el arreglo para los codigos
extern uint16_t adc1_codigo[];
extern uint8_t adc1_flag; //flag de conversion del adc


uint16_t ADC_Read(ADC_HandleTypeDef  *adc_n);
void ADC_Read_DMA (ADC_HandleTypeDef  *adc_n,uint8_t adc_num,uint16_t *adc_codigo);

#endif /* LIBRERIAS_ADC_H_ */


