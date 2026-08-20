/*
 * GY85.c
 * Created on: May 1, 2025
 * Author: ALCIDES_RAMOS
 */

#include <string.h>
#include "GY85.h"
#include "math.h"

extern I2C_HandleTypeDef imu_i2c;
GY85 imu;

#define RAD_TO_DEG       57.295779513f
#define DEG_TO_RAD 0.01745329251f

#define EPSILON          1e-6f // Límite para evitar divisiones por cero


float muestreo=tiempo_muestreo/1000.0;//PASA A SEGUNDOS EL TIEMPO DE MUESTREO


static int32_t x_offset;
static int32_t y_offset;
static int32_t z_offset;
float acel_roll, acel_pitch, giro_roll=0, giro_pitch=0, giro_yaw=0, ti_muestreo;
float compAngle_Roll, compAngle_Pitch;
float heading;

 uint8_t init_ace()
{
    uint8_t tmp = ADXL345_FMT;
    imu.ace_x = imu.ace_y = imu.ace_z = 0;

    // Configurar resolución y formato del acelerómetro
    if (HAL_I2C_Mem_Write(imu.hi2c, ADXL345, ADXL345_DATA_FMT, I2C_MEM_ADDR_SIZE_8BIT, &tmp, 1, HAL_MAX_DELAY) != HAL_OK)
        return 0;

    tmp = ADXL345_RATE_CFG;
    // Establecer frecuencia de muestreo a 200 Hz
    if (HAL_I2C_Mem_Write(imu.hi2c, ADXL345, ADXL345_BW_RATE, I2C_MEM_ADDR_SIZE_8BIT, &tmp, 1, HAL_MAX_DELAY) != HAL_OK)
        return 0;

    tmp = ADXL345_EN_MSR;
    // Habilitar mediciones
    if (HAL_I2C_Mem_Write(imu.hi2c, ADXL345, ADXL345_POWER_CTL, I2C_MEM_ADDR_SIZE_8BIT, &tmp, 1, HAL_MAX_DELAY) != HAL_OK)
        return 0;

    return 1;
}

 void calibrate_offset()
{
    int32_t x_tmp = 0, y_tmp = 0, z_tmp = 0;

    for (uint8_t i = 0; i < ITG3205_NUM_SAMPLES; i++) {
        HAL_Delay(50);
        GY85_Read_giro();
        x_tmp += imu.giro_x;
        y_tmp += imu.giro_y;
        z_tmp += imu.giro_z;
    }

    x_offset = x_tmp / ITG3205_NUM_SAMPLES;
    y_offset = y_tmp / ITG3205_NUM_SAMPLES;
    z_offset = z_tmp / ITG3205_NUM_SAMPLES;
}

uint8_t init_giro()
{
    uint8_t tmp = ITG3205_DIV;
    imu.giro_x = imu.giro_y = imu.giro_z = 0;

    // Configurar divisor de muestreo
    if (HAL_I2C_Mem_Write(imu.hi2c, ITG3205, ITG3205_SMPLRT_DIV, I2C_MEM_ADDR_SIZE_8BIT, &tmp, 1, HAL_MAX_DELAY) != HAL_OK)
        return 0;

    tmp = ITG3205_FS_CFG;
    // Configurar frecuencia de corte y escala completa
    if (HAL_I2C_Mem_Write(imu.hi2c, ITG3205, ITG3205_DLPF_FS, I2C_MEM_ADDR_SIZE_8BIT, &tmp, 1, HAL_MAX_DELAY) != HAL_OK)
        return 0;

    tmp = ITG3205_DIS_INT;
    // Deshabilitar interrupciones
    if (HAL_I2C_Mem_Write(imu.hi2c, ITG3205, ITG3205_INT, I2C_MEM_ADDR_SIZE_8BIT, &tmp, 1, HAL_MAX_DELAY) != HAL_OK)
        return 0;

    tmp = ITG3205_PWR_CFG;
    // Usar oscilador interno como fuente de reloj
    if (HAL_I2C_Mem_Write(imu.hi2c, ITG3205, ITG3205_PWR, I2C_MEM_ADDR_SIZE_8BIT, &tmp, 1, HAL_MAX_DELAY) != HAL_OK)
        return 0;

    calibrate_offset();

    return 1;
}

uint8_t init_bruj()
{
/*	uint8_t tmp = 0x70;
    imu.bruj_x = imu.bruj_y = imu.bruj_z = 0;

    // Configurar brújula en modo continuo
    if (HAL_I2C_Mem_Write(imu.hi2c, HMC5883,Config_Reg_A, I2C_MEM_ADDR_SIZE_8BIT, &tmp, 1, HAL_MAX_DELAY) != HAL_OK)
    	return 0;

    tmp = 0xA0;

     if (HAL_I2C_Mem_Write(imu.hi2c, HMC5883, Config_Reg_B, I2C_MEM_ADDR_SIZE_8BIT, &tmp, 1, HAL_MAX_DELAY) != HAL_OK)
    	return 0;
    tmp = 0x00;
       if (HAL_I2C_Mem_Write(imu.hi2c, HMC5883, Mode_Reg , I2C_MEM_ADDR_SIZE_8BIT, &tmp, 1, HAL_MAX_DELAY) != HAL_OK)
       	return 0;

       //pone la escala
          tmp = 0x02;
          if (HAL_I2C_Mem_Write(imu.hi2c, HMC5883, Config_Reg_B, I2C_MEM_ADDR_SIZE_8BIT, &tmp, 1, HAL_MAX_DELAY) != HAL_OK)
        	  return 0;

    return 1;
*/
	uint8_t tmp = HMC5883_MODECFG;

	   imu.bruj_x =imu.bruj_y =imu.bruj_z = 0;

	   if (HAL_I2C_Mem_Write(imu.hi2c, HMC5883, HMC5883_MODEREG, I2C_MEM_ADDR_SIZE_8BIT, &tmp, 1, HAL_MAX_DELAY) != HAL_OK)
	    return 0;

	    return 1;

}

uint8_t GY85_Init()
{
    imu.hi2c = &imu_i2c;
    if (!init_ace()) return 0;
    if (!init_giro()) return 0;
    if (!init_bruj()) return 0;

    return 1;
}

uint8_t GY85_Read_ace()
{
    uint8_t reg_addr = ADXL345_DATA;
    uint8_t tmp[ADXL345_OUT_LEN];


    if (HAL_I2C_Master_Transmit(imu.hi2c, ADXL345, &reg_addr, 1, HAL_MAX_DELAY) != HAL_OK)
        return 0;

    if (HAL_I2C_Master_Receive(imu.hi2c, ADXL345, tmp, ADXL345_OUT_LEN, HAL_MAX_DELAY) != HAL_OK)
        return 0;

    imu.ace_x = (tmp[1] << 8) | tmp[0];
    imu.ace_y = (tmp[3] << 8) | tmp[2];
    imu.ace_z = (tmp[5] << 8) | tmp[4];

    return 1;
}
void  GY85_Giro_flotantes()
{
//pasando a grados por segundo
	imu.giro_x_f=imu.giro_x*GIRO_SCALE;
	imu.giro_y_f=imu.giro_y*GIRO_SCALE;
	imu.giro_z_f=imu.giro_z*GIRO_SCALE;

}
uint8_t GY85_Read_giro()
{
    uint8_t reg_addr = ITG3205_DATA;
    uint8_t tmp[ITG3205_OUT_LEN];

    if (HAL_I2C_Master_Transmit(imu.hi2c, ITG3205, &reg_addr, 1, HAL_MAX_DELAY) != HAL_OK)
        return 0;

    if (HAL_I2C_Master_Receive(imu.hi2c, ITG3205, tmp, ITG3205_OUT_LEN, HAL_MAX_DELAY) != HAL_OK)
        return 0;

    imu.giro_x = ((tmp[0] << 8) | tmp[1]) - x_offset;
    imu.giro_y = ((tmp[2] << 8) | tmp[3]) - y_offset;
    imu.giro_z = ((tmp[4] << 8) | tmp[5]) - z_offset;
    GY85_Giro_flotantes();
    return 1;
}

uint8_t GY85_Read_bruj()
{
    uint8_t reg_addr = HMC5883_DATA;
    uint8_t tmp[HMC5883_OUT_LEN];
/*
    if (HAL_I2C_Master_Transmit(imu.hi2c, HMC5883, &reg_addr, 1, HAL_MAX_DELAY) != HAL_OK)
        return 0;

    if (HAL_I2C_Master_Receive(imu.hi2c, HMC5883, tmp, HMC5883_OUT_LEN, HAL_MAX_DELAY) != HAL_OK)
        return 0;
*/
    HAL_I2C_Mem_Read(imu.hi2c,HMC5883, reg_addr, I2C_MEMADD_SIZE_8BIT, tmp, HMC5883_OUT_LEN, HAL_MAX_DELAY);
    imu.bruj_x = (tmp[0] << 8) | tmp[1];
    imu.bruj_z = (tmp[2] << 8) | tmp[3];
    imu.bruj_y = (tmp[4] << 8) | tmp[5];

    return 1;
}


void GY85_calcula_flotantes()
{
	//pasando gravedades
	imu.ace_x_f=imu.ace_x*ACE_SCALE;
	imu.ace_y_f=imu.ace_y*ACE_SCALE;
	imu.ace_z_f=imu.ace_z*ACE_SCALE;

   //pasando a grados por segundo
	imu.giro_x_f=imu.giro_x*GIRO_SCALE;
	imu.giro_y_f=imu.giro_y*GIRO_SCALE;
	imu.giro_z_f=imu.giro_z*GIRO_SCALE;

	//pasando a gauss
	imu.bruj_x_f=imu.bruj_x*BRUJ_SCALE;
	imu.bruj_y_f=imu.bruj_y*BRUJ_SCALE;
	imu.bruj_z_f=imu.bruj_z*BRUJ_SCALE;


}



	void GY85_Ace_angulos()
{
    float denom = sqrtf(imu.ace_y * imu.ace_y + imu.ace_z * imu.ace_z);
    if (denom < EPSILON) denom = EPSILON;

    acel_pitch = atan2f(imu.ace_x, denom) * RAD_TO_DEG;

    denom = sqrtf(imu.ace_x * imu.ace_x + imu.ace_z * imu.ace_z);
    if (denom < EPSILON) denom = EPSILON;

    acel_roll = atan2f(imu.ace_y, denom) * RAD_TO_DEG;
}

	void GY85_Giro_angulos()
{

	// Integración simple de giroscopio (Euler)
	giro_roll += imu.giro_x_f * muestreo;
	giro_pitch += imu.giro_y_f *muestreo;
	giro_yaw += imu.giro_z_f * muestreo;

}

void Bruj_heading()
	{
	float pitch_rad = compAngle_Pitch * DEG_TO_RAD;
	float roll_rad  = compAngle_Roll  * DEG_TO_RAD;

	// Compenso por inclinacion
	float mag_x_comp = imu.bruj_x * cosf(pitch_rad) + imu.bruj_z * sinf(pitch_rad);
	float mag_y_comp = imu.bruj_x * sinf(roll_rad) * sinf(pitch_rad) +
			           imu.bruj_y * cosf(roll_rad) -imu.bruj_z * sinf(roll_rad) * cosf(pitch_rad);

	// Calcular heading
//	 heading = atan2f(mag_y_comp, mag_x_comp) * RAD_TO_DEG;
	 heading = atan2f(imu.bruj_y, imu.bruj_x) * RAD_TO_DEG;

	//ubica de 0 a 360
	 if (heading < 0)
	     heading += 360.0f;

	}

void Filtro_Complementario()
{
	  //cosntantes del filtro  peso dado al acelelometro
	    const float alpha = 0.95;
	    compAngle_Roll = alpha * (compAngle_Roll + imu.giro_x_f*muestreo) +   (1-alpha) * acel_roll; // Calculate the angle using a Complimentary filter
	    compAngle_Pitch = alpha * (compAngle_Pitch + imu.giro_y_f*muestreo) + (1-alpha) * acel_pitch;
	}


void  filtro_kalman()
{
	kalman_roll=Kalman_Update(&kalman_roll_, acel_roll, imu.giro_x_f, muestreo);
	kalman_pitch=Kalman_Update(&kalman_pitch_, acel_pitch, imu.giro_y_f, muestreo);


}
