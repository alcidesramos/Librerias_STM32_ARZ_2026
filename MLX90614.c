/*
 * MLX90614.c
 *
 *  Created on: May 26, 2025
 *      Author: ALCIDES_RAMOS
 */


#include "MLX90614.h"
#include "math.h"

uint8_t _addr;

extern I2C_HandleTypeDef MLX90614_i2c;


uint8_t MLX90614_Init()
{
if (HAL_I2C_IsDeviceReady(&MLX90614_i2c, MLX90614_I2CADDR, 2, 500)==HAL_OK) return 1;
else return(0);

}


uint16_t MLX90614_read16(uint8_t dir)
{
  uint8_t buffer[3];
  HAL_StatusTypeDef status;
 //lee 3 bytes
   status= (HAL_I2C_Mem_Read (&MLX90614_i2c,MLX90614_I2CADDR,dir,1, &buffer, 3, 500));

  if (status!=HAL_OK)
    return 0;
  return ((uint16_t) buffer[0] | (uint16_t) buffer[1]  << 8);
}

uint8_t MLX90614_crc8(uint8_t *addr, uint8_t len)
{
 uint8_t crc = 0;
  while (len--) {
    uint8_t inbyte = *addr++;
    for (uint8_t i = 8; i; i--) {
      uint8_t carry = (crc ^ inbyte) & 0x80;
      crc <<= 1;
      if (carry)
        crc ^= 0x7;
      inbyte <<= 1;
    }
  }
  return crc;
}

void MLX90614_write16(uint8_t a, uint16_t v) {
  uint8_t buffer[4];

  buffer[0] = MLX90614_I2CADDR;
  buffer[1] = a;
  buffer[2] = v & 0xff;
  buffer[3] = v >> 8;

  uint8_t pec = MLX90614_crc8(buffer, 4);

  buffer[0] = buffer[1];
  buffer[1] = buffer[2];
  buffer[2] = buffer[3];
  buffer[3] = pec;
  HAL_I2C_Master_Transmit(&MLX90614_i2c,MLX90614_I2CADDR, buffer, 4, 500);
}

uint16_t MLX90614_readEmissivityReg()
{
  return MLX90614_read16(MLX90614_EMISS);
}

void MLX90614_writeEmissivityReg(uint16_t ereg)
{
	MLX90614_write16(MLX90614_EMISS, 0); //
  HAL_Delay(10);
  MLX90614_write16(MLX90614_EMISS, ereg);
  HAL_Delay(10);
}

double MLX90614_readEmissivity()
{
  uint16_t ereg = MLX90614_read16(MLX90614_EMISS);
  if (ereg == 0)
    return NAN;
  return ((double)ereg) / 65535.0;
}

void MLX90614_writeEmissivity(double emissivity)
{
  uint16_t ereg = (uint16_t)(0xffff * emissivity);

  MLX90614_writeEmissivityReg(ereg);
}

float MLX90614_readTemp(uint8_t reg)
{
  float temp;

  temp = MLX90614_read16(reg);
  if (temp == 0)
    return NAN;
  temp *= .02;
  temp -= 273.15;
  return temp;
}

double MLX90614_readObjectTempF()
{
  return (MLX90614_readTemp(MLX90614_TOBJ1) * 9 / 5) + 32;
}

double MLX90614_readAmbientTempF()
{
  return (MLX90614_readTemp(MLX90614_TA) * 9 / 5) + 32;
}

double MLX90614_readObjectTempC()
{
  return MLX90614_readTemp(MLX90614_TOBJ1);
}

double MLX90614_readAmbientTempC()
{
  return MLX90614_readTemp(MLX90614_TA);
}
