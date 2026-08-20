/*
 * BNO055.C
 *
 *  Created on: Oct 24, 2023
 *      Author: alcid
 */


#include "bno055.h"
#define M_PI 3.14159265358979323846

uint8_t BNO055_I2C_ADDRESS =BNO055_ADD<<1;

//varibles de acelelacion

//aceleracion cruda
int16_t ax_raw, ay_raw,az_raw;

//acelacion gravedades
float ax,ay,az;

//aceleraciones lineal
float alx,aly,alz;


//variables de los giros

//giro crudo
int16_t gx_raw,gy_raw,gz_raw;
//velocidad angular
float gx,gy,gz;


//magnetometros

//magnetometro crudo
int16_t max_raw,may_raw,maz_raw;

//gaus
float max,may,maz;

//angulos de euler
float pitch,roll,yaw;
//quaternion
float q1,q2,q3,q4;
float pitch1,roll1,yaw1;



// Variables globales para los offsets de los sensores
int16_t acc_offset_x;
int16_t acc_offset_y;
int16_t acc_offset_z;

int16_t gyr_offset_x;
int16_t gyr_offset_y;
int16_t gyr_offset_z;

int16_t mag_offset_x;
int16_t mag_offset_y;
int16_t mag_offset_z;

float pitch_offset=0,roll_offset=0;

//calibraciones
   uint8_t cali_sys,cali_giro,cali_ace,cali_mag ;
//promedio raw

   int16_t     promedio_raw_ax,promedio_raw_ay,promedio_raw_az,
   	   	   	   promedio_raw_gx,promedio_raw_gy,promedio_raw_gz,
			   promedio_raw_max,promedio_raw_may,promedio_raw_maz;

   typedef struct {
    int16_t accel_offset_x;
    int16_t accel_offset_y;
    int16_t accel_offset_z;
    int16_t gyro_offset_x;
    int16_t gyro_offset_y;
    int16_t gyro_offset_z;
    int16_t mag_offset_x;
    int16_t mag_offset_y;
    int16_t mag_offset_z;
    int16_t acc_radius;
    int16_t	mag_radius;
} BNO055_CalibrationData;

BNO055_CalibrationData calibrame;
uint8_t BNO055_INIT()
{
    uint8_t data;
    uint8_t ok = 0;

    if (HAL_I2C_IsDeviceReady(&BNO055_I2C, BNO055_I2C_ADDRESS , 2, 200) == HAL_OK)
        ok = 1;

    HAL_Delay(200);

    // CONFIG MODE
    data = 0x00;
    HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS,
                      BNO055_OPR_MODE, I2C_MEMADD_SIZE_8BIT,
                      &data, 1, 10);
    HAL_Delay(50);

    // PAGE 1
    data = 0x01;
    HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS,
                      BNO055_PAGE_ID, I2C_MEMADD_SIZE_8BIT,
                      &data, 1, 10);

    // ACC CONFIG
    uint8_t acc_config = (ACCEL_SCALE_4G << 5) | (0x04 << 2) | 0x00;
    HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS,
                      0x08, I2C_MEMADD_SIZE_8BIT,
                      &acc_config, 1, 10);

    // GYRO CONFIG
    uint8_t gyro_config0 = (GYRO_SCALE_2000DPS << 3) | 0x02;
    HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS,
                      0x0A, I2C_MEMADD_SIZE_8BIT,
                      &gyro_config0, 1, 10);

    uint8_t gyro_config1 = 0x00;
    HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS,
                      0x0B, I2C_MEMADD_SIZE_8BIT,
                      &gyro_config1, 1, 10);

    // VOLVER A PAGE 0
    data = 0x00;
    HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS,
                      BNO055_PAGE_ID, I2C_MEMADD_SIZE_8BIT,
                      &data, 1, 10);

    // AXIS MAP
    data = BNO055_AXIS_MAP_CONFIG_DEFAULT;
    HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS,
                      BNO055_AXIS_MAP_CONFIG, I2C_MEMADD_SIZE_8BIT,
                      &data, 1, 10);

    data = BNO055_AXIS_MAP_SIGN_DEFAULT;
    HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS,
                      BNO055_AXIS_MAP_SIGN, I2C_MEMADD_SIZE_8BIT,
                      &data, 1, 10);

    // POWER
    data = 0x00;
    HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS,
                      BNO055_PWR_MODE, I2C_MEMADD_SIZE_8BIT,
                      &data, 1, 10);

    HAL_Delay(30);
    // MODO FUSION
    data = 0x0C;//yaw brujula
 //   data = 0x08;//yaw del giro

    HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS,
                      BNO055_OPR_MODE, I2C_MEMADD_SIZE_8BIT,
                      &data, 1, 10);

    HAL_Delay(100);

    return ok;
}


void BNO055_Modo(uint8_t mode)
{
    uint8_t data;

    // 1. Ir a CONFIG MODE
    data = 0x00;
    HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS,
                      BNO055_OPR_MODE,
                      I2C_MEMADD_SIZE_8BIT,
                      &data, 1, 10);

    HAL_Delay(30);

    //  Cambio al nuevo modo
    data = mode;
    HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS,
                      BNO055_OPR_MODE,
                      I2C_MEMADD_SIZE_8BIT,
                      &data, 1, 10);

    HAL_Delay(50);
}

uint8_t BNO055_INIT1()
{
char reto=0;
if (HAL_I2C_IsDeviceReady(&BNO055_I2C, BNO055_I2C_ADDRESS , 2, 200) == HAL_OK)  reto=1;
HAL_Delay(200);
    // Configurar axis_map_config y axis_map_sign
    uint8_t axis_map_config = BNO055_AXIS_MAP_CONFIG_DEFAULT;
    uint8_t axis_map_sign = BNO055_AXIS_MAP_SIGN_DEFAULT;


    HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_AXIS_MAP_CONFIG, I2C_MEMADD_SIZE_8BIT, &axis_map_config, 1, 10);
    HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_AXIS_MAP_SIGN, I2C_MEMADD_SIZE_8BIT, &axis_map_sign, 1, 10);
    HAL_Delay(500);
    //escala del acelelometro
    uint8_t escala= ACCEL_SCALE_4G<< 3;
    HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_ACC_CONFIG, I2C_MEMADD_SIZE_8BIT, &escala, 1, HAL_MAX_DELAY);

     //escala del giro


    // Configurar el sensor para el modo fusion
    uint8_t power_mode = 0x00;  // Modo normal
    uint8_t operation_mode = 0x08;  // Modo fusion IMU

    HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_PWR_MODE, I2C_MEMADD_SIZE_8BIT, &power_mode, 1, 10);
    HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_OPR_MODE, I2C_MEMADD_SIZE_8BIT, &operation_mode, 1, 10);
    HAL_Delay(200);
    return reto;
}

// Función para leer aceleraciones en m/s^2
void BNO055_Lee_Aceleracion()
{
    uint8_t data[6];
    HAL_I2C_Mem_Read(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_ACC_DATA_X_LSB, I2C_MEMADD_SIZE_8BIT, data, 6, 10);

    int16_t raw_ax = (int16_t)(data[1] << 8 | data[0]);
    int16_t raw_ay = (int16_t)(data[3] << 8 | data[2]);
    int16_t raw_az = (int16_t)(data[5] << 8 | data[4]);

    // Convertir datos en bruto a m/s^2

    ax = (float)raw_ax / 100.0;
    ay = (float)raw_ay / 100.0;
    az = (float)raw_az / 100.0;
}

// Función para leer aceleraciones lineales
void BNO055_Lee_Aceleracion_lineal()
{
    uint8_t data[6];
    HAL_I2C_Mem_Read(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_LINEAR_ACC_DATA_X_LSB, I2C_MEMADD_SIZE_8BIT, data, 6, 10);

    int16_t raw_alx = (int16_t)(data[1] << 8 | data[0]);
    int16_t raw_aly = (int16_t)(data[3] << 8 | data[2]);
    int16_t raw_alz = (int16_t)(data[5] << 8 | data[4]);

    // Convertir datos en bruto a m/s^2
    alx = (float)raw_alx / 100.0;
    aly = (float)raw_aly / 100.0;
    alz = (float)raw_alz / 100.0;
}




// Función para leer velocidades angulares de los giroscopios en grados por segundo
void BNO055_Lee_Giro()
{
    uint8_t data[6];
    HAL_I2C_Mem_Read(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_GYRO_DATA_X_LSB, I2C_MEMADD_SIZE_8BIT, data, 6, 10);

    int16_t raw_gx = (int16_t)(data[1] << 8 | data[0]);
    int16_t raw_gy = (int16_t)(data[3] << 8 | data[2]);
    int16_t raw_gz = (int16_t)(data[5] << 8 | data[4]);

    gx = (float)raw_gx / 16.0;
    gy = (float)raw_gy / 16.0;
    gz = (float)raw_gz / 16.0;
}


// Función para leer magnetometros
void BNO055_Lee_Magnetometro()
{
    uint8_t data[6];
    HAL_I2C_Mem_Read(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_MAG_DATA_X_LSB, I2C_MEMADD_SIZE_8BIT, data, 6, 10);

    int16_t raw_max = (int16_t)(data[1] << 8 | data[0]);
    int16_t raw_may = (int16_t)(data[3] << 8 | data[2]);
    int16_t raw_maz = (int16_t)(data[5] << 8 | data[4]);

    max = (float)raw_max / 16.0;
    may = (float)raw_may / 16.0;
    maz = (float)raw_maz / 16.0;
}

void BNO055_LEE_DATOS_CRUDOS()
{
    uint8_t data[6];
    HAL_I2C_Mem_Read(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_ACC_DATA_X_LSB, I2C_MEMADD_SIZE_8BIT, data, 6, 10);
    ax_raw = (int16_t)(data[1] << 8 | data[0]);
    ay_raw= (int16_t)(data[3] << 8 | data[2]);
    az_raw  = (int16_t)(data[5] << 8 | data[4]);

    HAL_I2C_Mem_Read(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_GYRO_DATA_X_LSB, I2C_MEMADD_SIZE_8BIT, data, 6, 10);
    gx_raw  = (int16_t)(data[1] << 8 | data[0]);
    gy_raw  = (int16_t)(data[3] << 8 | data[2]);
    gz_raw  = (int16_t)(data[5] << 8 | data[4]);

    HAL_I2C_Mem_Read(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_MAG_DATA_X_LSB, I2C_MEMADD_SIZE_8BIT, data, 6, 10);
    max_raw  = (int16_t)(data[1] << 8 | data[0]);
    may_raw  = (int16_t)(data[3] << 8 | data[2]);
    maz_raw  = (int16_t)(data[5] << 8 | data[4]);


}

void BNO055_LEE_DATOS_CRUDOS_Prom(int cuantos)
{
    int32_t sum_raw_ax = 0;
    int32_t sum_raw_ay = 0;
    int32_t sum_raw_az = 0;

    int32_t sum_raw_gx = 0;
    int32_t sum_raw_gy = 0;
    int32_t sum_raw_gz = 0;

    int32_t sum_raw_max = 0;
    int32_t sum_raw_may = 0;
    int32_t sum_raw_maz = 0;

    uint8_t data[6];

    for (int i = 0; i < cuantos; i++)
    {
        HAL_I2C_Mem_Read(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_ACC_DATA_X_LSB, I2C_MEMADD_SIZE_8BIT, data, 6, 10);
        sum_raw_ax += (int16_t)(data[1] << 8 | data[0]);
        sum_raw_ay += (int16_t)(data[3] << 8 | data[2]);
        sum_raw_az += (int16_t)(data[5] << 8 | data[4]);

        HAL_I2C_Mem_Read(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_GYRO_DATA_X_LSB, I2C_MEMADD_SIZE_8BIT, data, 6, 10);
        sum_raw_gx += (int16_t)(data[1] << 8 | data[0]);
        sum_raw_gy += (int16_t)(data[3] << 8 | data[2]);
        sum_raw_gz += (int16_t)(data[5] << 8 | data[4]);

        HAL_I2C_Mem_Read(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_MAG_DATA_X_LSB, I2C_MEMADD_SIZE_8BIT, data, 6, 10);
        sum_raw_max += (int16_t)(data[1] << 8 | data[0]);
        sum_raw_may += (int16_t)(data[3] << 8 | data[2]);
        sum_raw_maz += (int16_t)(data[5] << 8 | data[4]);
    }

    // Calcula el promedio dividiendo la suma por 'cuantos'
    promedio_raw_ax = sum_raw_ax / cuantos;
    promedio_raw_ay = sum_raw_ay / cuantos;
    promedio_raw_az = sum_raw_az / cuantos;

     promedio_raw_gx = sum_raw_gx / cuantos;
    promedio_raw_gy = sum_raw_gy / cuantos;
    promedio_raw_gz = sum_raw_gz / cuantos;

     promedio_raw_max = sum_raw_max / cuantos;
     promedio_raw_may = sum_raw_may / cuantos;
     promedio_raw_maz = sum_raw_maz / cuantos;

    // Ahora tienes los valores promedio en las variables promedio_raw_ax, promedio_raw_ay, etc.
}

// Función para leer ángulos de Euler
void BNO055_Lee_Euler()
{
    uint8_t data[6];
    HAL_I2C_Mem_Read(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_EUL_HEADING_LSB, I2C_MEMADD_SIZE_8BIT, data, 6, 10);

    yaw = (float)((int16_t)(data[1] << 8 | data[0])) / 16.0;
    roll = (float)((int16_t)(data[3] << 8 | data[2])) / 16.0;
    pitch = (float)((int16_t)(data[5] << 8 | data[4])) / 16.0;
    roll-=roll_offset;
    pitch-=pitch_offset;
}




// Función para leer cuaterniones
void BNO055_Lee_Quaternion()
{
    uint8_t data[8];
    HAL_I2C_Mem_Read(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_QUATERNION_DATA_W_LSB, I2C_MEMADD_SIZE_8BIT, data, 8, 10);

    q1 = (float)((int16_t)(data[1] << 8 | data[0])) / 16384.0;
    q2 = (float)((int16_t)(data[3] << 8 | data[2])) / 16384.0;
    q3 = (float)((int16_t)(data[5] << 8 | data[4])) / 16384.0;
    q4 = (float)((int16_t)(data[7] << 8 | data[6])) / 16384.0;
}

// Función para convertir un cuaternión a ángulos de orientación (pitch, roll y yaw)
// Función para convertir cuaterniones a ángulos de Euler (pitch, roll y yaw)
void QuaternionToEuler()
{
   // Normaliza el cuaternión
    float norm = sqrt(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);
    q1 /= norm;
    q2 /= norm;
    q3 /= norm;
    q4 /= norm;

    // Cálculo de los ángulos de Euler
    pitch1 = atan2(2 * (q2 * q3 + q1 * q4), 1 - 2 * (q3 * q3 + q4 * q4));
    roll1 = asin(2 * (q1 * q3 - q2 * q4));
    yaw1 = atan2(2 * (q1 * q2 + q3 * q4), 1 - 2 * (q2 * q2 + q3 * q3));
}

void BNO055_Offset_Carga()
{
    uint8_t data[18]; // Cada sensor tiene 6 bytes de offset, y hay 3 sensores en total

    // Cambiar el sensor al modo de configuración
        uint8_t config_mode = 0x00; // 0x00 para el modo de configuración
        HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_OPR_MODE, I2C_MEMADD_SIZE_8BIT, &config_mode, 1, 10);


    HAL_I2C_Mem_Read(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_ACC_DATA_X_LSB, I2C_MEMADD_SIZE_8BIT, data, 18, 10);

    // Procesa los datos de offset para cada sensor
    acc_offset_x = (int16_t)(data[1] << 8 | data[0]);
    acc_offset_y = (int16_t)(data[3] << 8 | data[2]);
    acc_offset_z = (int16_t)(data[5] << 8 | data[4]);

    mag_offset_x = (int16_t)(data[7] << 8 | data[6]);
    mag_offset_y = (int16_t)(data[9] << 8 | data[8]);
    mag_offset_z = (int16_t)(data[11] << 8 | data[10]);

    gyr_offset_x = (int16_t)(data[13] << 8 | data[12]);
    gyr_offset_y = (int16_t)(data[15] << 8 | data[14]);
    gyr_offset_z = (int16_t)(data[17] << 8 | data[16]);
   //lo guarda en el registo offset
       HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_ACC_OFFSET_X_LSB, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&acc_offset_x, 2, 10);
       HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_ACC_OFFSET_Y_LSB, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&acc_offset_y, 2, 10);
       HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_ACC_OFFSET_Z_LSB, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&acc_offset_z, 2, 10);

       HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_GYR_OFFSET_X_LSB, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&gyr_offset_x, 2, 10);
       HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_GYR_OFFSET_Y_LSB, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&gyr_offset_y, 2, 10);
       HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_GYR_OFFSET_Z_LSB, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&gyr_offset_z, 2, 10);

       HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_MAG_OFFSET_X_LSB, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&mag_offset_x, 2, 10);
       HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_MAG_OFFSET_Y_LSB, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&mag_offset_y, 2, 10);
       HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_MAG_OFFSET_Z_LSB, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&mag_offset_z, 2, 10);


}

void BNO055_Offset_Carga_Promedio(uint16_t cuantos)
{
    int32_t sum_acc_offset_x = 0;
    int32_t sum_acc_offset_y = 0;
    int32_t sum_acc_offset_z = 0;
    int32_t sum_gyr_offset_x = 0;
    int32_t sum_gyr_offset_y = 0;
    int32_t sum_gyr_offset_z = 0;
    int32_t sum_mag_offset_x = 0;
    int32_t sum_mag_offset_y = 0;
    int32_t sum_mag_offset_z = 0;

    // Cambiar el sensor al modo de configuración
    uint8_t config_mode = 0x00; // 0x00 para el modo de configuración
    HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_OPR_MODE, I2C_MEMADD_SIZE_8BIT, &config_mode, 1, 10);


    for (uint16_t i = 0; i < cuantos; i++)
    {
        uint8_t data[18];
        HAL_I2C_Mem_Read(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_ACC_DATA_X_LSB, I2C_MEMADD_SIZE_8BIT, data, 18, 10);

        int16_t acc_offset_x = (int16_t)(data[1] << 8 | data[0]);
        int16_t acc_offset_y = (int16_t)(data[3] << 8 | data[2]);
        int16_t acc_offset_z = (int16_t)(data[5] << 8 | data[4]);
        int16_t mag_offset_x = (int16_t)(data[7] << 8 | data[6]);
        int16_t mag_offset_y = (int16_t)(data[9] << 8 | data[8]);
        int16_t mag_offset_z = (int16_t)(data[11] << 8 | data[10]);
        int16_t gyr_offset_x = (int16_t)(data[13] << 8 | data[12]);
        int16_t gyr_offset_y = (int16_t)(data[15] << 8 | data[14]);
        int16_t gyr_offset_z = (int16_t)(data[17] << 8 | data[16]);

        sum_acc_offset_x += acc_offset_x;
        sum_acc_offset_y += acc_offset_y;
        sum_acc_offset_z += acc_offset_z;
        sum_gyr_offset_x += gyr_offset_x;
        sum_gyr_offset_y += gyr_offset_y;
        sum_gyr_offset_z += gyr_offset_z;
        sum_mag_offset_x += mag_offset_x;
        sum_mag_offset_y += mag_offset_y;
        sum_mag_offset_z += mag_offset_z;
        HAL_Delay(1);
    }

    // Calcula el promedio dividiendo la suma por 'cuantos'
    int16_t avg_acc_offset_x = (int16_t)(sum_acc_offset_x / cuantos);
    int16_t avg_acc_offset_y = (int16_t)(sum_acc_offset_y / cuantos);
    int16_t avg_acc_offset_z = (int16_t)(sum_acc_offset_z / cuantos);
    int16_t avg_gyr_offset_x = (int16_t)(sum_gyr_offset_x / cuantos);
    int16_t avg_gyr_offset_y = (int16_t)(sum_gyr_offset_y / cuantos);
    int16_t avg_gyr_offset_z = (int16_t)(sum_gyr_offset_z / cuantos);
    int16_t avg_mag_offset_x = (int16_t)(sum_mag_offset_x / cuantos);
    int16_t avg_mag_offset_y = (int16_t)(sum_mag_offset_y / cuantos);
    int16_t avg_mag_offset_z = (int16_t)(sum_mag_offset_z / cuantos);

    // Guarda los valores promedio en los registros de offset del BNO055
    HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_ACC_OFFSET_X_LSB, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&avg_acc_offset_x, 2, 10);
    HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_ACC_OFFSET_Y_LSB, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&avg_acc_offset_y, 2, 10);
    HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_ACC_OFFSET_Z_LSB, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&avg_acc_offset_z, 2, 10);

    HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_GYR_OFFSET_X_LSB, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&avg_gyr_offset_x, 2, 10);
    HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_GYR_OFFSET_Y_LSB, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&avg_gyr_offset_y, 2, 10);
    HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_GYR_OFFSET_Z_LSB, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&avg_gyr_offset_z, 2, 10);

    HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_MAG_OFFSET_X_LSB, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&avg_mag_offset_x, 2, 10);
    HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_MAG_OFFSET_Y_LSB, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&avg_mag_offset_y, 2, 10);
    HAL_I2C_Mem_Write(&BNO055_I2C, BNO055_I2C_ADDRESS, BNO055_MAG_OFFSET_Z_LSB, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&avg_mag_offset_z, 2, 10);

}

void BNO055_Lee_calibracion()
{
	uint8_t calib_status = 0;
    HAL_I2C_Mem_Read(&BNO055_I2C, BNO055_I2C_ADDRESS,
                     0x35, 1, &calib_status, 1, 100);

    cali_sys = (calib_status >> 6) & 0x03;
    cali_giro = (calib_status >> 4) & 0x03;
    cali_ace = (calib_status >> 2) & 0x03;
     cali_mag = calib_status & 0x03;


}


uint8_t BNO055_Calibra(uint8_t modo,uint16_t tiempo)
{


    // poner modo deseado
    BNO055_Modo(modo);
    HAL_Delay(50);

    uint32_t timeout = HAL_GetTick() + tiempo*1000;

    while (HAL_GetTick() < timeout)
    {
    	 BNO055_Lee_calibracion();

        // según modo
        if (modo == 0x0C) // NDOF
        {
            if (cali_sys == 3 && cali_giro == 3 && cali_ace == 3 && cali_mag == 3)
                return 1;
        }
        else if (modo == 0x08) // IMU
        {
            if (cali_giro == 3 && cali_ace == 3)
                return 1;
        }

        HAL_Delay(200);
    }

    return 0;
}



