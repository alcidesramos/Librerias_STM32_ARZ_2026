#include "main.h"
#include "delays.h"




#define  BH1750_address_w          0x46
#define  power_down                0x00
#define  power_up                  0x01
//modos operacion

//autoresolucion
#define  continua_auto_res1         0b00010000
#define  continua_auto_res2         0b00100000
//alta resolucion 1Lux  Hasta  8191 Lux
#define  continua_HI_1                0b0001_0010
#define  continua_HI_2                0b0010_0010

//baja resolucion 8Lux   Hasta 65528 Lux
#define  continua_LO_1                0b00010011
#define  continua_LO_2                0b00010110
#define  continua_LO_3                0b00100011
#define  continua_LO_4                0b00100110


void BH1750_init();
uint16_t BH1750_read_data();
void BH1750_write(uint8_t dato);
uint16_t BH1750_Lumen(uint8_t modo);
