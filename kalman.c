/*
 * kalman.c
 *
 *  Created on: May 2, 2025
 *      Author: ALCIDES_RAMOS
 */


#include "kalman.h"


Kalman_f  kalman_roll_;
Kalman_f  kalman_pitch_;
float kalman_roll,kalman_pitch;


void Kalman_Init(Kalman_f  *kalman)
{
    kalman->angle = 0.0f;
    kalman->bias = 0.0f;
    kalman->P[0][0] = 1.0f;
    kalman->P[0][1] = 0.0f;
    kalman->P[1][0] = 0.0f;
    kalman->P[1][1] = 1.0f;
}

float Kalman_Update(Kalman_f *kalman, float acc_angle, float gyro_rate, float dt) {


/*Rangos
  Q_angle: 0.001 – 0.01  //mas alto  reaciona  mas rapido a cambio pero ams ruido
  Q_bias: 0.001 – 0.01  //mas alto confio mas en el giro
 R_measure: 0.01 – 0.1  //entre mas bajo confio mas en el acelelometro
  */

	// Parámetros del modelo
    //float Q_angle = 0.008f;  // Ruido del proceso (para el ángulo)
    //float Q_bias  = 0.003f;  // Ruido del proceso (para el sesgo del gyro)
    //float R_measure = 0.01f; // Ruido de la medida (del acelerómetro)

	float Q_angle = 0.001f;  // Ruido del proceso (para el ángulo)
	float Q_bias  = 0.001f;  // Ruido del proceso (para el sesgo del gyro)
	float R_measure = 0.05f; // Ruido de la medida (del acelerómetro)

    // Predicción
    float rate = gyro_rate - kalman->bias;
    kalman->angle += dt * rate;

    // Actualización de matriz de error P
    kalman->P[0][0] += dt * (dt*kalman->P[1][1] - kalman->P[0][1] - kalman->P[1][0] + Q_angle);
    kalman->P[0][1] -= dt * kalman->P[1][1];
    kalman->P[1][0] -= dt * kalman->P[1][1];
    kalman->P[1][1] += Q_bias * dt;

    // Corrección
    float y = acc_angle - kalman->angle;
    float S = kalman->P[0][0] + R_measure;
    float K0 = kalman->P[0][0] / S;
    float K1 = kalman->P[1][0] / S;

    kalman->angle += K0 * y;
    kalman->bias  += K1 * y;

    float P00_temp = kalman->P[0][0];
    float P01_temp = kalman->P[0][1];

    kalman->P[0][0] -= K0 * P00_temp;
    kalman->P[0][1] -= K0 * P01_temp;
    kalman->P[1][0] -= K1 * P00_temp;
    kalman->P[1][1] -= K1 * P01_temp;

    return kalman->angle;
}
