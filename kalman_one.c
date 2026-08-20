/*
 * kalman_one.c
 *
 *  Created on: May 10, 2025
 *      Author: ALCIDES_RAMOS
 */

#include "kalman_one.h"

KalmanFilter1  kalman_fil;
/*
  q ruido del proceso entre 0.001 y 1.0.   si cambia rapido mas alto
  r ruido de la medicion y el sensor cuanto varia  0.1 a 0.5


 */
// Inicializa el filtro
void kalman_one_init( KalmanFilter1 *kf, float q, float r, float initial_value) {
    kf->q = q;
    kf->r = r;
    kf->x = initial_value;
    kf->p = 1.0f;
    kf->k = 0.0f;
}

// Actualiza el filtro con una nueva medición
float kalman_one_update(KalmanFilter1 *kf, float measurement)
{
    // Predicción
    kf->p = kf->p + kf->q;

    // Actualización
    kf->k = kf->p / (kf->p + kf->r);
    kf->x = kf->x + kf->k * (measurement - kf->x);
    kf->p = (1.0f - kf->k) * kf->p;

    return kf->x;
}
