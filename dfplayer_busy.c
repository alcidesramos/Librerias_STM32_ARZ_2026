/*
 * dfplayer_busy.c
 */

#include "dfplayer_busy.h"

/* UART que usa tu librería DFPlayer */
#if defined(DFPLAYER_HUART)
  #define DFP_HUART   DFPLAYER_HUART
#elif defined(DFP_UART_HANDLE)
  #define DFP_HUART   DFP_UART_HANDLE
#else
  extern UART_HandleTypeDef huart1;     /* Fallback común */
  #define DFP_HUART   huart1
#endif

  typedef enum {
      AUTH_IDLE = 0,     // nada armado, nada sonando
      AUTH_ARMED,        // intención válida, esperando caída de BUSY
      AUTH_PLAYING       // BUSY cayó dentro de ventana -> reproducción legítima en curso
  } auth_state_t;

  static volatile auth_state_t s_auth_state = AUTH_IDLE;
  static uint32_t s_authorized_until_ms = 0;   // ya la tenías (déjala aquí)


/* ======== Definiciones reales de flags (solo aquí) ======== */
volatile uint8_t df_busy_flag = 0;     // 1 = reproduciendo (BUSY=LOW)
volatile uint8_t df_kill_pending = 0;  // pedir kill en el loop principal

/* ---- Helper interno: leer BUSY ---- */
static inline GPIO_PinState busy_read(void)
{
    return HAL_GPIO_ReadPin(DFPLAYER_BUSY_GPIO_Port, DFPLAYER_BUSY_Pin);
}

/* ================= API ================= */

uint8_t DFP_IsPlaying(void)
{
    /* BUSY activo en LOW durante reproducción */
    return (busy_read() == GPIO_PIN_RESET) ? 1u : 0u;
}

uint8_t DFP_WaitUntilFree(uint32_t timeout_ms)
{
    /* Requiere ~100 ms seguidos con BUSY=HIGH (debounce simple) */
    uint8_t stable = 0;
    uint32_t t0 = HAL_GetTick();

    for (;;)
    {
        if (busy_read() == GPIO_PIN_SET) {
            if (++stable >= 10) return 1;   /* ~10x10ms = 100 ms */
        } else {
            stable = 0;
        }

        HAL_Delay(10);

        if (timeout_ms) {
            uint32_t elapsed = HAL_GetTick() - t0;
            if (elapsed >= timeout_ms) return 0;
        }
    }
}

void DFP_FlushRx(uint16_t max_drain_ms)
{
    /* Descarta bytes sueltos tras el boot del módulo */
    uint8_t byte;
    uint32_t t0 = HAL_GetTick();

    while ((HAL_GetTick() - t0) < max_drain_ms)
    {
        if (HAL_UART_Receive(&DFP_HUART, &byte, 1, 5) != HAL_OK) {
            HAL_Delay(2);
        }
    }
}

void DFP_SafeInit(void)
{
    HAL_Delay(300);
    mp3_modo_reset();       /* tu reset */
    HAL_Delay(500);
    mp3_modo_pausa();       /* “stop” seguro */
    DFP_FlushRx(200);
    mp3_volumen_x(20);
    (void)DFP_WaitUntilFree(1000);
}

/* ------- Anti-autoplay ------- */

void DFP_ArmAuthorization(uint32_t grace_ms)
{
    s_authorized_until_ms = HAL_GetTick() + grace_ms;
    s_auth_state = AUTH_ARMED;
}

uint8_t DFP_HasAuthorization(void)
{
    // útil para otros módulos; true si estamos armados o reproduciendo legítimamente
    if (s_auth_state == AUTH_PLAYING) return 1;
    return (HAL_GetTick() <= s_authorized_until_ms);
}

uint8_t DFP_KillIfPhantom(uint32_t max_wait_ms)
{
	uint32_t now = HAL_GetTick();
	if (!DFP_IsPlaying()) return 1;

	/* No mates si estamos en reproducción legítima */
	if (s_auth_state == AUTH_PLAYING) return 1;
	if ((int32_t)(s_authorized_until_ms - now) > 0) return 1;

	mp3_modo_pausa();
	return DFP_WaitUntilFree(max_wait_ms);
}

/* ====== Llamada desde la ISR EXTI (PA6) ====== */
void DFP_EXTI_Update(void)
{
    static uint32_t last_edge_ms = 0;
    uint32_t now = HAL_GetTick();

    /* Debounce más robusto: ignora cambios < 20 ms */
    if ((now - last_edge_ms) < 20) return;
    last_edge_ms = now;

    GPIO_PinState s = HAL_GPIO_ReadPin(DFPLAYER_BUSY_GPIO_Port, DFPLAYER_BUSY_Pin);

    if (s == GPIO_PIN_RESET) {
        /* BUSY bajó = inicio de reproducción */
        df_busy_flag = 1;

        if (s_auth_state == AUTH_ARMED || (int32_t)(s_authorized_until_ms - now) > 0) {
            /* Caída dentro de la ventana => reproducción legítima */
            s_auth_state = AUTH_PLAYING;
            df_kill_pending = 0;     // por si quedó algo colgado
        } else {
            /* Caída sin autorización => fantasma */
            if (s_auth_state != AUTH_PLAYING) {
                df_kill_pending = 1;
            }
        }
    } else {
        /* BUSY subió = fin de reproducción */
        df_busy_flag = 0;
        s_auth_state = AUTH_IDLE;
        s_authorized_until_ms = 0;
        df_kill_pending = 0;         // limpia cualquier residual
    }
}


