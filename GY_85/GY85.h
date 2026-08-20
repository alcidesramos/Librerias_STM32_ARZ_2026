/*
 * GY85.h
 * Created on: May 1, 2025
 * Author: ALCIDES_RAMOS
 */

#ifndef INC_GY85_H_
#define INC_GY85_H_

#include <stdint.h>
#include "main.h"
#include "kalman.h"

//colocar el i2c a usar con el imu
#define imu_i2c hi2c1;

#define TIMIMU  TIM9   //colocar timer a usar
#define htimimu htim9

//Comentar si usa timer sino poner el tiempo de muestreo
//#define  tiempo_muestreo 10.0 //tiempo de muestreo em milis



 /*
     en  MPU6050.h
  sino  usa timer definir tiempo de muestreo
      #define  tiempo_muestreo 10.0 //tiempo de muestreo em milis
   si usa timer dejar comentada la linea de ese define

   //defina i2c y timer a usar
    #define imu_i2c hi2c1
    #define TIMIMU  TIM9   //colocar timer a usar
    #define htimimu htim9

/*
  En el main
   if (GY85_Init() != 1)  uartx_write_text(&huart1, "SENSOR NO CONECTADO\r\n");

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
  #include "GY85.h"

	//mira si el timer deseado
	 if (htim->Instance == TIMIMU)
	 {

		 GY85_Read_ace();
	     GY85_Read_giro();
	   GY85_Read_bruj();
	     GY85_Ace_angulos();
	     GY85_Giro_angulos();
	     Filtro_Complementario();
         filtro_kalman();
	 }

   // o en el while si es sin timer leo los procedimeintos que necesite
   //no puede haber retardos mas alla del tiempo del muestreo en el codigo

   if (uwTick-tiempo_imu>=tiempo_muestreo)
   {

		 GY85_Read_ace();
	     GY85_Read_giro();
	   GY85_Read_bruj();
	     GY85_Ace_angulos();
	     GY85_Giro_angulos();
	     Filtro_Complementario();
         filtro_kalman();
	 		 sprintf(texto,"%.1f,%.1f,%.1f,%.1f,%.1f,%.1f\r",acel_pitch,compAngle_Pitch,kalman_pitch,acel_roll,compAngle_Roll,kalman_roll);
	 	  uartx_write_text(&huart1, texto);
	 	    tiempo_imu=uwTick;
    }


 */
extern uint32_t tiempo_imu;



extern float acel_roll,acel_pitch,giro_roll,giro_pitch,giro_yaw,ti_muestreo;
extern float compAngle_Roll, compAngle_Pitch;
extern float heading;
extern float kalman_roll,kalman_pitch;


#define ADXL345             (0x53 << 1)     /* I2C address of accelerometer */
#define ADXL345_BW_RATE     (0x2C)          /* Register controls low power and ODR/Bandwidth */
#define ADXL345_RATE_CFG    (0x0B)          /* Set output data rate to 200 Hz */
#define ADXL345_POWER_CTL   (0x2D)          /* Register controls power settings*/
#define ADXL345_EN_MSR      (0x08)          /* Enable measurement mode */
#define ADXL345_DATA_FMT    (0x31)          /* Register controls representation of data */
#define ADXL345_FMT         (0x09)          /* Enable +-4g representation,
                                               full resolution and right-justified mode */
#define ADXL345_DATA        (0x32)          /* First accelerometer output register  */
#define ADXL345_OUT_LEN     (0x06)          /* Length of all output registers */


#define ITG3205             (0x68 << 1)     /* I2C address of gyroscope */
#define ITG3205_SMPLRT_DIV  (0x15)          /* Register controls sample rate divider */
#define ITG3205_DIV         (0x04)          /* Output sampling frequency - 200 Hz */
#define ITG3205_DLPF_FS     (0x16)          /* Register controls internal digital low-pass filter
                                               and full-scale range */
#define ITG3205_FS_CFG      (0x1D)          /* Value for proper gyro operation, low-pass bandwidth - 10 Hz,
                                               internal ADC sampling rate - 1kHz */
#define ITG3205_INT         (0x17)          /* Interrupt configuration register */
#define ITG3205_DIS_INT     (0x00)          /* Disables all interrupts on gyro */
#define ITG3205_PWR         (0x3E)          /* Register for power management */
#define ITG3205_PWR_CFG     (0x00)          /* Power management value - use internal oscillator */
#define ITG3205_DATA        (0x1D)          /* First gyro output register */
#define ITG3205_OUT_LEN     (0x06)          /* Length of all output registers */
#define ITG3205_NUM_SAMPLES (20)            /* Number of test samples used to account for gyroscope offset */

//escanear i2c el magnetometro puede cambiar
#define HMC5883_             (0x1E << 1)     /* I2C address of a magnetometer */
#define HMC5883             (0x0D << 1)     /* I2C address of a magnetometer */
#define Config_Reg_A             0x00
#define Config_Reg_B             0x01
#define Mode_Reg                 0x02
#define HMC5883_MODEREG     (0x02)          /* Register that controls device operation mode */
#define HMC5883_MODECFG     (0x00)          /* Continuous measurement mode */
#define HMC5883_DATA        (0x03)          /* First magnetometer output register */
#define HMC5883_OUT_LEN     (0x06)          /* Length of all output registers */

/* Operational frequencies of devices */
#define ADXL345_FREQ        (200)
#define ITG3205_FREQ        (1000)
#define HMC58883_FREQ       (75)


//  CONSTANTE PARA PASAR A UNIDADES BASICAS CADA LECTURAS
#define ACE_SCALE           (0.004)         /* g per LSB */
#define GIRO_SCALE          (1.0 / 14.375)  /* Degrees per second per LSB */
#define BRUJ_SCALE       (1.22)          /* Gauss per LSB */


#define I2C_MEM_ADDR_SIZE_8BIT  (1u)

typedef struct {
	I2C_HandleTypeDef *hi2c;
	int16_t ace_x;
    int16_t ace_y;
    int16_t ace_z;
    int16_t giro_x;
    int16_t giro_y;
    int16_t giro_z;
    int16_t bruj_x;
    int16_t bruj_y;
    int16_t bruj_z;

    float  ace_x_f;
	float  ace_y_f;
	float  ace_z_f;
	float  giro_x_f;
	float  giro_y_f;
	float  giro_z_f;
	float  bruj_x_f;
	float  bruj_y_f;
	float  bruj_z_f;

} GY85 ;


extern GY85  imu;

void TIMERIMU_Init();
uint8_t GY85_Init();
uint8_t GY85_Read_ace();
uint8_t GY85_Read_giro();
uint8_t  GY85_Read_bruj();
void  GY85_Giro_flotantes();
void GY85_calcula_flotantes();
void GY85_Ace_angulos();
void GY85_Giro_angulos();
void Filtro_Complementario();
void  filtro_kalman();
void Bruj_heading();




#endif
