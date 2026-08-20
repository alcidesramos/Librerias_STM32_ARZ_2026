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


extern float velo_giro;// varibale para calcular la velocidad de giro
//extern float velo_der,velo_izq; // velocidad a cada motor

extern float velo_basica; //velocidad basica de los motores
extern float velo_giro_max;  //velocidad para contol de giro
extern float PID_MAX;
extern float PID_MIN;  //  SE USA NEGATIVO PERO SOLO APRA DECIR QUE DEBE INVERTIR GIRO

//para poder usarlas en el main sin definir
extern float kp,ki,kd;
extern float error,up,ui,error_,ud,ut;
extern float setpoint;


float PID_Control(float pid_in);


#endif /* SRC_LIBRERIAS_PID_H_ */
