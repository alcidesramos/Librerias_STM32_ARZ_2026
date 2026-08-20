/*
 * HMC5883.h
 *
 *  Created on: 15/11/2021
 *      Author: Alcides Ramos
 */


#define HMC5883L_WRITE_ADDR      0x3C
#define HMC5883L_READ_ADDR       0x3D

#define Config_Reg_A             0x00
#define Config_Reg_B             0x01
#define Mode_Reg                 0x02
#define X_MSB_Reg                0x03
#define X_LSB_Reg                0x04
#define Z_MSB_Reg                0x05
#define Z_LSB_Reg                0x06
#define Y_MSB_Reg                0x07
#define Y_LSB_Reg                0x08
#define Status_Reg               0x09
#define ID_Reg_A                 0x0A
#define ID_Reg_B                 0x0B
#define ID_Reg_C                 0x0C

#define PI                      3.141592
#define declination_angle  		 0 //  (-7.0 +  6.0/60.0)        //Declinacion Cartagena -7°.6'


void HMC5883L_init(float escala_gauss);

float HMC5883L_heading();
float HMC5883L_heading_compen(float a_roll,float a_pitch);




