/*
 * RainfallSensor.c
 *
 *  Created on: Jun 19, 2024
 *      Author: Alcides Ramos
 */

#include "main.h"
#include "RainfallSensor.h"

uint32_t vid;
uint32_t pid;

#define readRegister  I2C_Readm_n
#define writeRegister I2C_Writem_n
//ESCRIBE UN BYTE I2C
void I2C_Writem(uint8_t dir,uint8_t val)
{
	HAL_I2C_Mem_Write(&RainfallSensor_I2C,RainfallSensor_ADDR, dir, I2C_MEMADD_SIZE_8BIT, &val, 1, 1000);
}
//LEE UN BYTE I2C
uint8_t I2C_Readm(uint8_t dir)
{
	uint8_t Buf;
	HAL_I2C_Mem_Read(&RainfallSensor_I2C,RainfallSensor_ADDR, dir, I2C_MEMADD_SIZE_8BIT, Buf, 1, 1000);
   return(Buf);

}
uint8_t I2C_Readm_n(uint8_t dir,uint8_t *buffr,uint8_t ndat)
{
HAL_I2C_Mem_Read(&RainfallSensor_I2C,RainfallSensor_ADDR, dir, I2C_MEMADD_SIZE_8BIT, buffr, ndat, 1000);
}

uint8_t I2C_Writem_n(uint8_t dir,uint8_t *bufft,uint8_t ndat)
{
HAL_I2C_Mem_Write(&RainfallSensor_I2C,RainfallSensor_ADDR, dir, I2C_MEMADD_SIZE_8BIT, &bufft, ndat, 1000);
}



uint8_t RainfallSensor_getPidVid()
{
  uint8_t ret = 0;
    uint8_t buff[4] ;
    readRegister( I2C_REG_PID, &buff, 4 );
    pid = buff[0] | ( ( (uint16_t)buff[1] ) << 8 ) | ( ( (uint32_t)( buff[3] & 0xC0 ) ) << 10 );
    vid = buff[2] | (uint16_t)( ( buff[3] & 0x3F ) << 8 );

  if( ( vid == 0x3343 ) && ( pid == 0x100C0 ) ){
    ret = 1;
  }
  return ret;
}


float RainfallSensor_getRainfall()
{
  uint32_t rainfall=0;
    uint8_t buff[4]={0,0,0,0};
    readRegister( I2C_REG_CUMULATIVE_RAINFALL, &buff, 4 );
    rainfall = buff[0] | ( ( (uint32_t)buff[1] ) << 8 ) | ( ( (uint32_t)buff[2] ) << 16 ) | ( ( (uint32_t)buff[3]) << 24 );
    return ((float)rainfall / 10000.0);
}


float RainfallSensor_getRainfall_h(uint8_t hour)
{
  uint32_t rainfall = 0 ;

  //writeRegister(I2C_REG_RAIN_HOUR, &hour, 1);
    uint8_t buff[4] ;
    if( readRegister(I2C_REG_TIME_RAINFALL, &buff, 4 ) == 0 )
    {
      return -1;
    }
    rainfall = buff[0] | ( ( (uint32_t)buff[1] ) << 8 ) | ( ( (uint32_t)buff[2]) << 16 ) | ( ( (uint32_t)buff[3] ) << 24 );

  return ((float)rainfall / 10000.0);
}

uint32_t RainfallSensor_getRawData()
{
  uint32_t rawdata = 0;
    uint8_t buff[4] ;
    readRegister( I2C_REG_RAW_DATA, &buff, 4 );
    rawdata = buff[0] | ( ( (uint32_t)buff[1] ) << 8 ) | ( ( (uint32_t)buff[2] ) << 16 ) | ( ( (uint32_t)buff[3]) << 24 );
    return rawdata;
}

uint8_t RainfallSensor_setRainAccumulatedValue(float value)
{
  uint8_t ret = 0;
  uint16_t data = value * 10000;
    uint8_t buff[2] ;
    buff[0] = ( data & 0xFF );
    buff[1] = ( data >> 8 );
    ret = writeRegister( I2C_REG_BASE_RAINFALL, &buff, 2 );
  return ret;
}


void RainfallSensor_Reset()
{

    uint8_t buff[2] ;
    buff[0] = 0;
    buff[1] = 0;
    writeRegister( I2C_REG_BASE_RAINFALL, &buff, 2 );

}

float RainfallSensor_getSensorWorkingTime()
{
  uint16_t WorkingTime = 0 ;
    uint8_t buff[2] ;
    readRegister( I2C_REG_SYS_TIME, &buff, 2 );
    WorkingTime = buff[0] | ( ( (uint32_t)buff[1] ) << 8 );
    return WorkingTime / 60.0;
}


