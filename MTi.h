/*
 * MTi.h
 *
 *  Created on: Jul 14, 2023
 *      Author: Administrator
 */

#ifndef SRC_MTI_H_
#define SRC_MTI_H_

#include <stdlib.h>
#include "Xbus.h"
#include "main.h"

//colocar i2c a usar
#define  IMU_I2C   hi2c3


#define MTII2CADDRESS 0x6B


typedef struct {
    GPIO_TypeDef* drdy_port;
    uint16_t drdy_pin;
    uint16_t address;
    Xbus xbus;
} MTi;

void MTi_init(MTi* mti, uint16_t x, GPIO_TypeDef* y_port, uint16_t y_pin);
bool MTi_detect(MTi* mti, uint32_t timeout);
void MTi_requestDeviceInfo(MTi* mti);
void MTi_configureOutputs(MTi* mti);
void MTi_goToConfig(MTi* mti);
void MTi_goToMeasurement(MTi* mti);
void MTi_readMessages(MTi* mti);
void MTi_sendMessage(MTi* mti, uint8_t *message, uint8_t numBytes);
float* MTi_getEulerAngles(MTi* mti);
float* MTi_getAcceleration(MTi* mti);
float* MTi_getRateOfTurn(MTi* mti);
float* MTi_getLatLon(MTi* mti);
uint32_t MTi_getSampleTimeFine(MTi* mti);
float* MTi_getMagneticField(MTi* mti);
uint32_t MTi_getStatusWord(MTi* mti);


#endif /* SRC_MTI_H_ */
