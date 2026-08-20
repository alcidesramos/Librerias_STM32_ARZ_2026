/*
 * filtro_iir.c
 *
 *  Created on: Jun 5, 2025
 *      Author: AveMaria
 */

#include "filtro_iir.h"
LowPassFilter my_filter1;
LowPassFilter my_filter2;


void init_low_pass_filter(LowPassFilter* filter, float cutoff_hz, float sample_rate_hz)
{
    float tau = 1.0f / (2.0f * M_PI * cutoff_hz);
    float dt = 1.0f / sample_rate_hz;
    filter->a = dt / (tau + dt);
    filter->y_prev = 0.0f;
}

float update_low_pass_filter(LowPassFilter* filter, float input)
{
    float y = filter->a * input + (1.0f - filter->a) * filter->y_prev;
    filter->y_prev = y;
    return y;
}


