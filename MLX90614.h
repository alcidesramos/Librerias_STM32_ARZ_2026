/*
 * MLX90614.h
 *
 *  Created on: May 26, 2025
 *      Author: ALCIDES_RAMOS
 */
#include "main.h"
#include "stdbool.h"

#ifndef LIBRERIAS_MLX90614_H_
#define LIBRERIAS_MLX90614_H_


//colocar el i2c a usar con el imu
#define MLX90614_i2c hi2c1


//direccion del sensor
#define MLX90614_I2CADDR 0x5A <<1

//MAPA DE MEMORIA DEL SENSOR

// RAM
#define MLX90614_RAWIR1 0x04
#define MLX90614_RAWIR2 0x05
#define MLX90614_TA 0x06
#define MLX90614_TOBJ1 0x07
#define MLX90614_TOBJ2 0x08

// EEPROM
#define MLX90614_TOMAX 0x20
#define MLX90614_TOMIN 0x21
#define MLX90614_PWMCTRL 0x22
#define MLX90614_TARANGE 0x23
#define MLX90614_EMISS 0x24
#define MLX90614_CONFIG 0x25
#define MLX90614_ADDR 0x2E
#define MLX90614_ID1 0x3C
#define MLX90614_ID2 0x3D
#define MLX90614_ID3 0x3E
#define MLX90614_ID4 0x3F


//procedimientos

  uint8_t MLX90614_Init();
  double MLX90614_readObjectTempC();
  double MLX90614_readAmbientTempC();
  double MLX90614_readObjectTempF();
  double MLX90614_readAmbientTempF();
   double MLX90614_readEmissivity();
  float MLX90614_readTemp(uint8_t reg);
    uint16_t MLX90614_read16(uint8_t addr);
    void MLX90614_write16(uint8_t addr, uint16_t data);
    uint8_t crc8(uint8_t *addr, uint8_t len);
     void MLX90614_writeEmissivity(double emissivity);



#endif /* LIBRERIAS_MLX90614_H_ */
