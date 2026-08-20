/*
 * BNO055.H
 *
 *  Created on: Oct 25, 2023
 *      Author: alcid
 */

#ifndef LIBRERIAS_BNO055_H_
#define LIBRERIAS_BNO055_H_
#include "math.h"
#include "main.h"


#define BNO055_I2C  hi2c1



// Dirección I2C del sensor BNO055  0x28 O 0x29
#define BNO055_ADD  0X28

extern uint8_t  BNO055_I2C_ADDRESS;  //colocar direccion del sensor


//ojo programar aqui orientacion del sensor var archivo BNO055.png
// Valores predeterminados para configuración y signo de ejes
#define BNO055_AXIS_MAP_CONFIG_DEFAULT  0x24
#define BNO055_AXIS_MAP_SIGN_DEFAULT    0x00


/*
 antes del while  llamar a   BNO055_INIT();

 //MODO DE OPERECION O FUSION
 // BNO055_Modo(modo_brujula);//opcional
 // BNO055_Modo(modo_giro_ace);//opcional

COLOque un
 HAL_Delay(1500);  //retardo que permite calibrar giros

  //si se quiere leer los estados de calibracion donde  3 es ok.

  BNO055_Lee_calibracion();
  sprintf(texto,"cal_gir=%u\r\n",cali_giro);
  uartx_write_text(&huart1, texto);
  sprintf(texto,"cal_acel=%u\r\n",cali_ace);
  uartx_write_text(&huart1, texto);
  sprintf(texto,"cal_mag=%u\r\n",cali_mag);
   uartx_write_text(&huart1, texto);



 en el while
 llamar funciones deseadas, ejemplo
 BNO055_ReadEuler();


OJO:
SI TIENE OTRO SENSORES CONECTADO Y QUIERE LEER UNO O EL OTRO
ANTES DE LLAMAR CADA FUNCION DEBE PONER  \
BNO055_I2C_ADDRESS=0x29<<1;  O
BNO055_I2C_ADDRESS=0x28<<1;  DEPENDIENDO DEL SENSOR DESEADO
 */

//acelelaciones
extern int16_t ax_raw, ay_raw,az_raw;
extern float ax,ay,az;
//aceleraciones lineal
extern float alx,aly,alz;

//variables de los giros
extern int16_t gx_raw,gy_raw,gz_raw;
extern float gx,gy,gz;

//magnetometros
extern int16_t max_raw,may_raw,maz_raw;
extern float max,may,maz;





//angulos de euler
extern float pitch,roll,yaw;
//quaternion
extern float q1,q2,q3,q4;
extern float pitch1,roll1,yaw1;

extern float pitch_offset,roll_offset;

//calibraciones
extern uint8_t cali_sys,cali_giro,cali_ace,cali_mag ;

//promedios
extern int16_t     promedio_raw_ax,promedio_raw_ay,promedio_raw_az,
	   	   	   promedio_raw_gx,promedio_raw_gy,promedio_raw_gz,
			   promedio_raw_max,promedio_raw_may,promedio_raw_maz;


// Registros de control
extern I2C_HandleTypeDef BNO055_I2C ;

//modos del yaw
#define modo_brujula  0x0c
#define modo_giro_ace     0x08

#define BNO055_PAGE_ID          0x07
#define BNO055_SYS_TRIGGER      0x3F
#define BNO055_PWR_MODE         0x3E

// Registros de calibración
#define BNO055_ACC_OFFSET_X_LSB 0x55
#define BNO055_ACC_OFFSET_X_MSB 0x56
#define BNO055_ACC_OFFSET_Y_LSB 0x57
#define BNO055_ACC_OFFSET_Y_MSB 0x58
#define BNO055_ACC_OFFSET_Z_LSB 0x59
#define BNO055_ACC_OFFSET_Z_MSB 0x5A
// Continúa con los registros de calibración para el magnetómetro y el giroscopio.

// Registros de datos de sensores
#define BNO055_TEMP             0x34
#define BNO055_ACC_DATA_X_LSB   0x08
#define BNO055_ACC_DATA_X_MSB   0x09
#define BNO055_ACC_DATA_Y_LSB   0x0A
#define BNO055_ACC_DATA_Y_MSB   0x0B
#define BNO055_ACC_DATA_Z_LSB   0x0C
#define BNO055_ACC_DATA_Z_MSB   0x0D


//registro para la aceleracion lineal
#define BNO055_LINEAR_ACC_DATA_X_LSB   0x28
#define BNO055_LINEAR_ACC_DATA_X_MSB   0x29
#define BNO055_LINEAR_ACC_DATA_Y_LSB   0x2A
#define BNO055_LINEAR_ACC_DATA_Y_MSB   0x2B
#define BNO055_LINEAR_ACC_DATA_Z_LSB   0x2C
#define BNO055_LINEAR_ACC_DATA_Z_MSB   0x2D

//registro para las gravedades
#define BNO055_GRAVITY_DATA_X_LSB   0x2E
#define BNO055_GRAVITY_DATA_X_MSB   0x2F
#define BNO055_GRAVITY_DATA_Y_LSB   0x30
#define BNO055_GRAVITY_DATA_Y_MSB   0x31
#define BNO055_GRAVITY_DATA_Z_LSB   0x32
#define BNO055_GRAVITY_DATA_Z_MSB   0x33


// Registros de datos del giroscopio
#define BNO055_GYRO_DATA_X_LSB 0x14
#define BNO055_GYRO_DATA_X_MSB 0x15
#define BNO055_GYRO_DATA_Y_LSB 0x16
#define BNO055_GYRO_DATA_Y_MSB 0x17
#define BNO055_GYRO_DATA_Z_LSB 0x18
#define BNO055_GYRO_DATA_Z_MSB 0x19

// Registros de datos del magnetómetro
#define BNO055_MAG_DATA_X_LSB 0x0E
#define BNO055_MAG_DATA_X_MSB 0x0F
#define BNO055_MAG_DATA_Y_LSB 0x10
#define BNO055_MAG_DATA_Y_MSB 0x11
#define BNO055_MAG_DATA_Z_LSB 0x12
#define BNO055_MAG_DATA_Z_MSB 0x13


// Registros de orientación (Euler)
#define BNO055_EUL_HEADING_LSB  0x1A
#define BNO055_EUL_HEADING_MSB  0x1B
#define BNO055_EUL_ROLL_LSB     0x1C
#define BNO055_EUL_ROLL_MSB     0x1D
#define BNO055_EUL_PITCH_LSB    0x1E
#define BNO055_EUL_PITCH_MSB    0x1F

// Registros de datos de cuaterniones
#define BNO055_QUATERNION_DATA_W_LSB 0x20
#define BNO055_QUATERNION_DATA_W_MSB 0x21
#define BNO055_QUATERNION_DATA_X_LSB 0x22
#define BNO055_QUATERNION_DATA_X_MSB 0x23
#define BNO055_QUATERNION_DATA_Y_LSB 0x24
#define BNO055_QUATERNION_DATA_Y_MSB 0x25
#define BNO055_QUATERNION_DATA_Z_LSB 0x26
#define BNO055_QUATERNION_DATA_Z_MSB 0x27

// registros de estados
#define BNO055_CALIB_STAT       0x35
#define BNO055_SELFTEST = 0X36
#define BNO055_INTR_STAT = 0X37
#define BNO055_SYS_STATUS       0x39

// Registros de configuración de ejes
#define BNO055_AXIS_MAP_CONFIG  0x41
#define BNO055_AXIS_MAP_SIGN    0x42



//registos de configuracion imu
#define  BNO055_ACC_CONFIG 0x08
#define  BNO055_OPR_MODE 0x3D

#define BNO055_GYRO_CONFIG0 0x0A
#define BNO055_GYRO_CONFIG1 0x0B

// Definiciones de escalas para el acelerómetro
#define ACCEL_SCALE_2G  0x00  // ±2g
#define ACCEL_SCALE_4G  0x01  // ±4g
#define ACCEL_SCALE_8G  0x02  // ±8g
#define ACCEL_SCALE_16G 0x03  // ±16g

// Definiciones de escalas para el giroscopio
#define GYRO_SCALE_2000DPS  0x00  // ±2000 grados por segundo
#define GYRO_SCALE_1000DPS  0x01  // ±1000 grados por segundo
#define GYRO_SCALE_500DPS   0x02  // ±500 grados por segundo
#define GYRO_SCALE_250DPS   0x03  // ±250 grados por segundo
#define GYRO_SCALE_125DPS   0x04  // ±125 grados por segundo
//offset
#define BNO055_ACC_OFFSET_X_LSB 0x55 // Registros para offset del acelerómetro
#define BNO055_ACC_OFFSET_X_MSB 0x56
#define BNO055_ACC_OFFSET_Y_LSB 0x57
#define BNO055_ACC_OFFSET_Y_MSB 0x58
#define BNO055_ACC_OFFSET_Z_LSB 0x59
#define BNO055_ACC_OFFSET_Z_MSB 0x5A

#define BNO055_GYR_OFFSET_X_LSB 0x67 // Registros para offset del giroscopio
#define BNO055_GYR_OFFSET_X_MSB 0x68
#define BNO055_GYR_OFFSET_Y_LSB 0x69
#define BNO055_GYR_OFFSET_Y_MSB 0x6A
#define BNO055_GYR_OFFSET_Z_LSB 0x6B
#define BNO055_GYR_OFFSET_Z_MSB 0x6C

#define BNO055_MAG_OFFSET_X_LSB 0x71 // Registros para offset del magnetómetro
#define BNO055_MAG_OFFSET_X_MSB 0x72
#define BNO055_MAG_OFFSET_Y_LSB 0x73
#define BNO055_MAG_OFFSET_Y_MSB 0x74
#define BNO055_MAG_OFFSET_Z_LSB 0x75
#define BNO055_MAG_OFFSET_Z_MSB 0x76

uint8_t BNO055_INIT();
void BNO055_Modo(uint8_t mode);
void BNO055_Lee_calibracion();
uint8_t BNO055_Calibra(uint8_t modo,uint16_t tiempo);
void BNO055_Lee_Aceleracion();
void BNO055_Lee_Aceleracion_lineal();
void BNO055_Lee_Magnetometro();
void BNO055_Lee_Giro();
void BNO055_Lee_Euler();
void BNO055_Lee_Quaternion();
void QuaternionToEulerAngles();
void BNO055_Offset_Carga();
void BNO055_Offset_Carga_Promedio(uint16_t cuantos);





#endif /* LIBRERIAS_BNO055_H_ */
