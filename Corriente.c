/**
  ******************************************************************************
  * @file           : Corriente.c
  * @brief          : Detector de picos puro para SCT-013 con diodo Schottky.
  * Compensación no lineal ajustada (Corrección de +0.1A de desfase).
  ******************************************************************************
  */

#include "Corriente.h"
#include <stdlib.h>
#include <math.h>

static ADC_HandleTypeDef *_hadc;
static uint16_t *_adc_buffer;

// Variable estática para almacenar el histórico de la corriente
static float corriente_filtrada_anterior = 0.0f;

/**
 * @brief Inicializa la librería y vincula el ADC con el DMA
 */
void CORRIENTE_Init(ADC_HandleTypeDef *hadc, uint16_t *buffer)
{
    _hadc = hadc;
    _adc_buffer = buffer;

    // Arranca el periférico ADC en modo DMA
    HAL_ADC_Start_DMA(_hadc, (uint32_t*)_adc_buffer, 1);
    HAL_Delay(50);

    corriente_filtrada_anterior = 0.0f;
}

/**
 * @brief Mide la corriente aplicando una curva de corrección no lineal precisa
 */
Energia_t CORRIENTE_Medir(uint32_t tiempo_ms)
{
    Energia_t datos = {0.0f, 0.0f};

    uint16_t max_pico = 0;
    uint32_t contador_picos_reales = 0;
    float corriente_instantanea = 0.0f;

    // Muestreo rápido de 35ms (2 ciclos de la red a 60Hz) para máxima fluidez
    uint32_t tiempo_muestreo_rapido = 35;
    uint32_t t_inicio = HAL_GetTick();

    // 1. ESCANEO DE PICOS VELOZ
    while((HAL_GetTick() - t_inicio) < tiempo_muestreo_rapido)
    {
        uint16_t lectura = *_adc_buffer;

        if(lectura > max_pico)
        {
            max_pico = lectura;
        }

        // El ruido base de tu circuito llega hasta 3 cuentas. Cualquiera mayor es señal real.
        if(lectura >= 4)
        {
            contador_picos_reales++;
        }

        HAL_Delay(1);
    }

    // 2. COMPENSACIÓN NO LINEAL CORREGIDA:
    // Si el pico es mayor a 3 cuentas y es repetitivo, calculamos el valor real.
    if ((max_pico >= 4) && (contador_picos_reales > 3))
    {
        // Convertimos el pico crudo a voltaje medido
        float voltaje_pico = ((float)max_pico * ADC_REF) / ADC_RES;

        // Voltaje base linealizado
        float voltaje_rms = voltaje_pico / 2.0f;
        float corriente_base = voltaje_rms * 95.1f;

        // ECUACIÓN DE CORRECCIÓN AJUSTADA:
        // Bajamos ligeramente la pendiente (de 1.15 a 1.08) y el offset (de 0.22 a 0.14)
        // para absorber el exceso de 0.1A que mostraba la lectura en la terminal.
        corriente_instantanea = (1.08f * corriente_base) + 0.14f;
    }
    else
    {
        corriente_instantanea = 0.0f;
    }


    // 3. FILTRO DE PROMEDIO MÓVIL EXPONENCIAL (Suavizado)
    const float ALFA = 0.25f;
    datos.Irms = (ALFA * corriente_instantanea) + ((1.0f - ALFA) * corriente_filtrada_anterior);
    corriente_filtrada_anterior = datos.Irms;

    // 4. FILTRO DE SUELO: Si el abanico se apaga por completo, forzamos a cero absoluto.
    // Reducido a 0.20A para dar margen a la nueva curva de calibración más baja.
    if(datos.Irms < 0.20f)
    {
        datos.Irms = 0.0f;
        corriente_filtrada_anterior = 0.0f;
    }

    // Calcular potencia estimada
    datos.potencia = datos.Irms * VOLTAJE_RED;

    return datos;
}
