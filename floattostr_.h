/*
 * floartostr_.h
 *
 *  Created on: Jan 31, 2025
 *      Author: ALCIDES_RAMOS
 */

#ifndef SRC_LIBRERIAS_FLOATTOSTR__H_
#define SRC_LIBRERIAS_FLOATTOSTR__H_

#include "main.h"
#include "stdio.h"
#include "string.h"

void floattostr(char *str, float float_number, int n_decimales);

void floattostr(char *str, float float_number, int n_decimales)
{
    // Verifica que el número de decimales sea válido
    if (n_decimales < 0) {
        n_decimales = 0;
    }

    // Convierte la parte entera
    int integerPart = (int)float_number;
    float fractionalPart =float_number - integerPart;

    // Convierte la parte entera a cadena
    int len = sprintf(str, "%d", integerPart);

    // Si se requieren decimales, añade el punto y los decimales
    if (n_decimales > 0) {
        str[len] = '.';  // Añade el punto decimal
        len++;

        // Convierte la parte fraccionaria a cadena
        for (int i = 0; i < n_decimales; i++) {
            fractionalPart *= 10;
            int digit = (int)fractionalPart;
            str[len + i] = '0' + digit;
            fractionalPart -= digit;
        }
        str[len + n_decimales] = '\0';  // Termina la cadena
    } else {
        str[len] = '\0';  // Termina la cadena si no hay decimales
    }
}

#endif /* SRC_LIBRERIAS_FLOATTOSTR__H_ */
