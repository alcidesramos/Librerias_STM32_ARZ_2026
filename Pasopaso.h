/*
 * Pasopaso.h
 *
 *  Created on: May 22, 2024
 *      Author: Alcides Ramos
 */

#ifndef LIBRERIAS_PASOPASO_H_
#define LIBRERIAS_PASOPASO_H_
#include "main.h"
#include "bits.h"

//definir estos factores
#define factor 10// factor de velocidad en milisegundos mas pequeño mas rapido
#define vel_max  100




//como usar
/*  en el archivo stm32f4xx_it.h
 #include "Pasopaso.h"
 definer la variable
 uint16_t fact_vel=0;


 en el procedimiento SysTick_Handler(void)poner

 fact_vel++;

		if (fact_vel>=factor)
		{
		pasopaso_int();
		fact_vel=0;
		}


*/

//incluir en el main

//#include "Pasopaso.h"
//ejemplo de uso solo dar el estado velocidad o nuemro de pasos
/*
 estado_giro1=izquierda;
  estado_giro2=derecha;

  velo_m1=100;
  n_pasos2=30;
  velo_m2=100;
  estado_modo1=continuo;
  estado_modo2=pasos;

 */

// si giran  contrario cambiar  derecha por izquierda y visceversa
#define derecha 1
#define izquierda 2
#define quieto 3

//modo de giro de los motores
#define continuo 1
#define pasos 2


//variables del dentido de giro d elos motores
extern uint8_t giro_m1,giro_m2;
//variable de la velocidad d elos motores de 0 - vel_max
extern uint16_t velo_m1,velo_m2;
//variable almacena numero pasos a dar en modo pasos
extern uint32_t  n_pasos1,n_pasos2;
//contador de numeros de pasos dado por los motores
extern uint32_t  pasos1,pasos2;
extern uint8_t estado_giro1,estado_giro2;
extern uint8_t estado_modo1,estado_modo2;


void pasopaso_int();
//MOTOR 1
void saca_secuenciaM1();
void izquierdaM1();
void derechaM1();
//MOTOR 2
void saca_secuenciaM2();
void izquierdaM2();
void derechaM2();


#endif /* LIBRERIAS_PASOPASO_H_ */
