/*
 * filtro_iir.h
 *
 *  Created on: Jun 5, 2025
 *      Author: AveMaria
 */

#ifndef LIBRERIAS_FILTRO_IIR_H_
#define LIBRERIAS_FILTRO_IIR_H_



#endif /* LIBRERIAS_FILTRO_IIR_H_ */

#include "main.h"
#include <math.h>

typedef struct {
    float y_prev;
    float a;
} LowPassFilter;

extern LowPassFilter my_filter1;
extern LowPassFilter my_filter2;

void init_low_pass_filter(LowPassFilter* filter, float cutoff_hz, float sample_rate_hz);
float update_low_pass_filter(LowPassFilter* filter, float input);


