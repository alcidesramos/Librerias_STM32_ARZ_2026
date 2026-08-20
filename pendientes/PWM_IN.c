#include "pwm_in.h"

// INICIALIZAR

void PWMIn_Init(PWMIn_t *obj)
{
    for (int i=0; i<PWMIN_CHANNELS; i++)
    {
        obj->waiting_falling[i] = 1;
        obj->last_rise[i] = 0;
        obj->duty[i] = 0;
        obj->last_update[i] = 0;

        if (obj->channel[i] != 0)
        {
            HAL_TIM_IC_Start_IT(obj->htim, obj->channel[i]);
            // iniciar capturando flanco RISING
            __HAL_TIM_SET_CAPTUREPOLARITY(obj->htim, obj->channel[i],
                                          TIM_INPUTCHANNELPOLARITY_RISING);
        }
    }
}


// MANEJAR INTERRUPCIÓN
// (llamar desde TImX_CC_IRQHandler)

void PWMIn_IRQ_Handler(PWMIn_t *obj)
{
    for (int i=0; i<PWMIN_CHANNELS; i++)
    {
        uint32_t ch = obj->channel[i];
        if (ch == 0) continue;

        uint32_t flag = 0;

        if      (ch == TIM_CHANNEL_1) flag = TIM_FLAG_CC1;
        else if (ch == TIM_CHANNEL_2) flag = TIM_FLAG_CC2;
        else if (ch == TIM_CHANNEL_3) flag = TIM_FLAG_CC3;
        else if (ch == TIM_CHANNEL_4) flag = TIM_FLAG_CC4;

        if (__HAL_TIM_GET_FLAG(obj->htim, flag))
        {
            uint32_t ccr;
            // leer CCR correcto
            if      (ch == TIM_CHANNEL_1) ccr = obj->htim->Instance->CCR1;
            else if (ch == TIM_CHANNEL_2) ccr = obj->htim->Instance->CCR2;
            else if (ch == TIM_CHANNEL_3) ccr = obj->htim->Instance->CCR3;
            else                          ccr = obj->htim->Instance->CCR4;

            if (obj->waiting_falling[i])
            {
                // --- FLANCO RISING ---
                obj->last_rise[i] = ccr;
                obj->waiting_falling[i] = 0;

                __HAL_TIM_SET_CAPTUREPOLARITY(obj->htim, ch,
                        TIM_INPUTCHANNELPOLARITY_FALLING);
            }
            else
            {
                // --- FLANCO FALLING ---
                uint32_t rise = obj->last_rise[i];
                uint32_t arr = obj->htim->Instance->ARR;

                uint32_t width;
                if (ccr >= rise)
                    width = ccr - rise;
                else
                    width = (arr - rise) + ccr;

                obj->duty[i] = width;
                obj->last_update[i] = HAL_GetTick();

                obj->waiting_falling[i] = 1;
                __HAL_TIM_SET_CAPTUREPOLARITY(obj->htim, ch,
                        TIM_INPUTCHANNELPOLARITY_RISING);
            }

            __HAL_TIM_CLEAR_FLAG(obj->htim, flag);
        }
    }
}


// DETECTAR PÉRDIDA DE SEÑAL
// Llamar cada 10–20ms desde el main

void PWMIn_Update_Timeout(PWMIn_t *obj, uint32_t now_ms)
{
    for (int i=0; i<PWMIN_CHANNELS; i++)
    {
        if (obj->channel[i] == 0) continue;

        if (now_ms - obj->last_update[i] > obj->timeout_ms)
        {
            obj->duty[i] = 0;  // sin señal
        }
    }
}
