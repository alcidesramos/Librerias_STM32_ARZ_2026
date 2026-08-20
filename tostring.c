/*
 * tostring.c
 *
 *  Created on: Dec 5, 2023
 *      Author: alcid
 */

#include "tostring.h"
/*
void printFloat(float v, int decimalDigits, char s[])
{
  uint8_t neg = 0;
  if (v < 0)
  {
    neg = 1;
    v = v - (2.0 * v);
  }
  int i = 1;
  int intPart, fractPart;
  for (; decimalDigits != 0; i *= 10, decimalDigits--)
    ;
  intPart = (int)v;
  fractPart = (int)((v - (float)(int)v) * i);
  if (fractPart < 0)
    fractPart *= -1;
  if (neg)
    sprintf(s, "-%i.%i", intPart, fractPart);
  else
    sprintf(s, "%i.%i", intPart, fractPart);
}

*/

void inttostr1(uint32_t numero,unsigned char *cadena)
{
	//*cadena++=(numero/100000)+48;
	*cadena++=((numero/10000)%10)+48;
     *cadena++=((numero/1000)%10)+48;
	*cadena++=((numero/100)%10)+48;
      *cadena++=((numero/10)%10)+48;
      *cadena++=((numero)%10)+48;
      *cadena++= 0;
}

//calcula y se desea devuelve el numero de digitos de un entero

int num_largo(int numero)
{
    int longitud = 0;
    while (numero != 0) {
        numero /= 10;
        longitud++;
    }
    return longitud;
}

// Función para convertir un número entero a una cadena
uint16_t inttostr(int numero, char *cadena)
{

	if (numero == 0) {
        cadena[0] = '0';
        cadena[1] = '\0';
        return(1);
    }
    // Manejar números negativos
    int negativo = 0;
    if (numero < 0) {
        negativo = 1;
        numero = -numero;
    }

    // Obtener la longitud del número
    int longitud = num_largo(numero);
    int longi=longitud;
    if (negativo==1) longi++;
    // Ajustar la longitud si el número es negativo
    if (negativo) {
        longitud++;
    }

    // Construir la cadena de derecha a izquierda
    cadena[longitud] = '\0';
    while (numero != 0) {
        longitud--;
        cadena[longitud] = '0' + numero % 10;
        numero /= 10;
    }

    // Agregar el signo negativo si es necesario
    if (negativo) {
        cadena[0] = '-';
    }
    return(longi++);
}

void floattostr(float numero, char *cadena, int decimales)
{
    int parte_entera;
	int parte_decimal;
   char cadena1[10];
   uint16_t longi;


    // Manejo de decimales
    int factor = 1;
    for (int i = 0; i < decimales; ++i) {
        factor *= 10;
    }

    // Convertir parte entera
     parte_entera = (int)numero;
     longi=inttostr(parte_entera, cadena);
     cadena[longi]='.';
    // strcat(cadena, ".");

     // Convertir parte decimal
    if (decimales > 0) {

    if (numero<0)
    {

    	numero=-numero;
         parte_entera=-parte_entera;
    }
    parte_decimal = (int)((numero - parte_entera) * factor);

    inttostr(parte_decimal, cadena1);
    strcat(cadena, cadena1);


    }
}
