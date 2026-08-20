/*
 * lidarvl53.h
 *
 *  Created on: Jul 20, 2023
 *      Author: alcid
 */

#define lidar_max_mm 2000

#ifndef LIBERRIAS_VL53L0X_LIDARVL53_H_
#define LIBERRIAS_VL53L0X_LIDARVL53_H_

#define dir_s1 0x52  //direccion del 1° sensor
#define dir_s2 0x53  //direccion del 2° sensor
#define dir_s3 0x54  //direccion del 3° sensor
#define dir_s4 0x55  //direccion del 4° sensor
#define dir_s5 0x56  //direccion del 5° sensor
#define dir_s6 0x57  //direccion del 6° sensor
#define dir_s7 0x58  //direccion del 7° sensor
#define dir_s8 0x59  //direccion del 8° sensor



#include "vl53l0x_api.h"
#include "main.h"


void lidar_init(uint8_t dir);
uint16_t lidar_lee_mm(uint8_t dir);
float lidar_lee_cm(uint8_t dir);
uint8_t lidar_set_dir(uint8_t dir);

#endif /* LIBERRIAS_VL53L0X_LIDARVL53_H_ */
