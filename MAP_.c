/*
 * MAP.C
 *
 *  Created on: Jun 1, 2025
 *      Author: ALCIDES_RAMOS
 */
#include "MAP_.h"

float map(float x_in, float x1_, float y1_, float x2_, float y2_)
{
float pendiente;
  pendiente=(y2_ - y1_) / (x2_ - x1_);
  pendiente=pendiente*(x_in- x1_);
  pendiente=pendiente+y1_;
  return( pendiente);
}


