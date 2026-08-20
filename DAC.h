/*
 * DAC.h
 *
 *  Created on: 3 may 2026
 *      Author: Alcides Ramos
 */

#ifndef LIBRERIAS_DAC_H_
#define LIBRERIAS_DAC_H_
#include "main.h"


//estructura de DACS
typedef struct {
	DAC_HandleTypeDef *hdac;
	uint32_t channel;
	TIM_HandleTypeDef *htim;
	uint32_t *buffer;
	uint16_t size;
} DACS;

//define tamaño buffer de lod DAC del usuario
#define sizebuff_DACU1 200
#define sizebuff_DACU2 200

//datos propios del dac
#define DAC_MAX_VALUE     4095
#define DAC_CENTER        2048
#define DAC_VREF        3.3f
#define DAC_RESOLUCION 4096.0f   // 12 bits



extern uint32_t DACU1_Buffer[];
extern uint32_t DACU2_Buffer[];
extern uint32_t DACU3_Buffer[];
extern uint32_t DACU3_Buffer[];


extern  DACS   DACU1;
extern  DACS   DACU2;
extern  DACS   DACU3;
extern  DACS   DACU3;


void  DAC_INIT(DACS *dac);
void DAC_OUT_CODIGO(DACS *dac,uint16_t codigo);
void DAC_OUT_VOLTAJE(DACS *dac,float valor);
void  DAC_INIT_DMA(DACS *dac);
void SENO(DACS *dac,float frecuencia,float amplitud,float offset);
void SENO_FASE(DACS *dac, float frecuencia, float amplitud, float offset, float fase_grados);
void TRIANGULAR(DACS *dac,float frecuencia,float amplitud, float offset);
void DIENTE_SIERRA(DACS *dac,float frecuencia,float amplitud,float offset);
void CUADRADA(DACS *dac,float frecuencia,float amplitud, float duty,float offset);
void ROTA_DER(uint32_t *buffer, uint32_t size, uint32_t n);
void ROTA_IZQ(uint32_t *buffer, uint32_t size, uint32_t n);
#endif /* LIBRERIAS_DAC_H_ */
