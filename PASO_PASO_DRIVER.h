/*
 * PASO_PASO_DRIVER.h
 *
 *  Created on: 9 may 2026
 *      Author: Alcides Ramos
 */

#ifndef LIBRERIAS_PASO_PASO_DRIVER_H_
#define LIBRERIAS_PASO_PASO_DRIVER_H_
#include "main.h"
#include "pinout1.h"

#define DER 1
#define IZQ 2
#define QUI 3


#define modo_pasos    0
#define modo_continuo 1

/*
//   en el archivo de interrupcion inlcuir
   #include "PASO_PASO_DRIVER.h"



//  en el procedimiento de  SysTick_Handler  colocar por cada  motor
	 PASO_Analisis(&PASO_PASO1);
	 PASO_Analisis(&PASO_PASO2);

//  EN EL MAIN

   //PUEDE INiciar cada motor

    PASO_PASO1.dir=DER;
     PASO_PASO1.pulsos=20;

    PASO_PASO2.dir=IQZ;

   PASO_Init(&PASO_PASO1);
   PASO_Init(&PASO_PASO2);


  Ya luego puede cambiar para,etro en el while  si lee datos o algo

  PASO_PASO1.modo=modo_continuo;
  PASO_PASO2.modo=modo_pasos;
  PASO_PASO1.dir=DER;

  PASO_PASO2.dir=DER;
  PASO_PASO2.pulsos=20;




 */


//estructura de MOTORES paso -paso
typedef struct {
	//necesarias
	uint8_t  motor_num;
    uint16_t fact_tiempo;
    uint8_t  modo;

    uint16_t fact_aux;
    uint8_t  dir;
    uint16_t pulsos;
    uint16_t  pulso_cont;


}PASOS;


extern  PASOS   PASO_PASO1;
extern  PASOS   PASO_PASO2;
extern  PASOS   PASO_PASO3;
extern  PASOS   PASO_PASO4;
extern  PASOS   PASO_PASO5;
extern  PASOS   PASO_PASO6;


void PASO_Init(PASOS *pas);
void PASO_Analisis(PASOS *pas);


#endif /* LIBRERIAS_PASO_PASO_DRIVER_H_ */
