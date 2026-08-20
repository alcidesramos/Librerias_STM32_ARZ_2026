/*
 * HX711.c
 *
 *  Created on: Apr 9, 2023
 *      Author: Alcides Ramos
 */

#include "HX711.h"
extern signed long offset;  //guarda el offset

void HX711_init()
{  delay_us_dwt_init();
   HAL_GPIO_WritePin(HX711_CK_GPIO_Port, HX711_CK_Pin, 0); //saca cero por el reloj
   HAL_Delay(500);
}

uint8_t HX_711_pulso()
{
uint8_t lei;
 HAL_GPIO_WritePin(HX711_CK_GPIO_Port, HX711_CK_Pin, 1); //saca uno por el reloj
 Delay_us(delay_pulso);
  lei=HAL_GPIO_ReadPin(HX711_DAT_GPIO_Port, HX711_DAT_Pin);
  HAL_GPIO_WritePin(HX711_CK_GPIO_Port, HX711_CK_Pin, 0); //saca cero por el reloj
   Delay_us(delay_pulso);
  return lei;
}

int32_t HX711_lectura(uint8_t ganancia)
{

int16_t	  hx711_cont;
int32_t hx711_dato=0;

//si el sensor esta ocupado le da 10ms para que se desocupe y no hacer
//el codigo bloqueante
waitforlow(HX711_DAT_GPIO_Port, HX711_DAT_Pin,10);

  //da los primeros 24 pulsos
  for (hx711_cont=23;hx711_cont>=0;hx711_cont--)
  {
     if (HX_711_pulso()==1)
     {
       hx711_dato=hx711_dato +( 1L<<hx711_cont);
     }
  }     //end for
      Delay_us(delay_pulso);

     if (ganancia==128)  HX_711_pulso();

     else if (ganancia==32)
     {
      HX_711_pulso();
      Delay_us(delay_pulso);
      HX_711_pulso();

      }
     else if (ganancia==64)
     {
      HX_711_pulso();
      Delay_us(delay_pulso);
      HX_711_pulso();
      Delay_us(delay_pulso);
      HX_711_pulso();
      }


  // Verifica si es negativo
    if ((hx711_dato & 0x800000)!=0)
    {
    hx711_dato=hx711_dato | 0xff000000;  //lo convierte a 32 bit con signo
    return  (-hx711_dato);// Se coloca - si no se quiere regersar valor negado
    }
    else     return  (hx711_dato);

}

int32_t HX711_promedio(uint8_t veces,uint8_t ganancia)   //numero de datos, a que ganancia
 {
        int32_t prom = 0;
        uint8_t cont;

        for (cont = 0; cont < veces; cont++)
        {
          prom =prom+  HX711_lectura(ganancia);
           Delay_ms(delay_promedio);
        }
        return (prom / cont);
}

void HX_tarar(uint8_t veces,uint8_t ganancia)   //numero de datos, a que ganancia
 {
         offset=HX711_promedio(veces,ganancia);

}


