/*
 * Filtro_exponencial.c
 *
 *  Created on: Jun 9, 2025
 *      Author: ALCIDES_RAMOS
 */

#include "Filtro_exponencial.h"
   float fil_out = 0.0;

float Filtro_Exp(float fil_input)
{

	 fil_out = alpha * fil_input + (1 - alpha) *  fil_out;
    return  fil_out;
}
