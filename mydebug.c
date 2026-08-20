/*
 * mydebug.c
 *
 *  Created on: Jul 14, 2023
 *      Author: Administrator
 */
#include <stdio.h>
#include <stdarg.h>
#include "mydebug.h"

void printDebugSimple(char* inputString)
{
	char buf[MAX_INPUT_LENGTH + 3]; // add space for "\r\n" and null terminator
	strcpy(buf, inputString);
	strcat(buf, "\r\n"); //printDebug会自动添加换行符，因此不再需要\n
	HAL_UART_Transmit(&imu_uart, (uint8_t *)buf, strlen(buf), HAL_MAX_DELAY);
}



void printDebug(const char* format, ...) {
    char buffer[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    printDebugSimple(buffer);
}

/*
 * example usage:
 * uint8_t message[] = {0x30, 0x00, 0x01, 0x02, 0x03};
 * printDebugHexString("Sending message: %s", message, sizeof(message));
 *
 */
void printDebugHexString(const char* format, uint8_t *message, uint8_t numBytes) {
    char hexMessage[(numBytes+1)*2 + 3]; // Each byte will be 2 hex digits, plus space for "\n\0"
    for(int i = 0; i < numBytes; i++) {
        snprintf(hexMessage + i*2, 3, "%02X", message[i]); // Format each byte as two hexadecimal digits
    }
    printDebug(format, hexMessage);
}


void printDebugCharArray(const char* format, char *message, uint8_t numChars) {
    char charMessage[numChars + 1]; // each char will be 1 char, plus space for null terminator
    for(int i = 0; i < numChars; i++) {
        charMessage[i] = message[i]; // copy each char
    }
    charMessage[numChars] = '\0'; // add null terminator
    printDebug(format, charMessage);
}

