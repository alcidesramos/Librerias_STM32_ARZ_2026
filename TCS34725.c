/*
 * TCS34725.c
 *
 *  Created on: Apr 14, 2025
 *      Author: daniel
 */

#include "TCS34725.h"

uint8_t _tcs34725Initialised = 0;
//int red, green, blue;

/* Writes a register and an 8 bit value over I2C */
void write8 (uint8_t reg, uint32_t value)
{
    uint8_t txBuffer[2];
    txBuffer[0] = (TCS34725_COMMAND_BIT | reg);
    txBuffer[1] = (value & 0xFF);
    HAL_I2C_Master_Transmit(&TCS34725_I2C, TCS34725_ADDRESS, txBuffer, 2, 100);
}

/* Reads an 8 bit value over I2C */
uint8_t read8(uint8_t reg)
{
    uint8_t buffer[1];
    buffer[0] = (TCS34725_COMMAND_BIT | reg);
    HAL_I2C_Master_Transmit(&TCS34725_I2C, TCS34725_ADDRESS, buffer, 1, 100);
    HAL_I2C_Master_Receive(&TCS34725_I2C, TCS34725_ADDRESS, buffer, 1, 100);
    return buffer[0];
}

/* Reads a 16 bit values over I2C */
uint16_t read16(uint8_t reg)
{
	  uint16_t ret;
	  uint8_t txBuffer[1], rxBuffer[2];
	  txBuffer[0] = (TCS34725_COMMAND_BIT | reg);
	  HAL_I2C_Master_Transmit(&TCS34725_I2C, TCS34725_ADDRESS, txBuffer, 1, 100);
	  HAL_I2C_Master_Receive(&TCS34725_I2C, TCS34725_ADDRESS, rxBuffer, 2, 100);
	  ret = rxBuffer[0];              // <- LSB primero
	  ret |= (rxBuffer[1] << 8);      // <- MSB después
	  return ret;
}

void enable(void)
{
  write8(TCS34725_ENABLE, TCS34725_ENABLE_PON);
  HAL_Delay(3);
  write8(TCS34725_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);
  HAL_Delay(50);
}

void disable(void)
{
  /* Turn the device off to save power */
  uint8_t reg = 0;
  reg = read8(TCS34725_ENABLE);
  write8(TCS34725_ENABLE, reg & ~(TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN));
}

void setIntegrationTime(uint8_t itime)
{
  if (_tcs34725Initialised == 0) tcs3272_init();
  write8(TCS34725_ATIME, itime);
}

void setGain(uint8_t gain)
{
  if (_tcs34725Initialised == 0) tcs3272_init();
  write8(TCS34725_CONTROL, gain);
}

void tcs3272_init(void)
{
  /* Make sure we're actually connected */
  uint8_t readValue = read8(TCS34725_ID);
  if ((readValue != 0x44) && (readValue != 0x10) && (readValue != 0x4d))
  {
    return;
  }
  _tcs34725Initialised = 1;
  /* Set default integration time and gain */
  setIntegrationTime(TCS34725_INTEGRATIONTIME_50MS);
  setGain(TCS34725_GAIN_4X);
  /* Note: by default, the device is in power down mode on bootup */
  enable();
}

/* Get raw data */
void getRawData (uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c)
{
  if (_tcs34725Initialised == 0) tcs3272_init();

  *c = read16(TCS34725_CDATAL);
  *r = read16(TCS34725_RDATAL);
  *g = read16(TCS34725_GDATAL);
  *b = read16(TCS34725_BDATAL);
  /* Delay time is from page no 16/26 from the datasheet  (256 − ATIME)* 2.4ms */
  HAL_Delay(50); // Set delay for (256 − 0xEB)* 2.4ms = 50ms
}

/* Get Red, Green and Blue color from Raw Data */
void getRGB(int *R, int *G, int *B)
{
    uint16_t rawRed, rawGreen, rawBlue, rawClear;
    getRawData(&rawRed, &rawGreen, &rawBlue, &rawClear);
    if(rawClear == 0)
    {
      *R = 0;
      *G = 0;
      *B = 0;
    }
    else
    {
      *R = (int)rawRed * 255 / rawClear;
      *G = (int)rawGreen * 255 / rawClear;
      *B = (int)rawBlue * 255 / rawClear;
    }
}


