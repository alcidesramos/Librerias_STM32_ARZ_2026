#include "mux16.h"

void mux16_init(void)
{
    // Arranca mudo por seguridad
    mux16_set_channel(MUX_CH_MUTE);
}

void mux16_set_channel(uint8_t ch)
{
#if MUX_USE_3BIT
    // S3 = 0 fijo por hardware; usamos S2..S0
    PIN_PUT(MUX_S0, (ch >> 0) & 1u);   // PB10
    PIN_PUT(MUX_S1, (ch >> 1) & 1u);   // PB2
    PIN_PUT(MUX_S2, (ch >> 2) & 1u);   // PB1
#else
    // Versión 4 bits (si conectas S3 en el futuro)
    PIN_PUT(MUX_S0, (ch >> 0) & 1u);
    PIN_PUT(MUX_S1, (ch >> 1) & 1u);
    PIN_PUT(MUX_S2, (ch >> 2) & 1u);
    PIN_PUT(MUX_S3, (ch >> 3) & 1u);
#endif
}
