#include "BH1750.h"
extern I2C_HandleTypeDef hi2c1;
uint8_t BH1750_dato[2];

void BH1750_init()
{       delay_us_dwt_init();
        delay_ms(50);//20 ms por datasheet
        BH1750_write(power_down);
}                  

void BH1750_write(uint8_t dato)
{ BH1750_dato[0]=dato;
	 HAL_I2C_Master_Transmit(&i2c_pin, BH1750_address_w  ,BH1750_dato, 1, 300);
}

uint16_t BH1750_read_data()
{                      
	    uint16_t luz = 0;
        uint8_t BH1750_dato[5];

//lee luz
   	 HAL_I2C_Master_Receive(&i2c_pin, BH1750_address_w, BH1750_dato,2, 300);
        luz = ( (uint16_t)BH1750_dato[0] << 8 ) | BH1750_dato[1];//arma el dato
        return luz;
} 


uint16_t  BH1750_Lumen(uint8_t modo)
{
	  uint16_t lumen = 0;
        BH1750_write(power_up);
        BH1750_write(modo);
        lumen = BH1750_read_data();
        delay_ms(120);//retardo lectura por datasheet
        BH1750_write(power_down);
        return lumen;
}                                
