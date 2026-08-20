/*
 * swapbytes.c
 *
 *  Created on: Aug 1, 2025
 *      Author: ALCIDES_RAMOS
 */


#include "swapbytes.h"
uint16_t swap_bytes16(uint16_t s_dato)
{

return ((s_dato<<8)&0xff00)|((s_dato>>8)&0x00ff);
}

uint32_t Invert_bytes32(uint32_t dato)
{
    return ((dato & 0x000000FF) << 24) |
           ((dato & 0x0000FF00) << 8) |
           ((dato & 0x00FF0000) >> 8) |
           ((dato & 0xFF000000) >> 24);
}



