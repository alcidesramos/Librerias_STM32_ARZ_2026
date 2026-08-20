/*
 * adc.h
 *
 *  Created on: 15/11/2021
 *      Author: Alcides Ramos
 */

#ifndef LIBRERIAS_ADC_H_
#define LIBRERIAS_ADC_H_
//configurar el DMA en normal si se usa dma , mas de un canal

#include "main.h"
#define adc1 hadc1

#define adc_canales  3 //  coloque numuro de canales del ADC a usar

extern uint16_t adc_codigo[];


uint16_t ADC_Read();
void ADC_Read_DMA();


#endif /* LIBRERIAS_ADC_H_ */


