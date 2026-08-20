#pragma once
#include "main.h"
#include "pinout.h"
#include <stdint.h>

/**
 * CD74HC4067 (16-ch) usando 3 líneas de selección (S3=0 por hardware).
 * Pines (según tu .ioc): S0=PB10 (MUX_S0), S1=PB2 (MUX_S1), S2=PB1 (MUX_S2).
 * EN va a GND (siempre habilitado).
 */

// Si en el futuro conectas S3, pon 0 y usa los 4 bits.
#define MUX_USE_3BIT   1

// Canales que usas en tu sistema
#define MUX_CH_MUTE    0u  // C00 → mudo a GND
#define MUX_CH_AUDIO   4u  // C04 → DFPlayer
#define MUX_CH_MIC     1u  // C01 → micrófono

// Init y selección de canal
void mux16_init(void);
void mux16_set_channel(uint8_t ch);
