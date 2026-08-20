#ifndef PWM_IN_H
#define PWM_IN_H

#include "main.h"

#define PWMIN_CHANNELS   4

typedef struct {
    TIM_HandleTypeDef *htim;
    uint32_t channel[PWMIN_CHANNELS];
    uint32_t last_rise[PWMIN_CHANNELS];
    uint32_t duty[PWMIN_CHANNELS];
    uint8_t waiting_falling[PWMIN_CHANNELS];  // 1 = esperaba falling
    uint32_t timeout_ms;                      // tiempo sin pulsos
    uint32_t last_update[PWMIN_CHANNELS];     // para detectar pérdida de señal
} PWMIn_t;


void PWMIn_Init(PWMIn_t *obj);
void PWMIn_IRQ_Handler(PWMIn_t *obj);
void PWMIn_Update_Timeout(PWMIn_t *obj, uint32_t now_ms);

#endif
