/*
 * MS5611.c
 *
 *  Created on: May 4, 2024
 *      Author: Alcides Ramos
 */


#include "MS5611.h"
#include "math.h"
#include "stdlib.h"

//como usar
// en el  void main
/*
  MS5611_Init();
 // o si se desea saber si constesta
 if (MS5611_Init()==1)  uartx_write_text(&huart1, "SENSOR OK\r\n");
else  uartx_write_text(&huart1, "SENSOR BAD\r\n");

 */

//en el while
/*
 * MS5611_read(OSR_ULTRA_LOW);
	   float temp= MS5611_getTemperature();
	     float pres= MS5611_getPressure();
	     float altura=MS5611_getaltura(pres, temp);

	         sprintf(texto, "Temp=%.2f, Pre=%.2fmbar  alt=%.1f\r\n", temp, pres,altura);
	 		uartx_write_text(&huart1, texto);
               HAL_Delay(800);

 */

  uint8_t  _address;
  uint8_t  _samplingRate;
  int32_t  _temperature;
  int32_t  _pressure;
  float    _pressureOffset;
  float    _temperatureOffset;
  int      _result;
  float    C[7];
  uint32_t _lastRead;
  uint32_t _deviceID;
  bool     _compensation;

  void initConstants(uint8_t mathMode)
   {
     C[0] = 1;
     C[1] = 32768L;          //  SENSt1   = C[1] * 2^15    |    * 2^16
     C[2] = 65536L;          //  OFFt1    = C[2] * 2^16    |    * 2^17
     C[3] = 3.90625E-3;      //  TCS      = C[3] / 2^8     |    / 2^7
     C[4] = 7.8125E-3;       //  TCO      = C[4] / 2^7     |    / 2^6
     C[5] = 256;             //  Tref     = C[5] * 2^8     |    * 2^8
     C[6] = 1.1920928955E-7; //  TEMPSENS = C[6] / 2^23    |    / 2^23

     if (mathMode == 1)  //  Appnote version for pressure.
     {
       C[1] = 65536L;          //  SENSt1
       C[2] = 131072L;         //  OFFt1
       C[3] = 7.8125E-3;       //  TCS
       C[4] = 1.5625e-2;       //  TCO
     }
   }
  bool MS5611_reset(uint8_t mathMode)
   {
     command(MS5611_CMD_RESET);
     HAL_Delay(3);   // antes en 3

     //  initialize the C[] array
     initConstants(mathMode);

     //  read factory calibrations from EEPROM.
     bool ROM_OK = true;
     for (uint8_t reg = 0; reg < 7; reg++)
     {
       //  used indices match datasheet.
       //  C[0] == manufacturer - read but not used;
       //  C[7] == CRC - skipped.
       uint16_t tmp = readProm(reg);
       C[reg] *= tmp;
       _deviceID <<= 4;
       _deviceID ^= tmp;
       if (reg > 0)
       {
         ROM_OK = ROM_OK && (tmp != 0);
       }
     }
     return ROM_OK;
   }

uint8_t MS5611_Init()
 {
	   _samplingRate      = OSR_ULTRA_LOW ;
	   _temperature       = MS5611_NOT_READ;
	   _pressure          = MS5611_NOT_READ;
	   _result            = MS5611_NOT_READ;
	   _lastRead          = 0;
	   _deviceID          = 0;
	   _pressureOffset    = 0;
	   _temperatureOffset = 0;
	   _compensation      = true;
	   MS5611_reset(0);  //

	if (HAL_I2C_IsDeviceReady(&MS5611_I2C ,MS5611_DIR, 2, 200)== HAL_OK) return (1);
	else return (0);

 }
  int command(const uint8_t command)
 {
	_result =	HAL_I2C_Master_Transmit(&MS5611_I2C ,MS5611_DIR, &command, 1, 1000);
    return _result;
 }
 uint16_t readProm(uint8_t reg)
 {
   uint8_t promCRCRegister = 7;
   uint8_t bytes[2];

   if (reg > promCRCRegister) return 0;
   uint8_t offset = reg * 2;

   command(MS5611_CMD_READ_PROM + offset);
   HAL_I2C_Master_Receive(&MS5611_I2C, MS5611_DIR, bytes, 2, 1000);
   return ((uint16_t) bytes[0] << 8) | bytes[1];

   }


 void MS5611_convert(const uint8_t addr, uint8_t bits)
 {
   uint8_t index = bits;
   if (index < 8) index = 8;
   else if (index > 12) index = 12;
   index -= 8;
   uint8_t offset = index * 2;
   command(addr + offset);
   HAL_Delay(10);// ajusta acorde a parametro del datasheet
 }
 uint32_t MS5611_readADC()
 {
	 uint8_t adc_bytes[3];
   command(MS5611_CMD_READ_ADC);
	HAL_I2C_Master_Receive (&MS5611_I2C,MS5611_DIR, adc_bytes, 3, 1000);

	return ((uint32_t) adc_bytes[0] << 16) | ((uint32_t) adc_bytes[1] << 8)
			| ((uint32_t) adc_bytes[2]);

 }


 int MS5611_read(uint8_t bits)
 {
	 MS5611_convert(MS5611_CMD_CONVERT_D1, bits);
   if (_result) return _result;
   uint32_t _D1 = MS5611_readADC();
   if (_result) return _result;

   MS5611_convert(MS5611_CMD_CONVERT_D2, bits);
   if (_result) return _result;
   uint32_t _D2 = MS5611_readADC();
   if (_result) return _result;
   float dT = _D2 - C[5];
   _temperature = 2000 + dT * C[6];

   float offset =  C[2] + dT * C[4];
   float sens = C[1] + dT * C[3];

   if (_compensation)
   {
     if (_temperature < 2000)
     {
       float T2 = dT * dT * 4.6566128731E-10;
       float t = (_temperature - 2000) * (_temperature - 2000);
       float offset2 = 2.5 * t;
       float sens2 = 1.25 * t;
       //  COMMENT OUT < -1500 CORRECTION IF NOT NEEDED
       if (_temperature < -1500)
       {
         t = (_temperature + 1500) * (_temperature + 1500);
         offset2 += 7 * t;
         sens2 += 5.5 * t;
       }
       _temperature -= T2;
       offset -= offset2;
       sens -= sens2;
     }
     //  END SECOND ORDER COMPENSATION
   }

   _pressure = (_D1 * sens * 4.76837158205E-7 - offset) * 3.051757813E-5;

   return MS5611_READ_OK;
 }

 void MS5611_setOversampling(uint8_t samplingRate)
 {
   _samplingRate = (uint8_t) samplingRate;
 }


 float MS5611_getTemperature()
 {
   if (_temperatureOffset == 0) return _temperature * 0.01;
   return _temperature * 0.01 + _temperatureOffset;
 };


 float MS5611_getPressure()
 {
   if (_pressureOffset == 0) return _pressure * 0.01;
   return _pressure * 0.01 + _pressureOffset;
 };
 //       EXPERIMENTAL
 uint16_t MS5611_getManufacturer()
 {
   return readProm(0);
 }

 //       EXPERIMENTAL
 uint16_t MS5611_getSerialCode()
 {
   return readProm(7) >> 4;
 }

 float MS5611_getaltura(float pressure, float temperature)
 {
     const float L = 0.0065; // Tasa de disminución de temperatura en K/m
     const float P0 = 1013.25; // Presión a nivel del mar en mbar
     const float g = 9.80665; // Aceleración debida a la gravedad en m/s²
     const float R = 287.05; // Constante de gas específica para el aire seco en J/(kg·K)

     // Convertir la temperatura a Kelvin
     float T = temperature + 273.15;

     // Calcular la altitud
     float altitude = (T / L) * (pow((P0 / pressure), (R * L / g)) - 1.0);

     return altitude;
 }

 float calc_altitude_pressure(float pressure_ambient,float temp)
 {
 	//return 44330.77f * ((1-pow((pressure_ambient/pressure_sea_level),(0.19029495f)))); // Meters
   //return logf(pressure_ambient / 101325.f) * (273.15f + temp) * (287.05f / 9.81f);
   //return 44330.0f * (1.0 - (powf(pressure_ambient / 101325.0, 0.19)));

	 pressure_ambient=pressure_ambient*100.0;

   return logf(101325.f / pressure_ambient) * (273.15f + temp) * (287.05f / 9.81f);

   //https://forums.whirlpool.net.au/archive/1403299

   //https://www.mide.com/air-pressure-at-altitude-calculator

 }
