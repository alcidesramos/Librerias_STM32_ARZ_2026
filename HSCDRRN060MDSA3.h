/*
 * HSCDRRN060MDSA3.h
 *
 *  Created on: Aug 10, 2024
 *      Author: AveMaria
 */

#ifndef LIBRERIAS_HSCDRRN060MDSA3_H_
#define LIBRERIAS_HSCDRRN060MDSA3_H_
#include"main.h"


#endif /* LIBRERIAS_HSCDRRN060MDSA3_H_ */

void SPI_Read_n(uint8_t *bufft,uint8_t ndat);
uint16_t ayuda();

//void SPI_Read_n(uint8_t *bufft,uint8_t ndat)

void HSCDRRN060MDSA3_read(float *presion, float *temperatura);

#define SalidaMax 14746.0
#define SalidaMin 1638.0

#define PresionMax 6.0  // en pascales
#define PresionMin -6.0
