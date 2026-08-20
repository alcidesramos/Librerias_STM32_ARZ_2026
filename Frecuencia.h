/*
 * FRECUENCIA.H
 *
 *  Created on: Dec 25, 2024
 *      Author: ALCIDES_RAMOS
 */

//como se usa

/*
 en el  frecuencia.c
 incluya los timer a usar
 extern TIM_HandleTypeDef htim1;

 configure cada entrada de trecuencia
 //configura entradas de frecuencia a usar timer y canal
FRECS FREC1 = {&htim1, &(TIM1->CCR1),TIM_CHANNEL_1,TIM_FLAG_CC1};


 */

/*
 en el archivo  de inetrrupcion.c
 incluya
#include "Frecuencia.h"
en cada interrpcion del timer analice el canal o canales deseado
    	 Frec_INT_Analiza(&FREC1);
    	 Frec_INT_Analiza(&FREC2);

 */

/*
 en el main.c
 //incluya  ferceuncia.h
   #include "Frecuencia.h"

 antes del while inicie los modulos de frecuencia
     Frec_init(&FREC1);
     Frec_init(&FREC2);

 //en el  while  declare las varibles de frecuancia a usar
   float frecuencia1,frecuencia2;

 //lea las frecuencia
 if (FREC1.frec_leida==0) frecuencia1=0;
	else
	frecuencia1=FREC1.frec_muestreo/FREC1.frec_leida;
//coloca la leida en cero apra otra lectura
	FREC1.frec_leida=0;

	 if (FREC2.frec_leida==0) frecuencia2=0;
  	else
  	frecuencia2=FREC2.frec_muestreo/FREC2.frec_leida;
  //coloca la leida en cero apra otra lectura
  	FREC2.frec_leida=0;



 */
#ifndef LIBRERIAS_FRECUENCIA_H_
#define LIBRERIAS_FRECUENCIA_H_

#include "main.h"


//estructura de FREC
typedef struct {
    TIM_HandleTypeDef *htim;
    volatile uint32_t *ccr;
    uint32_t channel;
    uint32_t frec_flag;
    uint32_t frec_muestreo;
    float frec_leida;
    uint32_t frec1_1,frec1_2;
    uint8_t cap_f1,cap_f2;

} FRECS;


//canales FREC a usar
extern FRECS FREC1;
extern FRECS FREC2;
extern FRECS FREC3;
extern FRECS FREC4;
extern FRECS FREC5;
extern FRECS FREC6;
extern FRECS FREC7;
extern FRECS FREC8;
extern FRECS FREC9;
extern FRECS FREC10;
extern FRECS FREC11;
extern FRECS FREC12;
extern FRECS FREC13;
extern FRECS FREC14;
extern FRECS FREC15;
extern FRECS FREC16;
extern FRECS FREC17;
extern FRECS FREC18;
extern FRECS FREC19;
extern FRECS FREC20;

void Frec_init(FRECS *frec);
void Frec_INT_Analiza(FRECS *frec);

#endif /* LIBRERIAS_FRECUENCIA_H_ */
