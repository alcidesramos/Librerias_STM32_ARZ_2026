/*
 * MPU6050.h
 *
 *  Created on: May 2, 2025
 *      Author: ALCIDES_RAMOS
 */

#ifndef LIBRERIAS_MPU6050_H_
#define LIBRERIAS_MPU6050_H_
#include "main.h"
#include "kalman.h"

//colocar el i2c a usar con el imu
#define imu_i2c hi2c1

#define TIMIMU  TIM9   //colocar timer a usar
#define htimimu htim9


//Comentar si usa timer sino poner el tiempo de muestreo
//#define  tiempo_muestreo 10.0 //tiempo de muestreo em milis


extern uint32_t tiempo_imu;

extern float acel_roll, acel_pitch, giro_roll, giro_pitch, giro_yaw, ti_muestreo;
extern float compAngle_Roll, compAngle_Pitch;


extern float off_acel_roll,off_acel_pitch,off_acel_z;
extern int32_t off_giro_x,off_giro_y,off_giro_z;

/*   en  MPU6050.h
  sino  usa timer definir tiempo de muestreo
      #define  tiempo_muestreo 10.0 //tiempo de muestreo em milis
   si usa timer dejar comentada la linea de ese define

   //defina i2c y timer a usar
    #define imu_i2c hi2c1
    #define TIMIMU  TIM9   //colocar timer a usar
    #define htimimu htim9


  En el main
    if (MPU6050_init()==1)  uartx_write_text(&huart1, "SENSOR OKS\r\n");
     MPU6050_Calibra();

  //si usa  kalman.h
   Kalman_Init(&kalman_roll_);
  Kalman_Init(&kalman_pitch_);

  //si usa timer para  muestreo inicielo
   TIMERIMU_Init();+
   //sino use tiempo_imu
    tiempo_imu=uwTick;

  en el while
  automaticamente tiene los datos que desee
  acel_pitch,compAngle_Pitch,kalman_pitch,acel_roll,compAngle_Roll,kalman_roll

   // en el timer.h
    #include "MPU6050.h"

	//mira si el timer deseado
	 if (htim->Instance == TIMIMU)
	 {
		 //LEE LOS SENSORES
	    MPU6050_lee_datos();
	    MPU6050_Ace_angulos();
	    MPU6050_Giro_angulos();
	    filtro_kalman();
	    Filtro_Complementario();
	 }

   // o en el while si es sin timer leo los procedimeintos que necesite
   //no puede haber retardos mas alla del tiempo del muestreo en el codigo

   if (uwTick-tiempo_imu>=tiempo_muestreo)
   {

	       MPU6050_lee_datos();
	 	    MPU6050_Ace_angulos();
	 	    MPU6050_Giro_angulos();
	 	    filtro_kalman();
	 	    Filtro_Complementario();
	 		 sprintf(texto,"%.1f,%.1f,%.1f,%.1f,%.1f,%.1f\r",acel_pitch,compAngle_Pitch,kalman_pitch,acel_roll,compAngle_Roll,kalman_roll);
	 	  uartx_write_text(&huart1, texto);
	 	    tiempo_imu=uwTick;
    }


 */


/* Dirección de defecto */
#define MPU6050_ADDR  		  		0x68<<1
#define MPU6050_AUX_VDDIO			0x01
#define MPU6050_SMPLRT_DIV			0x19
#define MPU6050_CONFIG				0x1A
#define MPU6050_GYRO_CONFIG			0x1B
#define MPU6050_ACCEL_CONFIG		0x1C
#define MPU6050_MOTION_THRESH		0x1F
#define MPU6050_FIFO_EN         	0x23
#define MPU6050_INT_PIN_CFG			0x37
#define MPU6050_INT_ENABLE			0x38
#define MPU6050_INT_STATUS			0x3A
#define MPU6050_ACCEL_XOUT_H		0x3B
#define MPU6050_ACCEL_XOUT_L		0x3C
#define MPU6050_ACCEL_YOUT_H		0x3D
#define MPU6050_ACCEL_YOUT_L		0x3E
#define MPU6050_ACCEL_ZOUT_H		0x3F
#define MPU6050_ACCEL_ZOUT_L		0x40
#define MPU6050_TEMP_OUT_H			0x41
#define MPU6050_TEMP_OUT_L			0x42
#define MPU6050_GYRO_XOUT_H			0x43
#define MPU6050_GYRO_XOUT_L			0x44
#define MPU6050_GYRO_YOUT_H			0x45
#define MPU6050_GYRO_YOUT_L			0x46
#define MPU6050_GYRO_ZOUT_H			0x47
#define MPU6050_GYRO_ZOUT_L			0x48
#define MPU6050_MOT_DETECT_STATUS	0x61
#define MPU6050_SIGNAL_PATH_RESET	0x68
#define MPU6050_MOT_DETECT_CTRL		0x69
#define MPU6050_USER_CTRL			0x6A
#define MPU6050_PWR_MGMT_1			0x6B
#define MPU6050_PWR_MGMT_2			0x6C
#define MPU6050_FIFO_COUNTH			0x72
#define MPU6050_FIFO_COUNTL			0x73
#define MPU6050_FIFO_R_W			0x74
#define MPU6050_WHO_AM_I			0x75
typedef struct
{
	int16_t ace_x;
	int16_t ace_y;
	int16_t ace_z;
	int16_t temperatura_raw;
	int16_t giro_x;
	int16_t giro_y;
	int16_t giro_z;

	float  ace_x_f;
	float  ace_y_f;
	float  ace_z_f;
	float  giro_x_f;
	float  giro_y_f;
	float  giro_z_f;
	float temperatura;

} MPU6050_;

extern MPU6050_ MPU6050;

void TIMERIMU_Init();
uint8_t MPU6050_init();
void MPU6050_lee_datos();
void MPU6050_Calibra();
void  MPU6050_Giro_flotantes();
void MPU6050_calcula_flotantes();
void MPU6050_Ace_angulos();
void MPU6050_Giro_angulos();
void Filtro_Complementario();
void  filtro_kalman();

#endif /* LIBRERIAS_MPU6050_H_ */
