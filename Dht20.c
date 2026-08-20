/*
 * am2320.c adapted to DHT20
 *
 *  Created on: 04.08.2019
 *      Author: SimpleMethod
 *  Updated on: 01.10.2020
 *  Adapted on: 16.04.2023
 *      Author: Arkrhiss
 *Copyright 2019 SimpleMethod
 *All credits for SimpleMethod
 *Permission is hereby granted, free of charge, to any person obtaining a copy of
 *this software and associated documentation files (the "Software"), to deal in
 *the Software without restriction, including without limitation the rights to
 *use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 *of the Software, and to permit persons to whom the Software is furnished to do
 *so, subject to the following conditions:
 *
 *The above copyright notice and this permission notice shall be included in all
 *copies or substantial portions of the Software.
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *THE SOFTWARE.
 ******************************************************************************
 */


#include "Dht20.h"

Dht20_HandleTypeDef Dht20_Init(I2C_HandleTypeDef* i2c_handle,uint8_t device_address) {
	Dht20_HandleTypeDef Dht20_;
	Dht20_.i2c_handle = i2c_handle;
	Dht20_.device_address = device_address;
	return Dht20_;
}

uint8_t Dht20_ReadValue(Dht20_HandleTypeDef *Dht20) {
	uint8_t registers[3] = {0xac,0x33,0x00};
	HAL_I2C_Master_Transmit( (Dht20->i2c_handle), (Dht20->device_address), 0x71, 0, HAL_MAX_DELAY);
	HAL_Delay(10);
	if (HAL_I2C_Master_Transmit(Dht20->i2c_handle, Dht20->device_address,registers, 3, HAL_MAX_DELAY) != HAL_OK) {
		return 1;
	}
	HAL_Delay(80);
	if (HAL_I2C_Master_Receive(Dht20->i2c_handle, Dht20->device_address, Dht20->data, 8, HAL_MAX_DELAY) != HAL_OK) {
		return 2;
	}
	if (Dht20->data[1] != 0x00 && Dht20->data[0] != 0xac) {
		return 3;
	}
	return 0;
}

void Dht20_GetTemperatureAndHumidity(Dht20_HandleTypeDef *Dht20, float *temperature,float *humidity) {

	uint16_t temp_temperature = ((Dht20->data[5]) | (Dht20->data[4] << 8) | ((Dht20->data[3] << 16) & 0xf));

	uint16_t temp_humidity = ((Dht20->data[3] & 0xf0) >> 4 | Dht20->data[2] << 4 | Dht20->data[1] << 12);

	*temperature = (float) temp_temperature * 50/8192 -100;
	*humidity = (float) temp_humidity * 25/16384;
}


