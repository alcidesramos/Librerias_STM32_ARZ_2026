/*
 * PMS7003.h
 *
 *  Created on: Dec 20, 2023
 *      Author: Alcides Ramos
 */

#ifndef LIBRERIAS_SENSORES_VARIOS_H_
#define LIBRERIAS_SENSORES_VARIOS_H_

#include "main.h"

void PMS7003_PASIVO();
void PMS7003_ACTIVO();
void PMS7003_READ();
//ZHO6
void ZHO6_PASIVO();
void ZHO6_ACTIVO();
void ZHO6_READ();
//CO2
void CO2_READ();
void CO2_PASIVO();
void CO2_ACTIVO();

//ALCOHOL
void ALCOHOL_PASIVO();
void ALCOHOL_ACTIVO();
void ALCOHOL_READ();



#endif /* LIBRERIAS_SENSORES_VARIOS_H_ */
