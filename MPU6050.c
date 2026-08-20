/*
 * MPU6050.c
 *
 *  Created on: May 2, 2025
 *      Author: ALCIDES_RAMOS
 */

#include "MPU6050.h"


MPU6050_ MPU6050;

extern I2C_HandleTypeDef imu_i2c;

//si manda info serial
extern UART_HandleTypeDef huart1;
char tele[20];




#ifndef tiempo_muestreo
extern TIM_HandleTypeDef htimimu;
#endif


#define RAD_TO_DEG       57.295779513f
#define DEG_TO_RAD 0.01745329251f

#define EPSILON          1e-6f // Límite para evitar divisiones por cero


#ifdef tiempo_muestreo
float muestreo=tiempo_muestreo/1000.0;//PASA A SEGUNDOS EL TIEMPO DE MUESTREO
#else
float muestreo;
#endif


float giro_sen,acel_sen;
float acel_roll, acel_pitch, giro_roll=0, giro_pitch=0, giro_yaw=0, ti_muestreo;
float compAngle_Roll, compAngle_Pitch;


float off_acel_roll=0,off_acel_pitch=0,off_acel_z=0;
int32_t	off_giro_x=0,off_giro_y=0,off_giro_z=0;


uint8_t imu_data;
uint8_t Data;
enum giro_sen_ {G_250=0,G_500=8,G_1000=16,G_2000=24};
enum acel_sen_ {A_2g=0,A_4g=8,A_8g=16,A_16g=24};




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

uint8_t MPU6050_init1()
{
	uint8_t check;
	uint8_t Data;

	// check device ID WHO_AM_I
	HAL_I2C_Mem_Read (&imu_i2c, MPU6050_ADDR, 0x75, 1, &check, 1, 1000);
	if (check == 0x68)  // 0x68 will be returned by the sensor if everything goes well
	{

		// power management register 0X6B we should write all 0's to wake the sensor up
		Data = 0;
		HAL_I2C_Mem_Write(&imu_i2c, MPU6050_ADDR, 0x6B, 1,&Data, 1, 1000);

		// Set DATA RATE of 1KHz by writing SMPLRT_DIV register
		Data = 0x07;
		HAL_I2C_Mem_Write(&imu_i2c, MPU6050_ADDR, 0x19, 1, &Data, 1, 1000);

		// Set Gyroscopic configuration in GYRO_CONFIG Register
		Data = 0x00;  // XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> ± 250 ̐/s
		HAL_I2C_Mem_Write(&imu_i2c, MPU6050_ADDR, 0x1B, 1, &Data, 1, 1000);

		// Set accelerometer configuration in ACCEL_CONFIG Register
		Data = 0x00;  // XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> ± 2g
		HAL_I2C_Mem_Write(&imu_i2c, MPU6050_ADDR, 0x1C, 1, &Data, 1, 1000);
	}

}


uint8_t MPU6050_init()

{
#ifdef tiempo_muestreo
    muestreo = tiempo_muestreo / 1000.0f;     // pasa a segundos
#else
    muestreo = (float)TIMIMU->ARR / 100000.0f;
#endif
char tele1[20];
    // 1️ Verificar comunicación
    imu_data = 0;
    HAL_I2C_Mem_Read(&imu_i2c, MPU6050_ADDR, MPU6050_WHO_AM_I, 1, &imu_data, 1, 300);
   //  if (imu_data  != 0x68)
   //     return 0;  // sensor no detectado

    // 2️ Reset del dispositivo
    imu_data = 0x80;  // bit 7 = DEVICE_RESET
    HAL_I2C_Mem_Write(&imu_i2c, MPU6050_ADDR, MPU6050_PWR_MGMT_1, 1, &imu_data, 1, 300);
    HAL_Delay(100);

    // 3️ Quita sleep y selecciona reloj del giroscopio X
    imu_data = 0x01;
    HAL_I2C_Mem_Write(&imu_i2c, MPU6050_ADDR, MPU6050_PWR_MGMT_1, 1, &imu_data, 1, 300);
    HAL_Delay(100);

    // 4️ Configura divisor de muestreo
    imu_data = 0x07;   // (1 kHz / (1 + 7)) = 125 Hz
    HAL_I2C_Mem_Write(&imu_i2c, MPU6050_ADDR, MPU6050_SMPLRT_DIV, 1, &imu_data, 1, 300);

    // 5️ Filtro digital (DLPF) para suavizar ruido
    imu_data = 0x03;   // DLPF ~44 Hz, f_s = 1 kHz
    HAL_I2C_Mem_Write(&imu_i2c, MPU6050_ADDR, MPU6050_CONFIG, 1, &imu_data, 1, 300);

    // 6️ Configura sensibilidad de acelerómetro y giroscopio
    MPU6050_SEN(G_250, A_2g);
    //MPU6050_SEN(G_500, A_2g);;


    /*
    imu_data = 0x40;   // habilita FIFO
    HAL_I2C_Mem_Write(&imu_i2c, MPU6050_ADDR, MPU6050_USER_CTRL, 1, &imu_data, 1, 300);
    imu_data = 0xF8;   // habilita guardar todo en FIFO
    HAL_I2C_Mem_Write(&imu_i2c, MPU6050_ADDR, MPU6050_FIFO_EN, 1, &imu_data, 1, 300);
    imu_data = 0x01;   // habilita interrupción de datos
    HAL_I2C_Mem_Write(&imu_i2c, MPU6050_ADDR, MPU6050_INT_ENABLE, 1, &imu_data, 1, 300);
    */

    return 1; // inicialización correcta
}

void MPU6050_lee_datos()
{
 uint8_t Dato_buff[14];
 HAL_I2C_Mem_Read(&imu_i2c, MPU6050_ADDR, MPU6050_ACCEL_XOUT_H, 1, Dato_buff, 14, HAL_MAX_DELAY);

//lee acelerometro
		MPU6050.ace_x= (int16_t)(Dato_buff[0] << 8 | Dato_buff [1]);
		MPU6050.ace_y = (int16_t)(Dato_buff[2] << 8 | Dato_buff [3]);
		MPU6050.ace_z = (int16_t)(Dato_buff[4] << 8 | Dato_buff [5]);

		//lee giros
		MPU6050.giro_x= (int16_t)(Dato_buff[8] << 8 | Dato_buff [9]);
		MPU6050.giro_y = (int16_t)(Dato_buff[10] << 8 | Dato_buff [11]);
		MPU6050.giro_z = (int16_t)(Dato_buff[12] << 8 | Dato_buff [13]);
		//lee la temperatura
		MPU6050.temperatura_raw= (int16_t)(Dato_buff[6] << 8 | Dato_buff [7]);

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
 MPU6050.ace_x = (int16_t)((uint16_t)Dato_buff[0] << 8 | Dato_buff[1]);
 MPU6050.ace_y = (int16_t)((uint16_t)Dato_buff[2] << 8 | Dato_buff[3]);
 MPU6050.ace_z = (int16_t)((uint16_t)Dato_buff[4] << 8 | Dato_buff[5]);

 MPU6050.giro_x = (int16_t)((uint16_t)Dato_buff[8] << 8 | Dato_buff[9]);
 MPU6050.giro_y = (int16_t)((uint16_t)Dato_buff[10] << 8 | Dato_buff[11]);
 MPU6050.giro_z = (int16_t)((uint16_t)Dato_buff[12] << 8 | Dato_buff[13]);

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
	float ax = MPU6050.ace_x_f;
	float ay = MPU6050.ace_y_f;
	float az = MPU6050.ace_z_f;

	float denom = sqrtf(ay*ay + az*az);
	if (denom < EPSILON) denom = EPSILON;

	acel_pitch = atan2f(ax, denom) * RAD_TO_DEG;

	denom = sqrtf(ax*ax + az*az);
	if (denom < EPSILON) denom = EPSILON;

	acel_roll = atan2f(ay, denom) * RAD_TO_DEG;

//  SI SE QUIERE COMPENSAR  OFFSET ACELELOMETRO
acel_roll-=off_acel_roll;
acel_pitch-=off_acel_pitch;
}

void MPU6050_Giro_angulos()
{
	//MPU6050_Giro_flotantes();
// Integración simple de giroscopio
giro_roll += MPU6050.giro_x_f * muestreo;
giro_pitch += MPU6050.giro_y_f *muestreo;
giro_yaw += MPU6050.giro_z_f * muestreo;



}
void MPU6050_Calibra()
{
    uint16_t conta_;

    off_giro_x = 0;
    off_giro_y = 0;
    off_giro_z = 0;

    for (conta_ = 0; conta_ < 1000; conta_++)
    {
        MPU6050_lee_datos1();

        off_giro_x += MPU6050.giro_x;
        off_giro_y += MPU6050.giro_y;
        off_giro_z += MPU6050.giro_z;
   HAL_Delay(1);

    }

    off_giro_x /= 1000;
    off_giro_y /= 1000;
    off_giro_z /= 1000;
}
void MPU6050_Calibra1()
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
void Filtro_Complementario(float alpha)
{
	  //cosntantes del filtro  peso dado al acelelometro
	   // const float alpha = 0.95;
	    compAngle_Roll = alpha * (compAngle_Roll + MPU6050.giro_x_f*muestreo) +   (1-alpha) * acel_roll;
	    compAngle_Pitch = alpha * (compAngle_Pitch + MPU6050.giro_y_f*muestreo) + (1-alpha) * acel_pitch;
	}

void  filtro_kalman()
{
	kalman_roll=Kalman_Update(&kalman_roll_, acel_roll, MPU6050.giro_x_f, muestreo);
	kalman_pitch=Kalman_Update(&kalman_pitch_, acel_pitch, MPU6050.giro_y_f, muestreo);
}
   //OPCIONAR SI SE QUIERE MANDAR INFO
//EN ESTE CASO A TELEPLOT
void MANDA_ACELERACION()
{
	sprintf(tele,">AX:%d\n",MPU6050.ace_x);
	uartx_write_text(&huart1, tele);
	sprintf(tele,">AY:%df\n",MPU6050.ace_y);
	uartx_write_text(&huart1, tele);
	sprintf(tele,">AZ:%d\n",MPU6050.ace_z);
	uartx_write_text(&huart1, tele);
}

void MANDA_GIROS()
{
	sprintf(tele,">GX:%d\n",MPU6050.giro_x);
	uartx_write_text(&huart1, tele);
	sprintf(tele,">GY:%d\n",MPU6050.giro_y);
	uartx_write_text(&huart1, tele);
	sprintf(tele,">GZ:%d\n",MPU6050.giro_z);
	uartx_write_text(&huart1, tele);
}

void MANDA_GRAVEDADES()
{
	sprintf(tele,">GRAX:%.2f\n",MPU6050.ace_x_f);
	uartx_write_text(&huart1, tele);
	sprintf(tele,">GRAY:%.2f\n",MPU6050.ace_y_f);
	uartx_write_text(&huart1, tele);
	sprintf(tele,">GRAZ:%.2f\n",MPU6050.ace_z_f);
	uartx_write_text(&huart1, tele);

}

void MANDA_VEL_ANGULAR()
{
	sprintf(tele,">VELX:%.2f\n",MPU6050.giro_y_f);
	uartx_write_text(&huart1, tele);
	sprintf(tele,">VELY:%.2f\n",MPU6050.giro_y_f);
	uartx_write_text(&huart1, tele);
	sprintf(tele,">VELZ:%.2f\n",MPU6050.giro_z_f);
	uartx_write_text(&huart1, tele);

}


void MANDA_ANG_ACEL()
{
	sprintf(tele,">PITCH_AC:%.2f\n",acel_pitch);
	uartx_write_text(&huart1, tele);
	sprintf(tele,">ROLL_AC:%.2f\n",acel_roll);
	uartx_write_text(&huart1, tele);
}

void MANDA_ANG_GIRO()
{
	sprintf(tele,">PITCH_GY:%.2f\n",giro_pitch);
	uartx_write_text(&huart1, tele);
	sprintf(tele,">ROLL_GY:%.2f\n",giro_roll);
	uartx_write_text(&huart1, tele);
	sprintf(tele,">YAW_GY:%.2f\n",giro_yaw);
	uartx_write_text(&huart1, tele);

}

void MANDA_ANG_COMPLE()
{
	sprintf(tele,">PITCH_COM:%.2f\n",compAngle_Pitch);
	uartx_write_text(&huart1, tele);
	sprintf(tele,">ROLL_COM:%.2f\n",compAngle_Roll);
	uartx_write_text(&huart1, tele);
}

void MANDA_ANG_KARMAN()
{
	sprintf(tele,">PITCH_KAR:%.2f\n",kalman_pitch);
	uartx_write_text(&huart1, tele);
	sprintf(tele,">ROLL_KAR:%.2f\n",kalman_roll);
	uartx_write_text(&huart1, tele);

}
