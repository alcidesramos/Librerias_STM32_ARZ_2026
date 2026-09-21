/*
 * PID.h
 *
 *  Created on: Nov 2, 2024
 *      Author: ALCIDES_RAMOS
 */
 

#ifndef SRC_LIBRERIAS_PID_H_
#define SRC_LIBRERIAS_PID_H_

#include "main.h"
#include "math.h"
#include "stdbool.h"
//comentar si no se usa
extern float velo_basica;

typedef struct {
    // Parámetros PID
    float Kp;
    float Ki;
    float Kd;
    float Ts;        // tiempo de muestreo (s)
    float Tau;       // constante del filtro derivativo (s)

    // Límites
    float out_min;
    float out_max;

    // Setpoint
    float setpoint;

    // Estado interno
    float error;
    float prev_error;
    float up;
    float ui;
    float output;

    // Derivada filtrada
    float prev_input;
    float ud;

    // Anti-windup (back-calculation)
    bool anti_windup;
    float Kaw;       // constante de back-calculation
    float aw_term;

} PID_;

extern PID_  PID_DIR;
extern PID_  PID_VEL;


void PID_Init(PID_ *pid,
              float Kp,
              float Ki,
              float Kd,
              float Ts,
              float out_min,
              float out_max,
              float setpoint,
//Tau=ts  respuesta rapida (señal sin ruido)
//Tau=5ts  respuesta media suviza la derivada filtra (señal algo ruido)
//Tau=10ts  respuesta lenta pero suaviza mas (Señal mucho ruido)
			  float Tau,
//kaw=0.1ki  control suave sin tanta saturacion;
//kaw=0.3ki  control rapido algunas saturaciones;
//kaw=0.5ki  a 1.0ki control agresivo
			  float Kaw,
//se calcula automatico inicia siempre en cero
// muestra cuánto está saturando la salida
			 float aw,
			 float up,
			 float ud,
			 float ui,
			 float ut);


float PID_Control(PID_ *pid, float input);
void PID_Reset(PID_ *pid);

#endif 
/*


//se llama asi
 //EN EL MAIN
 PID_DIR_INIT(&PID_DIR);


 //EN EL WHILE
 PID_VEL.setpoint=1.5;

float SALIDA=PID_Control(&PID_DIR,error);

*/
