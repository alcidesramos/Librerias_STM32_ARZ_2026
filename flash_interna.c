/*
 * flash_interna.c
 *
 *  Created on: Apr 21, 2025
 *      Author: ALCIDES_RAMOS
 */


#include "stm32f4xx_hal.h"

#define FLASH_ADDR_SECTOR_5  0x08060000

HAL_StatusTypeDef Flash_Write(uint32_t data)
{
    HAL_StatusTypeDef status;
    //desbloque fhash
    HAL_FLASH_Unlock();
  //borra antes de escribit
    FLASH_Erase_Sector(FLASH_SECTOR_5, VOLTAGE_RANGE_3);

    status = HAL_FLASH_Program(TYPEPROGRAM_WORD, FLASH_ADDR_SECTOR_5, data);
    while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY)) ;// espra termine la programacion
    // Bloquea
    HAL_FLASH_Lock();

    return status;
}

uint32_t Flash_Read()
{
    return (*(volatile uint32_t*)FLASH_ADDR_SECTOR_5 & 0xFFFF);
}
