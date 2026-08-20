#ifndef DFPLAYER_BUSY_H
#define DFPLAYER_BUSY_H

#include "main.h"
#include "stm32f4xx_hal.h"

/* Ajusta si usas otro pin */
#ifndef DFPLAYER_BUSY_GPIO_Port
  #define DFPLAYER_BUSY_GPIO_Port   GPIOA
  #define DFPLAYER_BUSY_Pin         GPIO_PIN_6   /* PA6 */
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Flags visibles fuera (solo declaradas aquí) */
extern volatile uint8_t df_kill_pending;
extern volatile uint8_t df_busy_flag;

/* API principal */
uint8_t DFP_IsPlaying(void);
uint8_t DFP_WaitUntilFree(uint32_t timeout_ms);
void    DFP_FlushRx(uint16_t max_drain_ms);
void    DFP_SafeInit(void);

/* Anti-autoplay */
void    DFP_ArmAuthorization(uint32_t grace_ms);
uint8_t DFP_KillIfPhantom(uint32_t max_wait_ms);
uint8_t DFP_HasAuthorization(void);

/* Llamada desde EXTI (ISR) */
void    DFP_EXTI_Update(void);

#ifdef __cplusplus
}
#endif

#endif /* DFPLAYER_BUSY_H */
