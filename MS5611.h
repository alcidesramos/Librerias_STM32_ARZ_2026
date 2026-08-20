/*
 * MS5611.h
 *
 *  Created on: May 4, 2024
 *      Author: Alcides Ramos
 */

#ifndef LIBRERIAS_MS5611_H_
#define LIBRERIAS_MS5611_H_
#include "main.h"
#include "stdint.h"
#include "stdbool.h"

	// 	// 0x76 si CSB GND    0x77 si CSB VDD
#define MS5611_DIR 0x77<<1
//define i2c a usar
#define MS5611_I2C hi2c1
extern  I2C_HandleTypeDef hi2c1;


#define MS5611_CMD_READ_ADC       0x00
#define MS5611_CMD_READ_PROM      0xA0
#define MS5611_CMD_RESET          0x1E
#define MS5611_CMD_CONVERT_D1     0x40
#define MS5611_CMD_CONVERT_D2     0x50
#define MS5611_READ_OK                        0
#define MS5611_ERROR_2                        2         //  low level I2C error
#define MS5611_NOT_READ                       -999

#define OSR_ULTRA_HIGH   12        // 10 millis
#define  OSR_HIGH        11        //  5 millis
#define OSR_STANDARD     10        //  3 millis
#define OSR_LOW          9         //  2 millis
#define OSR_ULTRA_LOW     8          //  1 millis

//procedimientos

uint8_t MS5611_Init();
void initConstants(uint8_t mathMode);
int command(const uint8_t command);
uint16_t readProm(uint8_t reg);
bool MS5611_reset(uint8_t mathMode);
void MS5611_convert(const uint8_t addr, uint8_t bits);
uint32_t MS5611_readADC();
int MS5611_read(uint8_t bits);
void MS5611_setOversampling(uint8_t samplingRate);
float MS5611_getTemperature();
float MS5611_getPressure() ;
uint16_t MS5611_getManufacturer();
uint16_t MS5611_getSerialCode();
float MS5611_getaltura(float pressure_ambient,float temp);
float calc_altitude_pressure(float pressure_ambient,float temp);

#endif /* LIBRERIAS_MS5611_H_ */
