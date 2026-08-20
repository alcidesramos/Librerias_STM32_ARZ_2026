/*
 * PID.c
 *
 *  Created on: Nov 2, 2024
 *      Author: ALCIDES_RAMOS
 */


#include "PID.h"

//constantes pid
float kp=1862,ki=0,kd=1833;
float t_mues=0.1;  //colocar el tiempo de meustro

float velo_basica=7187; //velocidad basica de los motores
float velo_giro=8602;// varibale para calcular la velocidad de giro
//DEFINE RANGOS DE OPERACION DEL PID
float PID_MAX=3700;
float PID_MIN=-3700;  //  SE USA NEGATIVO PERO SOLO APRA DECIR QUE DEBE INVERTIR GIRO


float velo_giro;// varibale para calcular la velocidad de giro
float error,up,ui=0.0,error_=0.0,ud,ut;
float setpoint=0;     //setpoint =0  en la recta

float PID_Control(float pid_in)
 {
  PID_MAX=velo_giro;
  PID_MIN=-velo_giro;
	//  se coloca en 1 segundo pero se ajusta o siny¿toniza acorde a los datos obtenidos
//Calculando el error
  error=setpoint-pid_in;

     up=kp*error;    // salida proporcional

     ui+=ki*t_mues*error;      // Salida integral

     ud=kd*(error-error_)/t_mues;  //salida derivativa

       ut=up+ui+ud;    // salida total

       //UBICA LA SALIDA EN LOS VALORES ADECUADOS
       if (ut>PID_MAX) ut=PID_MAX;
       if (ut<PID_MIN) ut=PID_MIN;

      error_=error;

    return(ut);
 }

