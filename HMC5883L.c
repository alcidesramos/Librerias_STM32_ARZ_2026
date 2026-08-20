/*
 * HMC5883L.c
 *
 *  Created on: 15/11/2021
 *      Author: Alcides Ramos
 */



#include <HMC5883L.h>
#include "puertos.h"
#include "math.h"

extern I2C_HandleTypeDef hi2c1;

unsigned char bruj_data;


signed int X_axis = 0;
signed int Y_axis = 0;
signed int Z_axis = 0;
float m_scale = 1.0;  //escala de los ejes



void HMC5883L_scale_axes()
{       X_axis *= m_scale;
        Z_axis *= m_scale;
        Y_axis *= m_scale;
}

void HMC5883L_set_scale(float gauss)
{
        unsigned char value = 0;

    if(gauss == 0.88)
    {
                value = 0x00;
                m_scale = 0.73;
        }

        else if(gauss == 1.3)
        {
                value = 0x01;
                m_scale = 0.92;
        }

        else if(gauss == 1.9)
        {
                value = 0x02;
                m_scale = 1.22;
        }

        else if(gauss == 2.5)
        {
                value = 0x03;
                m_scale = 1.52;
        }

        else if(gauss == 4.0)
        {
                value = 0x04;
                m_scale = 2.27;
        }

        else if(gauss == 4.7)
        {
                value = 0x05;
                m_scale = 2.56;
        }

        else if(gauss == 5.6)
        {
                value = 0x06;
                m_scale = 3.03;
        }

        else if(gauss == 8.1)
        {
                value = 0x07;
                m_scale = 4.35;
        }

        value <<= 5;


        bruj_data = value;
 	    HAL_I2C_Mem_Write(&hi2c1, HMC5883L_WRITE_ADDR, Config_Reg_B, 1,&bruj_data, 1, 1000);
 	}



void HMC5883L_init(float escala_gauss)
{

	       bruj_data = 0X70;   //CONFIGURA A 15hz  promedia 8 muestras
	        HAL_I2C_Mem_Write(&hi2c1, HMC5883L_WRITE_ADDR, Config_Reg_A, 1,&bruj_data, 1, 100);
	        bruj_data = 0xA0;   // gANANCIA A 4.6 Gauus
	        HAL_I2C_Mem_Write(&hi2c1, HMC5883L_WRITE_ADDR, Config_Reg_B, 1,&bruj_data, 1, 100);
	        bruj_data = 0x00;   // Medición contunua
	        HAL_I2C_Mem_Write(&hi2c1, HMC5883L_WRITE_ADDR, Mode_Reg, 1,&bruj_data, 1, 100);
	        HMC5883L_set_scale(escala_gauss);

}

void HMC5883L_Read()
{
	uint8_t Dato_buff[6];
	//lee los 6 datos
		HAL_I2C_Mem_Read (&hi2c1,  HMC5883L_WRITE_ADDR,X_MSB_Reg, 1, Dato_buff, 6, 100);
		 X_axis= (int16_t)(Dato_buff[0] << 8 | Dato_buff [1]);
		 Z_axis = (int16_t)(Dato_buff[2] << 8 | Dato_buff [3]);
		 Y_axis= (int16_t)(Dato_buff[4] << 8 | Dato_buff [5]);

}

float HMC5883L_heading()
{
    float heading = 0.0;
    HMC5883L_Read();
    HMC5883L_scale_axes();
    heading = atan2(Y_axis, X_axis);
    heading += declination_angle;

    if(heading < 0.0)
    {
            heading += (2.0 * PI);
    }

    if(heading > (2.0 * PI))
    {
            heading -= (2.0 * PI);
    }
        heading *= (180.0 / PI);
       return heading;

}

//heading por compensación de inclinación
float HMC5883L_heading_compen(float a_roll,float a_pitch)
{
    float heading1 = 0.0;
    float x_aux,y_aux;

    a_roll=a_roll*PI/180.0;
    a_pitch=a_pitch*PI/180.0;

    HMC5883L_Read();
    HMC5883L_scale_axes();
    x_aux=X_axis*cos(a_pitch)+Z_axis*sin(a_pitch);
    y_aux=X_axis*sin(a_roll)*sin(a_pitch)+Y_axis*cos(a_roll)-Z_axis*sin(a_roll)*cos(a_pitch);

    heading1 = atan2(y_aux, x_aux);
    heading1 += declination_angle;

    if(heading1 < 0.0)
    {
            heading1 += (2.0 * PI);
    }

    if(heading1 > (2.0 * PI))
    {
            heading1 -= (2.0 * PI);
    }

        heading1 *= (180.0 / PI);

        return heading1;
}




