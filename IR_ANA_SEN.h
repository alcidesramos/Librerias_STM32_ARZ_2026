/*
 * IR_ANA_SEN.h
 *
 *  Created on: Oct 10, 2025
 *      Author: ALCIDES_RAMOS
 */

#ifndef LIBRERIAS_IR_ANA_SEN_H_
#define LIBRERIAS_IR_ANA_SEN_H_

#include "IR_ANA_SEN.h"
#include "main.h"
#include "adc_x.h"
#include "uart.h"
#include "DRV8871.h"



#define ir_n_sensores 10   //numero de sensores
#define linea_blanco  0.4f //umbrar  para saber si no esta en la linea

#define IR_AN_UART huart1
extern UART_HandleTypeDef IR_AN_UART;


extern uint16_t lecturas_ir_an[];
extern float peso;


void Sensores_init();
void Lee_sensores();
void Manda_Lecturas();
void Calibrar_sensores();
void Manda_Limites();
void Normaliza_sensores();
void Manda_Normalizados();
float Calcula_Pesos();
void Manda_pesos();

#endif /* LIBRERIAS_IR_ANA_SEN_H_ */
