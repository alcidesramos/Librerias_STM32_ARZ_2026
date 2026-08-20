
/*
 * max30102.h.c
 *
 *  Created on: 28/04/2026
 *      Author: Alcides Ramos
 */


#ifndef MAX30102_STM32_H
#define MAX30102_STM32_H
#include "main.h"
//puerto i2c del sensor
#define max30102_i2c hi2c1

/*
 en el main
   MAX30102_DefaultConfig(&config);
        config.sample_rate_hz = 200U;
        config.led_red_current = 0x24;
         config.led_ir_current = 0x24;
            config.min_bpm = 50U;
         config.max_bpm = 140U;
        config.finger_threshold_ir = 10000U;

        if (MAX30102_Init() == HAL_OK) uartx_write_text(&huart1, "SENSOR OKS\r\n");
        else uartx_write_text(&huart1, "SENSOR BAD\r\n");

 //////// en el while



	    if (MAX30102_Update( ) == HAL_OK)
	        {
	            if (data.valid != 0U)
	            {
	                sprintf(texto,"Pulso: %.1f BPM | SpO2: %.1f %% | Temp: %.2f C\r\n",
	                       data.bpm,
	                       data.spo2,
	                       data.temperature_c);
	            uartx_write_text(&huart1, texto);
	            }
	            else if (data.finger_present == 0U)
	            {
	            uartx_write_text(&huart1, "Coloca el dedo en el sensor\r\n");
	            }
	        }

	        HAL_Delay(200);

 */
#ifndef MAX30102_BUFFER_SIZE
#define MAX30102_BUFFER_SIZE 200U
#endif

#define MAX30102_I2C_ADDR       (0x57U << 1)
#define MAX30102_PART_ID_VALUE  0x15U

typedef struct
{
    uint16_t sample_rate_hz;
    uint8_t led_red_current;
    uint8_t led_ir_current;
    uint32_t finger_threshold_ir;
    uint16_t min_bpm;
    uint16_t max_bpm;
    uint16_t temperature_period_ms;
} MAX30102_Config_t;

typedef struct
{
    float bpm;
    float spo2;
    float temperature_c;
    uint8_t finger_present;
    uint8_t valid;
} MAX30102_Data_t;

typedef struct
{
    I2C_HandleTypeDef *hi2c;
    uint16_t i2c_addr;
    uint16_t sample_rate_hz;
    uint32_t finger_threshold_ir;
    uint16_t min_bpm;
    uint16_t max_bpm;
    uint16_t temperature_period_ms;
    uint32_t red_buffer[MAX30102_BUFFER_SIZE];
    uint32_t ir_buffer[MAX30102_BUFFER_SIZE];
    uint16_t head;
    uint16_t count;
    uint32_t last_temperature_tick;
    float last_temperature_c;
} MAX30102_t;

void MAX30102_DefaultConfig(MAX30102_Config_t *config);
HAL_StatusTypeDef MAX30102_Init();
HAL_StatusTypeDef MAX30102_Update( );
HAL_StatusTypeDef MAX30102_ReadTemperature( float *temperature_c);
HAL_StatusTypeDef MAX30102_ClearFIFO();
       extern MAX30102_t sensor;
       extern MAX30102_Config_t config;
       extern  MAX30102_Data_t data;



       extern I2C_HandleTypeDef max30102_i2c;




#endif
