/*
 * MTi.cpp
 *
 *  Created on: Jul 14, 2023
 *      Author: Administrator
 */

#include "MTi.h"
#include <math.h>
#include <stdio.h>
#include "mydebug.h"





extern I2C_HandleTypeDef IMU_I2C;  // Add an external declaration for the I2C handler. Make sure the I2C handler you are using in your main program is "IMU_I2C", otherwise you need to change it.



void MTi_init(MTi* mti, uint16_t x, GPIO_TypeDef* y_port, uint16_t y_pin) {
	mti->address = x;
	mti->drdy_port = y_port;
	mti->drdy_pin = y_pin;
	Xbus_init(&(mti->xbus), &IMU_I2C);
}



bool MTi_detect(MTi* mti, uint32_t timeout) {
	//Send goToConfig messages until goToConfigAck is received from the device
	printDebug("Scanning for MTi.");
	long int starttime = HAL_GetTick();
	while ((HAL_GetTick() - starttime) < timeout) {
		printDebug("MTi.c line 32: MTi_detect: Go To Config.");
		MTi_goToConfig(mti);
		HAL_Delay(250);

		MTi_readMessages(mti);
		if (mti->xbus.configState) {
			printDebug("Device detected.");
			return true;
		}
	}
	printDebug("Failed to detect device.");
	return false;
}


void MTi_configureOutputs(MTi* mti) {
	//Configure the outputs of the MTi using Xbus commands. Refer to the MT Low Level Communication Protocol Document for more information on the commands used here:
	//https://mtidocs.xsens.com/mt-low-level-communication-protocol-documentation

	if (!mti->xbus.configState) {
		MTi_goToConfig(mti);
		HAL_Delay(1000);
	}
	MTi_readMessages(mti);                                                                                                //Clear the measurement/notification pipes (without printing) before configuring.


	if (mti->xbus.productCode == '1') {//MTi-1 IMU
		printDebug("Configuring Acceleration/RateOfTurn at 1 Hz.");
		uint8_t outputConfig[] = {0xC0, 0x08, 0x40, 0x20, 0x00, 0x01, 0x80, 0x20, 0x00, 0x01};    //setOutputConfiguration Xbus message with Data fields 0x40 0x20 0x00 0x01 / 0x40 0x20 0x00 0x01 (Acceleration/RateOfTurn, 32-bit float, at 1 Hz)
		//printDebug("Configuring Acceleration/RateOfTurn at 10 Hz.");
		//uint8_t outputConfig[] = {0xC0, 0x08, 0x40, 0x20, 0x00, 0x0A, 0x80, 0x20, 0x00, 0x0A};  //setOutputConfiguration Xbus message with Data fields 0x40 0x20 0x00 0x01 / 0x40 0x20 0x00 0x01 (Acceleration/RateOfTurn, 32-bit float, at 10 Hz)
		MTi_sendMessage(mti, outputConfig, sizeof(outputConfig));

	} else if ((mti->xbus.productCode == '2') | (mti->xbus.productCode == '3')) {//"MTi-2 VRU or MTi-3 AHRS
//		printDebug("Configuring Euler angles at 1 Hz.");
//		uint8_t outputConfig[] = {0xC0, 0x04, 0x20, 0x30, 0x00, 0x01};                            //setOutputConfiguration Xbus message with Data field 0x20 0x30 0x00 0x01 (EulerAngles, 32-bit float, at 1 Hz)
//		printDebug("Configuring Euler angles at 10 Hz.");
		//uint8_t outputConfig[] = {0xC0, 0x04, 0x20, 0x30, 0x00, 0x0A};                          //setOutputConfiguration Xbus message with Data field 0x20 0x30 0x00 0x01 (EulerAngles, 32-bit float, at 10 Hz)


		//C0 18 10 60 00 01 20 30 00 01 80 20 00 01 40 20 00 01 C0 20 00 01 E0 20 00 01 83
		printDebug("Configuring SampleTimeFine/Euler/RateOfTurn/Acc/Mag/Status at 1 Hz.");
		uint8_t outputConfig[] = {0xC0,0x18,0x10,0x60,0x00,0x01,0x20,0x30,0x00,0x01,0x80,0x20,0x00,0x01,0x40,0x20,0x00,0x01,0xC0,0x20,0x00,0x01,0xE0,0x20,0x00,0x01,0x83};
		//C0 18 10 60 00 64 20 30 00 64 80 20 00 64 40 20 00 64 C0 20 00 64 E0 20 00 64 31
//		printDebug("Configuring SampleTimeFine/Euler/RateOfTurn/Acc/Mag/Status at 100 Hz.");
//				uint8_t outputConfig[] = {0xC0, 0x18, 0x10, 0x60, 0x00, 0x64, 0x20, 0x30, 0x00, 0x64, 0x80, 0x20, 0x00, 0x64, 0x40, 0x20, 0x00, 0x64, 0xC0, 0x20, 0x00, 0x64, 0xE0, 0x20, 0x00, 0x64, 0x31};
		MTi_sendMessage(mti, outputConfig, sizeof(outputConfig));

	} else if ((mti->xbus.productCode == '7') | (mti->xbus.productCode == '8')) {//"MTi-7 or MTi-8 GNSS/INS
		printDebug("Configuring Euler Angles and Latitude/Longitude at 1 Hz.");
		uint8_t outputConfig[] = {0xC0, 0x08, 0x50, 0x40, 0x00, 0x01, 0x20, 0x30, 0x00, 0x01};    //setOutputConfiguration Xbus message with Data fields 0x50 0x40 0x00 0x01 / 0x20 0x30 0x00 0x01 (LatLon/EulerAngles, 32-bit float, at 1 Hz)
		//printDebug("Configuring Euler Angles and Latitude/Longitude at 10 Hz.");
		//uint8_t outputConfig[] = {0xC0, 0x08, 0x50, 0x40, 0x00, 0x0A, 0x20, 0x30, 0x00, 0x0A};  //setOutputConfiguration Xbus message with Data fields 0x50 0x40 0x00 0x01 / 0x20 0x30 0x00 0x01 (LatLon/EulerAngles, 32-bit float, at 10 Hz)
		MTi_sendMessage(mti, outputConfig, sizeof(outputConfig));

	} else {
		printDebug("Could not configure device. Device's product code is unknown.");
	}
	HAL_Delay(100);
	MTi_readMessages(mti);

}


void MTi_requestDeviceInfo(MTi* mti) {
	//Request device info from the MTi using Xbus commands. Refer to the MT Low Level Communication Protocol Document for more information on the commands used here:
	//https://mtidocs.xsens.com/mt-low-level-communication-protocol-documentation

	if (!mti->xbus.configState) {
		MTi_goToConfig(mti);
		HAL_Delay(1000);
	}
	MTi_readMessages(mti);                                                                                                       //Clear the measurement/notification pipes (without printing) before configuring.
	printDebug("Requesting device info...");

	uint8_t reqProductCode[] = {0x1C, 0x00};                                                    //reqProductCode Xbus message
	MTi_sendMessage(mti, reqProductCode, sizeof(reqProductCode));
	HAL_Delay(1000);

	MTi_readMessages(mti);

	uint8_t reqFWRev[] = {0x12, 0x00};                                                          //reqFWRev Xbus message
	MTi_sendMessage(mti, reqFWRev, sizeof(reqFWRev));
	HAL_Delay(1000);

	MTi_readMessages(mti);
}


void MTi_goToConfig(MTi* mti) {
	printDebug("Entering configuration mode.");
	uint8_t goToConfig[] = {0x30, 0x00};
	//goToConfig Xbus message
	MTi_sendMessage(mti, goToConfig, sizeof(goToConfig));
}


void MTi_goToMeasurement(MTi* mti) {
	printDebug("Entering measurement mode.");
	uint8_t goToMeas[] = {0x10, 0x00};                                                          //goToMeasurement Xbus message
	MTi_sendMessage(mti, goToMeas, sizeof(goToMeas));
}


void MTi_sendMessage(MTi* mti, uint8_t *message, uint8_t numBytes) {
	//Compute the checksum for the Xbus message to be sent. See https://mtidocs.xsens.com/messages for details.
	uint8_t checksum = 0x01;
	for (int i = 0; i < numBytes; i++) {
		checksum -= message[i];
	}
	message[numBytes] = checksum;
	//    numBytes++;

	//---------------Debug ONLY---------------------
	printDebug("numBytes of Xbus message: %d", numBytes+1);
	printDebugHexString("Sending reduced Xbus message(HEX): 0x%s", message, numBytes+1);

	//---------------Debug ONLY---------------------

	// Create a combined buffer
	uint8_t combined_buf[numBytes + 2];
	combined_buf[0] = XSENS_CONTROL_PIPE;
	memcpy(&combined_buf[1], message, numBytes + 1);
	printDebugHexString("Sending message with OpCode(HEX): 0x%s", combined_buf, numBytes+2);

	// Send the combined buffer
	HAL_StatusTypeDef status;
	status = HAL_I2C_Master_Transmit(&IMU_I2C, mti->address<<1, combined_buf, numBytes + 2, 1000);
	if(status != HAL_OK) {
		printDebug("Failed to send message, status: %d", status);
	}
}





void MTi_readMessages(MTi* mti) {
	// Assume that drdy_pin is the pin number connected to DRDY pin
	while (HAL_GPIO_ReadPin(mti->drdy_port, mti->drdy_pin) == GPIO_PIN_SET) {
		Xbus_read(&(mti->xbus), mti->address);
	}
}




float* MTi_getEulerAngles(MTi* mti)
{
	return mti->xbus.euler;

}


float* MTi_getAcceleration(MTi* mti)
{
	return mti->xbus.acc;

}

float* MTi_getRateOfTurn(MTi* mti)
{
	return mti->xbus.rot;
}

float* MTi_getLatLon(MTi* mti)
{
	return mti->xbus.latlon;
}


uint32_t MTi_getSampleTimeFine(MTi* mti) {
	return mti->xbus.sampleTimeFine;
}

float* MTi_getMagneticField(MTi* mti) {
	return mti->xbus.mag;
}

uint32_t MTi_getStatusWord(MTi* mti) {
	return mti->xbus.statusWord;
}









