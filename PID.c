/*
 * PID.c
 *  Created on: Nov 2, 2024
 * Autor: Alcides Ramos 
 */

#include <PID.h>

//dEFINO pid a usar
PID_  PID_DIR;
PID_  PID_VEL;

//variables adicionales si se requieren
float velo_basica;

//procedimeintos iniciar PID  usados
void PID_DIR_INIT()
{
	//constantes
	PID_DIR.Kp=100;  //KP
	PID_DIR.Ki=0;    //KI
	PID_DIR.Kd=10;    //KD
	PID_DIR.Ts=10e-3;  //Tiempo muestreo
	PID_DIR.out_min=-250; //rango minimo PID
	PID_DIR.out_max=250;  //Rango Maximo
	PID_DIR.setpoint=0.0;  //set point inicial
//Tau=ts  respuesta rapida (señal sin ruido)
//Tau=5ts  respuesta media suviza la derivada filtra (señal algo ruido)
//Tau=10ts  respuesta lenta pero suaviza mas (Señal mucho ruido)
	//inicia con tau  5 veces tiempo de muestreo
	PID_DIR.Tau=5*PID_DIR.Ts; // tau filtrado derivativo
//kaw=0.1ki  control suave sin tanta saturacion;
//kaw=0.3ki  control rapido algunas saturaciones;
//kaw=0.5ki  a 1.0ki control agresivo
	PID_DIR.Kaw=0.5*PID_DIR.Ki; //control saturacion integral

	PID_DIR.aw_term=0;   //ventana contorl de saturacion inetgral
   PID_Reset(&PID_DIR);  //RESETEA PARAMETROS
}
//colocar mas INIT por cada PID usado

float PID_Control(PID_ *pid, float input)
{
    pid->error = pid->setpoint - input;

        //  Parte proporcional 
        pid->up = pid->Kp * pid->error;

        //  Parte integral (Tustin) 
        pid->ui += (pid->Ki * pid->Ts * 0.5f) * (pid->error + pid->prev_error)
                       + pid->Kaw * pid->aw_term * pid->Ts;

        /*
         antes  clasico
        pid->integral +==pid->Ki*t_mues*error;
         */

        //  Parte derivativa filtrada (Tustin) 
        // D[k] = [ -2*Kd*(input - prev_input) + (2*tau - Ts)*D[k-1] ] / (2*tau + Ts)
        pid->ud = ( -2.0f * pid->Kd * (input - pid->prev_input)
                 + (2.0f * pid->Tau - pid->Ts) * pid->ud )
                 / (2.0f * pid->Tau + pid->Ts);

        pid->prev_input = input;

        //  salida PID 
        float output = pid->up  + pid->ui + pid->ud;

        //  Saturar 
        float output_sat = output;
        if (output_sat > pid->out_max) output_sat = pid->out_max;
        if (output_sat < pid->out_min) output_sat = pid->out_min;

//comentar si no se quiera usar  calculo saturacion
        pid->aw_term = output_sat - output;

        pid->prev_error = pid->error;
        pid->output = output_sat;

        return pid->output;

}

//
void PID_Reset(PID_ *pid)
{
    pid->error = 0.0f;
    pid->prev_error = 0.0f;
    pid->ui = 0.0f;
    pid->output = 0.0f;
    pid->aw_term = 0.0f;
    pid->prev_input = 0.0f;
    pid->ud = 0.0f;
}
