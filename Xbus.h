/*
 * Xbus.h
 *
 *  Created on: Jul 14, 2023
 *      Author: Administrator
 */

#ifndef SRC_XBUS_H_
#define SRC_XBUS_H_

#include <stdbool.h>
#include "main.h"
#include <math.h>


// Definition of opcodes: For more information on opcodes, refer to https://mtidocs.xsens.com/functional-description$mtssp-synchronous-serial-protocol
#define XSENS_CONTROL_PIPE 0x03
#define XSENS_STATUS_PIPE 0x04
#define XSENS_NOTIF_PIPE 0x05
#define XSENS_MEAS_PIPE 0x06

typedef enum {
	WAKEUP = 0x3E,
	GOTOCONFIGACK = 0x31,
	GOTOMEASUREMENTACK = 0x11,
	REQDID = 0x00,
	DEVICEID = 0x01,
	REQPRODUCTCODE = 0x1C,
	PRODUCTCODE = 0x1D,
	REQFWREV = 0x12,
	FIRMWAREREV = 0x13,
	XSERROR = 0x42,
	WARNING = 0x43,
	OUTPUTCONFIGURATION = 0xC1,
	MTDATA2 = 0x36
} MesID;

typedef enum {
	SAMPLETIMEFINE = 0x1060,
	EULERANGLES = 0x2030,
	RATEOFTURN = 0x8020,
	ACCELERATION = 0x4020,
	MAGNETICFIELD = 0xC020,
	STATUSWORD = 0xE020,
	LATLON = 0x5040
} DataID;

typedef struct Xbus {
	I2C_HandleTypeDef *_hi2c;                                      // used for I2C operations


	uint8_t status[4];                                            //Used to store indicators of the Status Pipe
	uint8_t datanotif[256];                                       //Used to store content read from the Notification Pipe
	uint8_t datameas[256];                                        //Used to store content read from the Measurement Pipe


	uint16_t notificationSize;
	uint16_t measurementSize;

	uint32_t sampleTimeFine;                                    //Used to store latest SampleTimeFine reading
	float euler[3];                                             //Used to store latest EulerAngle reading
	float acc[3];                                               //Used to store latest Acceleration reading
	float rot[3];                                               //Used to store latest RateOfTurn reading
	float mag[3];                                               //Used to store latest MagneticField reading
	float latlon[2];                                            //Used to store latest Latitude/Longitude reading
	uint32_t statusWord;                                            //Used to store latest StatusWord reading
	bool configState;                                           //True if MTi is in Config mode, false if MTi is in Measurement mode
	char productCode;
} Xbus;

void Xbus_init(Xbus* self, I2C_HandleTypeDef *hi2c);
bool Xbus_read(Xbus* self, uint8_t address);
void Xbus_readPipeStatus(Xbus* self, uint8_t address);
void Xbus_readPipeNotif(Xbus* self, uint8_t address);
void Xbus_readPipeMeas(Xbus* self, uint8_t address);
void Xbus_parseMTData2(Xbus* self, uint8_t* data, uint8_t datalength);
void Xbus_parseNotification(Xbus* self, uint8_t* data);
void Xbus_dataswapendian(Xbus* self, uint8_t* data, uint8_t length);

#endif /* SRC_XBUS_H_ */
