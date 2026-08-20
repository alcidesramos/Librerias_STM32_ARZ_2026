/*
 * SN0575.c
 *
 *  Created on: Jun 23, 2024
 *      Author: Alcides Ramos
 */

#include "SN0575.h"

#define readRegister  I2C_Readm_n
#define writeRegister I2C_Writem_n

    uint32_t vid=0;
    uint32_t pid=0;



    uint8_t I2C_Writem_n(uint8_t dir,uint8_t *bufft,uint8_t ndat)
    {
    HAL_I2C_Mem_Write(&RainfallSensor_I2C,RainfallSensor_ADDR, dir, I2C_MEMADD_SIZE_8BIT, &bufft, ndat, 1000);
    }

    uint8_t I2C_Readm_n(uint8_t dir,uint8_t *buffr,uint8_t ndat)
    {
    HAL_I2C_Mem_Read(&RainfallSensor_I2C,RainfallSensor_ADDR, dir, I2C_MEMADD_SIZE_8BIT, buffr, ndat, 1000);
    }


    bool RainfallSensor_getPidVid()
    {
      bool ret = false;
        uint8_t buff[4] = {0};
        readRegister( I2C_REG_PID, &buff, 4 );
        pid = buff[0] | ( ( (uint16_t)buff[1] ) << 8 ) | ( ( (uint32_t)( buff[3] & 0xC0 ) ) << 10 );
        vid = buff[2] | (uint16_t)( ( buff[3] & 0x3F ) << 8 );

      if( ( vid == 0x3343 ) && ( pid == 0x100C0 ) ){
        ret = true;
      }
      return ret;
    }

    uint32_t RainfallSensor_getRainfall()
    {
      uint32_t rainfall=0;
        uint8_t buff[4]={0};
        readRegister( I2C_REG_CUMULATIVE_RAINFALL, &buff, 4 );
        rainfall = buff[0] | ( ( (uint32_t)buff[1] ) << 8 ) | ( ( (uint32_t)buff[2] ) << 16 ) | ( ( (uint32_t)buff[3]) << 24 );
      return rainfall ;
    }

    float RainfallSensor_getRainfall_f()
      {
        uint32_t rainfall=0;
          uint8_t buff[4]={0};
          readRegister( I2C_REG_CUMULATIVE_RAINFALL, &buff, 4 );
          rainfall = buff[0] | ( ( (uint32_t)buff[1] ) << 8 ) | ( ( (uint32_t)buff[2] ) << 16 ) | ( ( (uint32_t)buff[3]) << 24 );
        return rainfall / 10000.0;
      }

    float RainfallSensor_getRainfall_h(uint8_t hour)
    {
      uint32_t rainfall = 0 ;

        writeRegister(I2C_REG_RAIN_HOUR, hour, 1);
        uint8_t buff[4] = {0};
        if( readRegister(I2C_REG_TIME_RAINFALL, &buff, 4 ) == 0 ){
          return -1;
        }
        rainfall = buff[0] | ( ( (uint32_t)buff[1] ) << 8 ) | ( ( (uint32_t)buff[2]) << 16 ) | ( ( (uint32_t)buff[3] ) << 24 );
           return rainfall / 10000.0;
    }


    uint32_t RainfallSensor_getRawData()
    {
      uint32_t rawdata = 0;

        uint8_t buff[4] = { 0 };
        readRegister( I2C_REG_RAW_DATA, &buff, 4 );
        rawdata = buff[0] | ( ( (uint32_t)buff[1] ) << 8 ) | ( ( (uint32_t)buff[2] ) << 16 ) | ( ( (uint32_t)buff[3]) << 24 );
           return rawdata;
    }

    uint8_t RainfallSensor_setRainAccumulatedValue(float value)
    {
      uint8_t ret = 0;
      uint16_t data = value * 10000;

        uint8_t buff[2] = { 0 };
        buff[0] = ( data & 0xFF );
        buff[1] = ( data >> 8 );
        ret = writeRegister( I2C_REG_BASE_RAINFALL, &buff, 2 );
      return ret;
    }

    uint16_t  RainfallSensor_getSensorWorkingTime()
    {
      uint16_t WorkingTime = 0 ;

        uint8_t buff[2] = { 0 };
        readRegister( I2C_REG_SYS_TIME, &buff, 2 );
        WorkingTime = buff[0] | ( ( (uint32_t)buff[1] ) << 8 );
      //return WorkingTime / 60.0;
        return WorkingTime;

    }

