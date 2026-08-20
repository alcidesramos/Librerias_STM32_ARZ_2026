/*
 * HSCDANN001BA2A3.h
 *
 *  Created on: Jul 3, 2024
 *      Author: AveMaria
 */

#ifndef LIBRERIAS_HSCDANN001BA2A3_H_
#define LIBRERIAS_HSCDANN001BA2A3_H_
#include"main.h"


#endif /* LIBRERIAS_HSCDANN001BA2A3_H_ */

#define HSCDANN001BA2A3_DIR 0x28<<1

//define i2c a usar

#define HSCDANN001BA2A3_I2C hi2c2
extern  I2C_HandleTypeDef hi2c2;

# define SalidaMax 14745.0
#define SalidaMin 1638.0

#define PresionMax 100.0
#define PresionMin 0.169

void I2C_Write_n(uint8_t *bufft,uint8_t ndat);
void I2C_Read_n(uint8_t *buffr,uint8_t ndat);
void HSCDANN001BA2A3_read(float *presion, float *temperatura);
uint8_t HSCDANN001BA2A3_ready();


