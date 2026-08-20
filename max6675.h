/*
 * max6675.h
 *
 *  Created on: Apr 9, 2023
 *      Author: Alcides Ramos
 */

#ifndef LIBRERIAS_MAX6675_H_
#define LIBRERIAS_MAX6675_H_

#include "main.h"
#include "delays.h"

void MAX6675_init();
uint8_t MAX6675_pulso();
float MAX6675_lee();

#endif /* LIBRERIAS_MAX6675_H_ */
