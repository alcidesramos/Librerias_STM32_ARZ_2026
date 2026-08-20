/*
 * kalman_one.h
 *
 *  Created on: May 10, 2025
 *      Author: ALCIDES_RAMOS
 */

#ifndef LIBRERIAS_KALMAN_ONE_H_
#define LIBRERIAS_KALMAN_ONE_H_

#include "main.h"
#include <stdio.h>
/*
//en el main llamar
  kalman_one_init(&kalman_fil, 0.01f, 0.1f, 0.0f);

  en el timer muestreo
  	setpoint= kalman_one_update(&kalman_fil,setpoint);



  */
typedef struct {
    float q;  // varianza del proceso (ruido del sistema)
    float r;  // varianza de la medición (ruido del sensor)
    float x;  // valor estimado
    float p;  // error de estimación
    float k;  // ganancia de Kalman
} KalmanFilter1;

extern  KalmanFilter1  kalman_fil;
void kalman_one_init(KalmanFilter1 *kf, float q, float r, float initial_value) ;
float kalman_one_update(KalmanFilter1 *kf, float measurement);



#endif /* LIBRERIAS_KALMAN_ONE_H_ */
