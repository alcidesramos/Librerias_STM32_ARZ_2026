/*
 * Xbus.cpp
 *
 *  Created on: Jul 14, 2023
 *      Author: Administrator
 */

#include "Xbus.h"
#include <string.h>
#include <stdio.h>
#include "mydebug.h"

#define TIMEOUT_MS 100
#define RADTODEG 57.295779513082320876798154814105	// (180.0/pi)

void Xbus_init(Xbus* self, I2C_HandleTypeDef *hi2c){
	self->_hi2c = hi2c;
	for (int i = 0; i < 3; ++i) {
		self->acc[i] = NAN;
		self->rot[i] = NAN;
		self->euler[i] = NAN;
		self->mag[i] = NAN; // initialize mag
	}
	for (int i = 0; i < 2; ++i) {
		self->latlon[i] = NAN;
	}
	self->sampleTimeFine = 0; // initialize sampleTimeFine
	self->statusWord = 0; // initialize status
}



bool Xbus_read(Xbus* self, uint8_t address) {
	Xbus_readPipeStatus(self, address);
	//HAL_Delay(250);
	if (self->notificationSize) { // New notification message available to be read
		Xbus_readPipeNotif(self, address);
		Xbus_parseNotification(self, self->datanotif);

		// Print the notification data
		//printDebugHexString("Xbus.c line 35: Received Notification data(HEX): 0x%s\n", self->datanotif, self->notificationSize);
	}
	if (self->measurementSize) { // New measurement packet available to be read
		Xbus_readPipeMeas(self, address);
		Xbus_parseMTData2(self, self->datameas, self->measurementSize);

		// Print the measurement data
		//printDebugHexString("Xbus.c line 42: Received Measurement data(HEX): 0x%s\n", self->datameas, self->measurementSize);

		return true; // Return true if new measurements were read
	} else {
		return false;
	}
}


void Xbus_readPipeStatus(Xbus* self, uint8_t address) {
	uint8_t buf[1] = {XSENS_STATUS_PIPE};
	HAL_StatusTypeDef status;
	status = HAL_I2C_Master_Transmit(self->_hi2c, address << 1, buf, 1, TIMEOUT_MS);
	if (status != HAL_OK) {
		printDebug("Xbus.c line 56, Xbus_readPipeStatus:HAL_I2C_Master_Transmit failed: %d", status);
	}
	printDebug("Xbus.c line 58: Send XSENS_STATUS_PIPE OpCode(HEX): 0x%02X\n", buf[0]);



	HAL_I2C_Master_Receive(self->_hi2c, address << 1, self->status, 4, TIMEOUT_MS);


	self->notificationSize = (uint16_t)self->status[0] | ((uint16_t)self->status[1] << 8);
	self->measurementSize = (uint16_t)self->status[2] | ((uint16_t)self->status[3] << 8);
	printDebug("Xbus.c line 65, Received notificationSize(decimal): %d", self->notificationSize);
	printDebug("Xbus.c line 66, Received measurementSize(decimal): %d", self->measurementSize);
}


void Xbus_readPipeNotif(Xbus* self, uint8_t address) {
	printDebug("Xbus.c line 73, Read NotificationPipe XSENS_NOTIF_PIPE(0x05)");
	if(self->notificationSize > 0) {
		HAL_StatusTypeDef status = HAL_I2C_Mem_Read(self->_hi2c, address << 1, XSENS_NOTIF_PIPE, I2C_MEMADD_SIZE_8BIT, self->datanotif, self->notificationSize, TIMEOUT_MS);
		if(status != HAL_OK) {
			printDebug("Xbus.c line 76, I2C Receive Error: %d", status);
		}
	}
}



void Xbus_readPipeMeas(Xbus* self, uint8_t address) {
	printDebug("Xbus.c line 86, Read MeasurementPipe XSENS_MEAS_PIPE(0x06)");
	if(self->measurementSize > 0) {
		HAL_StatusTypeDef status = HAL_I2C_Mem_Read(self->_hi2c, address << 1, XSENS_MEAS_PIPE, I2C_MEMADD_SIZE_8BIT, self->datameas, self->measurementSize, TIMEOUT_MS);
		if(status != HAL_OK) {
			printDebug("Xbus.c, line 95: I2C Receive Error: %d", status);
		}
	}
}





void Xbus_parseMTData2(Xbus* self, uint8_t* data, uint8_t datalength) {
	if (datalength < 2)                                                           //Reached the end of the MTData2 message
		return;

	uint8_t XDI = data[0] ;                                                       //Xsens Data Identifier
	if (XDI == (uint8_t)MTDATA2) {                                         //Start of the MTData2 message
		uint8_t length = data[1];
		if (length + 2 <= datalength) {
			Xbus_parseMTData2(self, data + 2, length);
		}
	} else {
		uint8_t length = data[2];
		uint16_t dataIdentifier = ((uint16_t)data[1] | ((uint16_t)data[0] << 8)) & (uint16_t)0xFFFF;  // Extract the 2-byte Xsens Data Identifier
//		printDebug("Xbus.c line 116: current dataIdentifier: 0x%02X", dataIdentifier);
		if (length + 3 <= datalength) {  // Ensure we have enough data
			char buf[50];
			switch (dataIdentifier) {
			case (uint16_t)EULERANGLES:
					Xbus_dataswapendian(self, data + 3, sizeof(float) * 3);
			memcpy(self->euler, data + 3, sizeof(float) * 3);
			printDebug("Euler angles [deg]:");
			for (int i = 0 ; i < 3; ++i) {
				snprintf(buf, sizeof(buf), "%.2f", self->euler[i]);
				printDebug("%s", buf);
			}
			printDebug("");
			break;

			case (uint16_t)ACCELERATION:
					Xbus_dataswapendian(self, data + 3, sizeof(float) * 3);
			memcpy(self->acc, data + 3, sizeof(float) * 3);
			printDebug("Acceleration [m/s^2]:");
			for (int i = 0 ; i < 3; ++i) {
				snprintf(buf, sizeof(buf), "%.2f", self->acc[i]);
				printDebug("%s", buf);
			}
			printDebug("");
			break;

			case (uint16_t)RATEOFTURN:
					Xbus_dataswapendian(self, data + 3, sizeof(float) * 3);
			memcpy(self->rot, data + 3, sizeof(float) * 3);
			printDebug("Rate of turn [deg/s]:");
			for (int i = 0 ; i < 3; ++i) {
				snprintf(buf, sizeof(buf), "%.2f", RADTODEG*self->rot[i]);
				printDebug("%s", buf);
			}
			printDebug("");
			break;

			case (uint16_t)LATLON:
					Xbus_dataswapendian(self, data + 3, sizeof(float) * 2);
			memcpy(self->latlon, data + 3, sizeof(float) * 2);
			printDebug("Latitude/Longitude [deg]:");
			for (int i = 0 ; i < 2; ++i) {
				snprintf(buf, sizeof(buf), "%.5f", self->latlon[i]);
				printDebug("%s", buf);
			}
			printDebug("");
			break;

			case (uint16_t)SAMPLETIMEFINE:
					Xbus_dataswapendian(self, data + 3, sizeof(uint32_t));
			memcpy(&self->sampleTimeFine, data + 3, sizeof(uint32_t));
			snprintf(buf, sizeof(buf), "Sample Time Fine [0.1ms]: %u", self->sampleTimeFine);
			printDebug("%s", buf);
			printDebug("");
			break;

			case (uint16_t)MAGNETICFIELD:
					Xbus_dataswapendian(self, data + 3, sizeof(float) * 3);
			memcpy(self->mag, data + 3, sizeof(float) * 3);
			printDebug("Magnetic Field [a.u.]:");
			for (int i = 0 ; i < 3; ++i) {
				snprintf(buf, sizeof(buf), "%.2f", self->mag[i]);
				printDebug("%s", buf);
			}
			printDebug("");
			break;

			case (uint16_t)STATUSWORD:
					Xbus_dataswapendian(self, data + 3, sizeof(uint32_t));
			memcpy(&self->statusWord, data + 3, sizeof(uint32_t));
			snprintf(buf, sizeof(buf), "Status Word: 0x%08X", self->statusWord);
			printDebug("%s", buf);
			printDebug("");
			break;

			default:
				break;
			}

			if (length + 3 + 3 <= datalength) {
				Xbus_parseMTData2(self, data + length + 3, datalength - length - 3);
			}                     //Move onto next data element within MTData2 packet

		}


	}
}



void Xbus_parseNotification(Xbus* self, uint8_t* notif) {                                           //Parse the most common notification messages
	uint8_t notifID = notif[0];
	switch (notifID) {
	case (uint8_t)WAKEUP: {
		printDebug("Xbus.c line 151:Received WakeUp(0x3E) message.");
		break;
	}
	case (uint8_t)XSERROR: {
		printDebug("Xbus.c line 155:Received an error with code(HEX): 0x%02X", notif[2]);
		break;
	}
	case (uint8_t)WARNING: {
		uint32_t warn = (uint32_t)notif[5] | ((uint32_t)notif[4] << 8);
		printDebug("Xbus.c line 161:Received a warning with code: %lu", warn);
		break;
	}
	case (uint8_t)PRODUCTCODE: {
		printDebugCharArray("Xbus.c line 183:Product code is: %s", (char*)(notif+2), self->notificationSize -3);
		self->productCode = notif[6];                                               //Store the product code (MTi-#) for later usage
		break;
	}
	case (uint8_t)FIRMWAREREV: {
		printDebug("Xbus.c line 177:Firmware version is: ");
		printDebug("%d.%d.%d", notif[2], notif[3], notif[4]);
		break;
	}
	case (uint8_t)GOTOCONFIGACK: {
		printDebug("Xbus.c line 183:Received GoToConfigACK(0x31).");
		self->configState = true;
		break;
	}
	case (uint8_t)GOTOMEASUREMENTACK: {
		printDebug("Xbus.c line 188:Received GoToMeasurementACK(0x11).");
		self->configState = false;
		break;
	}
	case (uint8_t)OUTPUTCONFIGURATION: {
		printDebug("Xbus.c line 193:Received SetOutputConfigurationACK(0xC1).");
		break;
	}
	default: {
		printDebugHexString("Xbus.c line 197:Received undefined notification: 0x%s", notif, self->notificationSize - 1);
		printDebug("\n");
		break;
	}
	}
}


void Xbus_dataswapendian(Xbus* self, uint8_t* data, uint8_t length) {                          //Swap the endianness of the data such that the float value can be printed
	uint8_t cpy[length];                                                              //Create a copy of the data
	memcpy(cpy, data, length);                                                        //Create a copy of the data
	for (int i = 0; i < length / 4; i++) {
		for (int j = 0; j < 4; j++) {
			data[j + i * 4] = cpy[3 - j + i * 4];                                         //Within each 4-byte (32-bit) float, reverse the order of the individual bytes
		}
	}
}


