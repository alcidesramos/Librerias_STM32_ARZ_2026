/*
 * AS5600.c
 *
 *  Created on: 2/04/2026
 *      Author: jjrom
 */

#include "AS5600.h"

// Función para obtener el valor puro (0 a 4095)
uint16_t AS5600_GetRawAngle(I2C_HandleTypeDef *hi2c) {
    uint8_t buffer[2];
    HAL_StatusTypeDef status;

    // Leemos 2 bytes desde el registro del ángulo
    status = HAL_I2C_Mem_Read(hi2c, AS5600_I2C_ADDR, AS5600_RAW_ANGLE_REG, I2C_MEMADD_SIZE_8BIT, buffer, 2, 100);

    if (status == HAL_OK) {
        // Concatenamos los dos bytes y aplicamos una máscara de 12 bits
        return ((buffer[0] << 8) | buffer[1]) & 0x0FFF;
    }

    return 0; // Si hay error, devuelve 0
}

// Función para convertir la lectura cruda a grados (0.0 a 360.0)
float AS5600_GetDegrees(I2C_HandleTypeDef *hi2c) {
    uint16_t rawAngle = AS5600_GetRawAngle(hi2c);
    return (float)rawAngle * (360.0f / 4095.0f);
}
