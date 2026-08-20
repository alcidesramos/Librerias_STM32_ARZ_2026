#pragma once
#include <stdint.h>

/* Init del ruteo (deja C00) */
void audio_router_init(void);

/* DFPlayer: llamado por dfplayer.c antes de reproducir y al pausar */
void audio_prepare_play(void);
void audio_on_stop(void);

/* Mic por estación (1..3). Selecciona C01/C02/C03 según emisor. */
void audio_mic_on_station(uint8_t station_id);
void audio_mic_off_station(uint8_t station_id);

/* Conveniencias “locales” (usa STATION_ID) */
void audio_mic_on(void);
void audio_mic_off(void);

/* Estado */
uint8_t audio_is_mic_on(void);
