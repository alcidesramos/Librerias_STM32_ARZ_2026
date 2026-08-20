#ifndef __CORRIENTE_H
#define __CORRIENTE_H

#include "stm32f4xx_hal.h"
#include <math.h>

// Configuración ADC de la STM32
#define ADC_REF        3.3f
#define ADC_RES        4095.0f

// Voltaje nominal de la red eléctrica
#define VOLTAJE_RED    114.0f

// Factor de escala del sensor (Ajustar experimentalmente si es necesario)
#define FACTOR_SENSOR  320.0f

// Punto de reposo del circuito (Valor del ADC con abanico apagado y resistencia de 10k a GND)
// Ajusta este número si tu lectura base en reposo es ligeramente distinta (ej. 1200 o 1300)
#define ADC_OFFSET     1150

typedef struct
{
    float Irms;
    float potencia;
} Energia_t;

// Prototipos de funciones
void CORRIENTE_Init(ADC_HandleTypeDef *hadc, uint16_t *buffer);
Energia_t CORRIENTE_Medir(uint32_t tiempo_ms);

#endif /* __CORRIENTE_H */
