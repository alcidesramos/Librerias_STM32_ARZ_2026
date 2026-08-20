/*
 * kalman.h
 *
 *  Created on: May 2, 2025
 *      Author: ALCIDES_RAMOS
 */

#ifndef LIBRERIAS_KALMAN_H_
#define LIBRERIAS_KALMAN_H_
#include "main.h"

typedef struct
{    float angle;     // Ángulo estimado
    float bias;      // Sesgo del giroscopio
    float P[2][2];   // Matriz de covarianza del error
} Kalman_f;

extern  Kalman_f  kalman_roll_;
extern  Kalman_f  kalman_pitch_;
extern float kalman_roll,kalman_pitch;

/*
como se usa
en el main antes del while
  Kalman_Init(&kalman_roll_);
  Kalman_Init(&kalman_pitch_);

  en el  .c del imu declarar
  void  filtro_kalman()
{
	kalman_roll=Kalman_Update(&kalman_roll_, acel_roll, imu.giro_x_f, muestreo);
	kalman_pitch=Kalman_Update(&kalman_pitch_, acel_pitch, imu.giro_y_f, muestreo);
}

//  en la int de timer llamar
  filtro_kalman();
 *

 */



void Kalman_Init(Kalman_f  *kalman);
float Kalman_Update(Kalman_f *kalman, float acc_angle, float gyro_rate, float dt);



#endif /* LIBRERIAS_KALMAN_H_ */
