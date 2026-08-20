#include "audio_router.h"
#include "mux16.h"
#include "app_config.h"   // STATION_ID, ST_E1..E3, MUX_CH_* defines
#include "main.h"

static volatile uint8_t s_mic_enable = 0;           // 0=normal, 1=mic activo
static volatile uint8_t s_mic_active_station = 0;   // 0=ninguna, 1..3

static inline void _mute(void)        { mux16_set_channel(MUX_CH_MUTE);     } // C00
static inline void _route_audio(void) { mux16_set_channel(MUX_CH_AUDIO_DF); } // C04

/* En ESTA placa: ¿qué canal corresponde al mic del emisor 'src'? */
static inline uint8_t _mic_channel_for(uint8_t src)
{
	if (src == STATION_ID) return MUX_CH_MIC_LOCAL;   // C01 = mic local

	#if (STATION_ID == ST_E1)
	    // 👉 En la placa E1: elige qué remoto va a C02 y cuál a C03
	    if (src == ST_E2) return MUX_CH_MIC_REMOTE_B;     // C03 para E2
	    else               return MUX_CH_MIC_REMOTE_A;    // C02 para E3 TIERRA

	#elif (STATION_ID == ST_E2)
	    // 👉 En la placa E2:
	    if (src == ST_E1) return MUX_CH_MIC_REMOTE_B;     // C03 para E1
	    else               return MUX_CH_MIC_REMOTE_A;    // C02 para E3 TIERRA

	#elif (STATION_ID == ST_E3)
	    // 👉 En la placa E3:
	    if (src == ST_E1) return MUX_CH_MIC_REMOTE_B;     // C03 para E1
	    else               return MUX_CH_MIC_REMOTE_A;    // C02 para E2
	#else
	    return MUX_CH_MIC_LOCAL; // fallback
	#endif
}

void audio_router_init(void)
{
    s_mic_enable = 0;
    s_mic_active_station = 0;
    mux16_init();           // deja C00
}

void audio_prepare_play(void)
{
    if (s_mic_enable) return;     // prioridad: si hay mic, no tocar MUX
    _mute();        HAL_Delay(1);
    _route_audio(); HAL_Delay(1); // C04 → DFPlayer
}

void audio_on_stop(void)
{
    _mute();  // C00
}

void audio_mic_on_station(uint8_t station_id)
{
    s_mic_enable = 1;
    s_mic_active_station = station_id;
    _mute(); HAL_Delay(3);
    mux16_set_channel(_mic_channel_for(station_id));  // C01/C02/C03
}

void audio_mic_off_station(uint8_t station_id)
{
    /* Solo suelta si el que apaga es quien lo tenía tomado */
    if (s_mic_enable && s_mic_active_station == station_id) {
        s_mic_enable = 0;
        s_mic_active_station = 0;
        _mute();   // C00
    }
}

void audio_mic_on(void)  { audio_mic_on_station(STATION_ID); }
void audio_mic_off(void) { audio_mic_off_station(STATION_ID); }

uint8_t audio_is_mic_on(void)
{
    return s_mic_enable;
}
