/*
 * PID.c
 *  Created on: Nov 2, 2024
 * Autor: Alcides Ramos 
 */

#include "PID.h"

//
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
              PID_Mode_t mode)
{
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->Ts = Ts;
    pid->Tau = Tau;

    pid->out_min = out_min;
    pid->out_max = out_max;
    pid->setpoint = setpoint;

    pid->error = 0.0f;
    pid->prev_error = 0.0f;
    pid->prev2_error = 0.0f;
    pid->integral = 0.0f;
    pid->output = 0.0f;

    pid->prev_input = 0.0f;
    pid->D = 0.0f;

    pid->anti_windup = anti_windup;
    pid->Kaw = Kaw;
    pid->aw_term = 0.0f;
    pid->mode = mode;
}

//
float PID_Compute(PID_t *pid, float input)
{
    pid->error = pid->setpoint - input;

    // 
    //  MODO ESTÁNDAR (PID clásico)
    // 
    if (pid->mode == PID_MODE_STANDARD)
    {
        //  Parte proporcional 
        float P = pid->Kp * pid->error;

        //  Parte integral (Tustin) 
        pid->integral += (pid->Ki * pid->Ts * 0.5f) * (pid->error + pid->prev_error)
                       + pid->Kaw * pid->aw_term * pid->Ts;

        //  Parte derivativa filtrada (Tustin) 
        // D[k] = [ -2*Kd*(input - prev_input) + (2*tau - Ts)*D[k-1] ] / (2*tau + Ts)
        pid->D = ( -2.0f * pid->Kd * (input - pid->prev_input)
                 + (2.0f * pid->Tau - pid->Ts) * pid->D )
                 / (2.0f * pid->Tau + pid->Ts);

        pid->prev_input = input;

        //  salida PID 
        float output = P + pid->integral + pid->D;

        //  Saturar 
        float output_sat = output;
        if (output_sat > pid->out_max) output_sat = pid->out_max;
        if (output_sat < pid->out_min) output_sat = pid->out_min;

        //  Anti-windup 
        if (pid->anti_windup)
            pid->aw_term = output_sat - output;
        else
            pid->aw_term = 0.0f;

        pid->prev_error = pid->error;
        pid->output = output_sat;
        return pid->output;
    }

   
    //  MODO INCREMENTAL (discreto optimizado)
   
    else if (pid->mode == PID_MODE_INCREMENTAL)
    {
        // Δu[k] = Kp*(e[k]-e[k-1]) + Ki*Ts/2*(e[k]+e[k-1]) + Kd/Ts*(e[k]-2e[k-1]+e[k-2])
        float delta_u =
            pid->Kp * (pid->error - pid->prev_error) +
            pid->Ki * pid->Ts * 0.5f * (pid->error + pid->prev_error) +
            pid->Kd / pid->Ts * (pid->error - 2.0f * pid->prev_error + pid->prev2_error);

        float output = pid->output + delta_u;

        // Saturar
        float output_sat = output;
        if (output_sat > pid->out_max) output_sat = pid->out_max;
        if (output_sat < pid->out_min) output_sat = pid->out_min;

        // Anti-windup
        if (pid->anti_windup)
            pid->aw_term = output_sat - output;
        else
            pid->aw_term = 0.0f;

        // Actualizar históricos
        pid->prev2_error = pid->prev_error;
        pid->prev_error = pid->error;
        pid->output = output_sat;

        return pid->output;
    }

    return pid->output;
}

//
void PID_Reset(PID_t *pid)
{
    pid->error = 0.0f;
    pid->prev_error = 0.0f;
    pid->prev2_error = 0.0f;
    pid->integral = 0.0f;
    pid->output = 0.0f;
    pid->aw_term = 0.0f;
    pid->prev_input = 0.0f;
    pid->D = 0.0f;
}
