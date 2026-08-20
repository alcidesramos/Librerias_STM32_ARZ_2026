/*
 * GPS_PA1010.h
 *
 *  Created on: Jun 10, 2024
 *      Author: Alcides Ramos
 */

#ifndef LIBRERIAS_GPS_PA1010_H_
#define LIBRERIAS_GPS_PA1010_H_


#define GPS_I2C_ADDRESS (0x10 << 1) // Dirección I2C del GPS PA1010
//i2c a usar
extern I2C_HandleTypeDef hi2c2;
#define GPS_I2C hi2c2

// Comandos de configuración NMEA
#define PMTK_SET_NMEA_UPDATE_100HZ            "$PMTK220,100*2F\r\n" // Update rate: 100 mHz (1 update every 10 seconds)
#define PMTK_SET_NMEA_UPDATE_1HZ             "$PMTK220,1000*1F\r\n"  // Update rate: 1 Hz (1 update every second)
#define PMTK_SET_NMEA_UPDATE_5HZ             "$PMTK220,200*2C\r\n"   // Update rate: 5 Hz (5 updates every second)

#define PMTK_API_SET_FIX_CTL_5HZ             "$PMTK300,200,0,0,0,0*2F\r\n" // Fix rate: 5 Hz
#define PMTK_API_SET_FIX_CTL_1HZ             "$PMTK300,1000,0,0,0,0*1C\r\n" // Fix rate: 1 Hz

#define PMTK_SET_BAUD_9600                   "$PMTK251,9600*17\r\n"  // Set baud rate to 9600
#define PMTK_SET_BAUD_57600                  "$PMTK251,57600*2C\r\n" // Set baud rate to 57600

#define PMTK_SET_NMEA_OUTPUT_GPGGA_ONLY      "$PMTK314,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n" // GPGGA only
#define PMTK_SET_NMEA_OUTPUT_GPRMC_ONLY      "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n" // GPGGA only

#define PMTK_SET_NMEA_OUTPUT_ALLDATA         "$PMTK314,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1*29\r\n" // All data
#define PMTK_SET_NMEA_OUTPUT_OFF             "$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n" // Turn off NMEA output

#define PMTK_ENABLE_SBAS                      "$PMTK313,1*2E\r\n"  // Enable SBAS
#define PMTK_ENABLE_WAAS                      "$PMTK301,2*2E\r\n"  // Enable WAAS

#define PMTK_CMD_HOT_START                    "$PMTK101*32\r\n" // Hot start
#define PMTK_CMD_WARM_START                   "$PMTK102*31\r\n" // Warm start
#define PMTK_CMD_COLD_START                   "$PMTK103*30\r\n" // Cold start
#define PMTK_CMD_FULL_COLD_START              "$PMTK104*37\r\n" // Full cold start
#define PMTK_Q_RELEASE                        "$PMTK605*31\r\n" // Query release

uint8_t GPS_I2C_CONNECT();
void GPS_CMD(const char *comando);
void STR_QUITA( const char caract, char *src, char *dst);


#endif /* LIBRERIAS_GPS_PA1010_H_ */
