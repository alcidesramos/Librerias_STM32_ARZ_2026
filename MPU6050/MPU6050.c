/*
 * MPU6050.c
 *
 *  Created on: May 2, 2025
 *      Author: ALCIDES_RAMOS
 */

#include "MPU6050.h"
#include "kalman.h"
#include "math.h"

MPU6050_ MPU6050;

extern I2C_HandleTypeDef imu_i2c;
extern TIM_HandleTypeDef htimimu;

#define RAD_TO_DEG       57.295779513f
#define DEG_TO_RAD 0.01745329251f

#define EPSILON          1e-6f // Límite para evitar divisiones por cero


//float muestreo=tiempo_muestreo/1000.0;//PASA A SEGUNDOS EL TIEMPO DE MUESTREO
float muestreo;
uint32_t tiempo_imu;



float giro_sen,acel_sen;
float acel_roll, acel_pitch, giro_roll=0, giro_pitch=0, giro_yaw=0, ti_muestreo;
float compAngle_Roll, compAngle_Pitch;

float off_acel_roll=0,off_acel_pitch=0,off_acel_z=0;
int32_t	off_giro_x=0,off_giro_y=0,off_giro_z=0;


uint8_t imu_data;
uint8_t Data;
enum giro_sen_ {G_250=0,G_500=8,G_1000=16,G_2000=24};
enum acel_sen_ {A_2g=0,A_4g=8,A_8g=16,A_16g=24};


void TIMERIMU_Init()
{
#ifndef tiempo_muestreo
HAL_TIM_Base_Start_IT(&htimimu);
#endif
}
void MPU6050_SEN(char giro, char acel)
{

 switch (giro)
  {
    case G_250:giro_sen=131.0;
                   break;
    case G_500:giro_sen=65.5;
                   break;
    case G_1000:giro_sen=32.8;
                   break;
    case G_2000:giro_sen=16.4;
                   break;

  }

  switch (acel)

  {
    case A_2g:acel_sen=16384.0;
                   break;
    case A_4g:acel_sen=8192.0;
                   break;
    case A_8g:acel_sen=4096.0;
                       break;
    case A_16g:acel_sen=2048.0;
                   break;

  }
   //configura el giro a la sensibilidad deseada
  imu_data = giro;
  HAL_I2C_Mem_Write(&imu_i2c, MPU6050_ADDR, MPU6050_GYRO_CONFIG, 1, &imu_data, 1, 300);

   //configura el acelerometro a la sensibilidad deseada
   imu_data = acel;
     HAL_I2C_Mem_Write(&imu_i2c, MPU6050_ADDR, MPU6050_ACCEL_CONFIG, 1, &imu_data, 1, 300);



}
uint8_t MPU6050_init()
{
#ifdef tiempo_muestreo
muestreo=tiempo_muestreo/1000.0;//PASA A SEGUNDOS EL TIEMPO DE MUESTREO
#else
muestreo =(float)TIMIMU->ARR/1000000.0;
#endif

	imu_data=0;
	HAL_I2C_Mem_Read (&imu_i2c, MPU6050_ADDR,MPU6050_WHO_AM_I,1, &imu_data, 1, 300);

	if (imu_data!=0x68) return(0);

//RESETEA
    	imu_data = 0X40;
		HAL_I2C_Mem_Write(&imu_i2c, MPU6050_ADDR, MPU6050_PWR_MGMT_1, 1,&imu_data, 1, 300);
         HAL_Delay(200);
	//LImpi registo de power para levantar el imu
	imu_data = 0;
	HAL_I2C_Mem_Write(&imu_i2c, MPU6050_ADDR, MPU6050_PWR_MGMT_1, 1,&imu_data, 1, 300);
	HAL_Delay(200);

	//muestreo a 1K
	imu_data = 0x07;//  si coloca 0 son 8khz
   HAL_I2C_Mem_Write(&imu_i2c, MPU6050_ADDR, MPU6050_SMPLRT_DIV, 1, &imu_data, 1, 300);
   //configura la sensibilidad de los sensores
   MPU6050_SEN(G_250,A_2g);

   //Habilita BUFFER FIFO
   //imu_data=0x40;
   //HAL_I2C_Mem_Write(&imu_i2c, MPU6050_ADDR,MPU6050_USER_CTRL , 1, &imu_data, 1, 300);

   //Habilita Guardar datos en  BUFFER FIFO
     //imu_data=0xF8;
     //HAL_I2C_Mem_Write(&imu_i2c, MPU6050_ADDR,MPU6050_FIFO_EN , 1, &imu_data, 1, 300);

    //Habilita interrupciones si se desea
      //imu_data=0x01;
      //HAL_I2C_Mem_Write(&imu_i2c, MPU6050_ADDR,MPU6050_INT_ENABLE , 1, &imu_data, 1, 300);

	return(1);

}
void MPU6050_lee_datos()
{
 uint8_t Dato_buff[14];
 HAL_I2C_Mem_Read(&imu_i2c, MPU6050_ADDR, MPU6050_ACCEL_XOUT_H, 1, Dato_buff, 14, HAL_MAX_DELAY);

//lee acelerometro
		MPU6050.ace_x= (uint16_t)(Dato_buff[0] << 8 | Dato_buff [1]);
		MPU6050.ace_y = (uint16_t)(Dato_buff[2] << 8 | Dato_buff [3]);
		MPU6050.ace_z = (uint16_t)(Dato_buff[4] << 8 | Dato_buff [5]);

		//lee giros
		MPU6050.giro_x= (uint16_t)(Dato_buff[8] << 8 | Dato_buff [9]);
		MPU6050.giro_y = (uint16_t)(Dato_buff[10] << 8 | Dato_buff [11]);
		MPU6050.giro_z = (uint16_t)(Dato_buff[12] << 8 | Dato_buff [13]);
		//lee la temperatura
		MPU6050.temperatura_raw= (uint16_t)(Dato_buff[6] << 8 | Dato_buff [7]);

     //ajuste de offset
/*		MPU6050.ace_x-=off_acel_roll;
		MPU6050.ace_y-=off_acel_pitch;
		MPU6050.ace_z-=off_acel_z;
*/

		MPU6050.giro_x-=off_giro_x;
		MPU6050.giro_y-=off_giro_y;
		MPU6050.giro_z-=off_giro_z;

}
void MPU6050_lee_datos1()
{
 uint8_t Dato_buff[14];
 HAL_I2C_Mem_Read(&imu_i2c, MPU6050_ADDR, MPU6050_ACCEL_XOUT_H, 1, Dato_buff, 14, HAL_MAX_DELAY);

//lee acelerometro
		MPU6050.ace_x= (uint16_t)(Dato_buff[0] << 8 | Dato_buff [1]);
		MPU6050.ace_y = (uint16_t)(Dato_buff[2] << 8 | Dato_buff [3]);
		MPU6050.ace_z = (uint16_t)(Dato_buff[4] << 8 | Dato_buff [5]);

		//lee giros
		MPU6050.giro_x= (uint16_t)(Dato_buff[8] << 8 | Dato_buff [9]);
		MPU6050.giro_y = (uint16_t)(Dato_buff[10] << 8 | Dato_buff [11]);
		MPU6050.giro_z = (uint16_t)(Dato_buff[12] << 8 | Dato_buff [13]);

}



void  MPU6050_Giro_flotantes()
{
//pasando a grados por segundo
	MPU6050.giro_x_f= (float)MPU6050.giro_x/giro_sen;
    MPU6050.giro_y_f= (float)MPU6050.giro_y/giro_sen;
    MPU6050.giro_z_f= (float)MPU6050.giro_z/giro_sen;
}

void MPU6050_calcula_flotantes()
{

MPU6050.ace_x_f=(float) MPU6050.ace_x/acel_sen;
MPU6050.ace_y_f=(float) MPU6050.ace_y/acel_sen;
MPU6050.ace_z_f=(float) MPU6050.ace_z/acel_sen;//14418.0;

MPU6050.giro_x_f= (float)MPU6050.giro_x/giro_sen;
MPU6050.giro_y_f= (float)MPU6050.giro_y/giro_sen;
MPU6050.giro_z_f= (float)MPU6050.giro_z/giro_sen;
MPU6050.temperatura= ((float)MPU6050.temperatura_raw/340.0)  +36.54;     // ecuación de la temperatura

}

void MPU6050_Ace_angulos()
{
float denom = sqrtf(MPU6050.ace_y * MPU6050.ace_y + MPU6050.ace_z * MPU6050.ace_z);
if (denom < EPSILON) denom = EPSILON;

acel_pitch = atan2f(MPU6050.ace_x, denom) * RAD_TO_DEG;

denom = sqrtf(MPU6050.ace_x * MPU6050.ace_x + MPU6050.ace_z * MPU6050.ace_z);
if (denom < EPSILON) denom = EPSILON;

acel_roll = atan2f(MPU6050.ace_y, denom) * RAD_TO_DEG;

//  SI SE QUIERE COMPENSAR  OFFSET ACELELOMETRO
acel_roll-=off_acel_roll;
acel_pitch-=off_acel_pitch;
}

void MPU6050_Giro_angulos()
{
	MPU6050_Giro_flotantes();
// Integración simple de giroscopio (Euler)
giro_roll += MPU6050.giro_x_f * muestreo;
giro_pitch += MPU6050.giro_y_f *muestreo;
giro_yaw += MPU6050.giro_z_f * muestreo;

}

void MPU6050_Calibra()
{
uint16_t conta_ ;


  off_acel_roll=0;
  off_acel_pitch=0;
  off_acel_z=0;
  off_giro_x=0;
  off_giro_y=0;
  off_giro_z=0;
 for (conta_=0;conta_<100;conta_++)
 {
	 HAL_Delay(20);
        MPU6050_lee_datos1();
        MPU6050_Ace_angulos();

        //EL ACELELOMETRO SOBRE ANGULOS Y GIRO SOBRE DATOS CRUDOS

     off_acel_roll+=acel_roll;
     off_acel_pitch+=acel_pitch;

     off_giro_x+=MPU6050.giro_x;
     off_giro_y+=MPU6050.giro_y;
     off_giro_z+=MPU6050.giro_z;

 }
 /* off_acel_roll=off_acel_roll/100;
  off_acel_pitch=off_acel_pitch/100;
  off_acel_z=off_acel_z/100;
*/
  off_giro_x=off_giro_x/100;
  off_giro_y=off_giro_y/100;
  off_giro_z=off_giro_z/100;
//     off_acel_roll=off_acel_pitch=off_acel_z=off_giro_x=off_giro_y=off_giro_z=0;
}
void Filtro_Complementario()
{
	  //cosntantes del filtro  peso dado al acelelometro
	    const float alpha = 0.95;
	    compAngle_Roll = alpha * (compAngle_Roll + MPU6050.giro_x_f*muestreo) +   (1-alpha) * acel_roll;
	    compAngle_Pitch = alpha * (compAngle_Pitch + MPU6050.giro_y_f*muestreo) + (1-alpha) * acel_pitch;
	}

void  filtro_kalman()
{
	kalman_roll=Kalman_Update(&kalman_roll_, acel_roll, MPU6050.giro_x_f, muestreo);
	kalman_pitch=Kalman_Update(&kalman_pitch_, acel_pitch, MPU6050.giro_y_f, muestreo);
}

