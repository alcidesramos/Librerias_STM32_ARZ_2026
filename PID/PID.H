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

typedef enum {
    PID_MODE_STANDARD = 0,   // PID clásico con integral y derivada filtrada
    PID_MODE_INCREMENTAL     // PID discreto incremental
} PID_Mode_t;

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
    float prev2_error;
    float integral;
    float output;

    // Derivada filtrada
    float prev_input;
    float D;

    // Anti-windup (back-calculation)
    bool anti_windup;
    float Kaw;       // constante de back-calculation
    float aw_term;

    // Modo
    PID_Mode_t mode;
} PID_t;

// Inicializa el PID
void PID_Init(PID_t *pid,
              float Kp,
              float Ki,
              float Kd,
              float Ts,
              float Tau,
              float out_min,
              float out_max,
              float setpoint,
              bool anti_windup,
              float Kaw,
              PID_Mode_t mode);

float PID_Compute(PID_t *pid, float input);
void PID_Reset(PID_t *pid);

#endif 
/*
// Parámetros del PID
    float Ts   = 0.02f;    // 20 ms
    float Tau  = 0.02f;    // Filtro derivativo (20 ms)
    float Kaw  = 800.0f;   // Constante de back-calculation

    PID_Init(&pid_velocidad,
             189.0f, 3517.0f, 0.09f,   // Kp, Ki, Kd
             Ts, Tau,
             1300.0f, 4200.0f,         // límites PWM
             0.0f,                     // setpoint inicial
             true, Kaw,                // anti-windup activado
             PID_MODE_STANDARD);       // Modo clásico con derivada filtrada



//se lalma asi
 pid_velocidad.setpoint = setpoint;

        float salida = PID_Compute(&pid_velocidad, entrada);
        PWM_valor(&PWM1, salida);

/* SRC_LIBRERIAS_PID_H_ */
