/*
 * RainfallSensor.h
 *
 *  Created on: Jun 19, 2024
 *      Author: Alcides Ramos
 */

#ifndef LIBRERIAS_RAINFALLSENSOR_H_
#define LIBRERIAS_RAINFALLSENSOR_H_

#define RainfallSensor_ADDR 0x1D<<1

//EL I2C A USAR
extern I2C_HandleTypeDef hi2c1;
#define RainfallSensor_I2C hi2c1

    //registros del sensor
    #define I2C_REG_PID                            0x00
    #define I2C_REG_VID                            0x02
    #define I2C_REG_VERSION                        0x0A
    #define I2C_REG_TIME_RAINFALL                  0x0C
    #define I2C_REG_CUMULATIVE_RAINFALL            0x10
    #define I2C_REG_RAW_DATA                       0x14
    #define I2C_REG_SYS_TIME                       0x18
    #define I2C_REG_RAIN_HOUR                      0x26
    #define I2C_REG_BASE_RAINFALL                  0x28

//procedimientos a usar
uint8_t RainfallSensor_getPidVid();
void RainfallSensor_Reset();
float RainfallSensor_getRainfall();
float RainfallSensor_getRainfall_h(uint8_t hour);
uint32_t RainfallSensor_getRawData();
uint8_t RainfallSensor_setRainAccumulatedValue(float value);
float RainfallSensor_getSensorWorkingTime();

#endif /* LIBRERIAS_RAINFALLSENSOR_H_ */
