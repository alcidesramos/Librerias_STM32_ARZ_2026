/*
 * MS4525DO.h
 *
 *  Created on: Jun 29, 2024
 *      Author: AveMaria
 */

#ifndef LIBRERIAS2_MS4525DO_H_
#define LIBRERIAS2_MS4525DO_H_

#include"main.h"
#define MS4525DO_DIR 0x28<<1

//define i2c a usar
#define MS4525DO_I2C hi2c1
extern  I2C_HandleTypeDef hi2c1;
void MS4525DO_config(const float p_max,const float p_min, const uint8_t type);
uint8_t Ms4525do_Begin();
uint8_t MS4525DO_read();
float ias_kts(float presion_MS452);

float calculate_true_airspeed(float altitude_m, float temperature_c);
float air_density(float altitude_m, float temperature_c);
float pressure_at_altitude(float altitude_m);

extern float presion_MS4525, temperatura_MS4525, ias;

// ---------- Estructura de resultado ----------
typedef struct {
    float IAS_mps;  // Indicada (para autopilot)
    float TAS_mps;  // Verdadera (para navegación)
} Airspeed_t;


// ---------- Prototipos ----------

float pressure_at_altitude(float altitude_m);
float air_density(float altitude_m, float temperature_c);
Airspeed_t calculate_airspeeds(float altitude_m, float temperature_c);




#endif /* LIBRERIAS2_MS4525DO_H_ */
