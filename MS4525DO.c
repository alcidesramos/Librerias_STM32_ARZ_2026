#include "MS4525DO.h"
#include "stdbool.h"
#include <math.h>
#include <stdio.h>


#define P_max 14746
#define P_min 1638

// ---------- Constantes atmosféricas ----------

#define P0        101325.0f     // Presión a nivel del mar (Pa)
#define T0        288.15f       // Temp. estándar (K)
#define L         0.0065f       // Gradiente térmico (K/m)
#define R         287.05f       // Constante específica del aire (J/kg·K)
#define G         9.80665f      // Gravedad (m/s²)
#define M         0.0289644f    // Masa molar del aire (kg/mol)
#define R_UNIV    8.3144598f    // Constante universal de los gases (J/mol·K)
#define RHO_STD   1.225f        // Densidad estándar (kg/m³)


enum OutputType {
    OUTPUT_TYPE_A,
    OUTPUT_TYPE_B
  };

uint8_t
    STATUS_GOOD = 0x00,
    STATUS_STALE_DATA = 0x02,
    STATUS_FAULT = 0x03;
int MAX_TRIES_ = 10;

float p_max_, p_min_;
float c_, d_;
int16_t P_CNT_ = 16383;
int16_t T_CNT_ = 2047;
float T_MAX_ = 150;
float T_MIN_ = -50;

uint8_t buf_[4];
uint8_t bytes_rx_;
uint8_t status_;
uint16_t pres_cnts_, temp_cnts_;
float pres_psi_, temp_;
float pres_pa_, temp_c_;


void MS4525DO_config(const float p_max,const float p_min, const uint8_t type)
 {

         p_min_ = p_min;
         p_max_ = p_max;
         if (type == OUTPUT_TYPE_A) {
           c_ = 0.1f;
           d_ = 0.8f;
         } else {
           c_ = 0.05f;
           d_ = 0.9f;
         }
       }


uint8_t Ms4525do_Begin()
 {
   /* Checking to see if we can communicate with sensor */
	 if(HAL_I2C_IsDeviceReady(&MS4525DO_I2C, MS4525DO_DIR ,10, 1000)==HAL_OK)
		 return(1);
	 else
		 return(0);

 }

uint8_t MS4525DO_read()
 {
	 uint8_t buffr[4];
	 float P_CNT_ = 16383;
	 float T_CNT_ = 2047;
	 float T_MAX_ = 150;
	 float T_MIN_ = -50;
	 float offset = -0.10;

#define p_max_ 1
#define p_min_ -1

	 HAL_I2C_Master_Receive(&MS4525DO_I2C,MS4525DO_DIR ,buffr, 4, 200);
	 uint16_t raw_pressure = ( (uint16_t)(buffr[0] & 0x3F) << 8) | buffr[1];

	 //pres_cnts_ = static_cast<uint16_t>(buf_[0] & 0x3F) << 8 | buf_[1];

	 //temp_cnts_ = static_cast<uint16_t>(buf_[2]) << 3 | buf_[3] & 0xE0 >> 5;

	 uint16_t raw_temperature = ((uint16_t)(buffr[2]) << 3) | (buffr[3] & 0xE0 >> 5);


	/* uint8_t status = (raw_pressure >> 14) & 0x03;
	         if (status != 0)
	         {
	             return (0);
	         }*/

	 // hacer mascara para eliminar los 14 y 15 que son estatus

	 //raw_pressure &= 0x3FFF;
	 //presion_MS4525 = ((float)((raw_pressure - 1638.3 / 0.99)+1638));

	 presion_MS4525 = ((float)(raw_pressure) - 0.1f * P_CNT_) *((p_max_ - p_min_) / (0.8f * P_CNT_)) + p_min_;


	 presion_MS4525 = (presion_MS4525 * 6.89476f)-offset; // en Pascales

	 //presion_MS4525= -(((raw_pressure - 0.1*16383) * (P_max-P_min)/(0.8*16383)) + P_min);

	 //temperatura_MS4525 = ((float)((raw_temperature / 2047.0) * 200.0) - 50.0);

	 temperatura_MS4525 = (float)(raw_temperature) * (T_MAX_ - T_MIN_) / T_CNT_ + T_MIN_;

	 //temperatura_MS4525=((200.0 * raw_temperature) / 2047) - 50;
	 //temperatura_MS4525=temperatura_MS4525/100;

     return (1);
 }


/**
  * Calculate indicated airspeed (IAS).
  *
  * Note that the indicated airspeed is not the true airspeed because it
  * lacks the air density and instrument error compensation.
  *
  * @param differential_pressure total_ pressure - static pressure
  * @return IAS in m/s
  */


float ias_kts(float presion_MS452)
 {

     float CONSTANTS_AIR_DENSITY_SEA_LEVEL_15C = 1.225f;                // kg/m^3 , Pa

     if (presion_MS452*6894.76 > 0.0f)
     {
         return sqrtf((2.0f * presion_MS452*6894.76) / CONSTANTS_AIR_DENSITY_SEA_LEVEL_15C); // presion en Pa

     } else {
         return -sqrtf((2.0f * fabsf(presion_MS452*6894.76)) / CONSTANTS_AIR_DENSITY_SEA_LEVEL_15C);
     }

 }


// ---------- Implementación DE LAS VELOCIDADES DE VUELO ----------

float pressure_at_altitude(float altitude_m) {
    float ratio = 1.0f - (L * altitude_m) / T0;
    float exponent = (G * M) / (R_UNIV * L);
    return P0 * powf(ratio, exponent);
}

float air_density(float altitude_m, float temperature_c) {
    float T_kelvin = temperature_c + 273.15f;
    float pressure = pressure_at_altitude(altitude_m);
    return pressure / (R * T_kelvin);
}

Airspeed_t calculate_airspeeds(float altitude_m, float temperature_c)
{
    Airspeed_t result = {0};
    float delta_p = presion_MS4525;  // presion diferencial del sensor en Pascales

    if (delta_p <= 0.0f)
    {
        return result;  // Retorna 0 si no hay presión válida
    }

    // IAS con densidad estándar SEGUN TABLA ISA

    result.IAS_mps = sqrtf((2.0f * delta_p) / RHO_STD);

    // TAS con densidad real

    float rho_real = air_density(altitude_m, temperature_c);
    result.TAS_mps = sqrtf((2.0f * delta_p) / rho_real);

    return result;
}



