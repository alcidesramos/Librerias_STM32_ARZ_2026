/*
 * HX710.c
 *
 *  Created on: Apr 21, 2025
 *      Author: Alcides Ramos
 */

#include "HX710.h"
const float offset_manualhx=6941123;  //offset, valor leido a 0
const float calibrahx=8388607- offset_manualhx;  // valor a la calobtracion menos el offset

//IMPORTANTE ESTE VALOR presion conocida
const float prueba_hx=30.0;  //  presion de prueba para calibrahxción    gramos


float factor_hx=prueba_hx/calibrahx; //factor de multiplicativo a lectura base    debe colocarse aqui

int32_t offset_hx=offset_manualhx;// carga el offset manual al offset a usar
int32_t celda_hx=0;
float presion_hx,cal_hx;



void HX710_init()
{  delay_init();
HAL_Delay(100);
   HAL_GPIO_WritePin(HX710_CK_GPIO_Port, HX710_CK_Pin, 0); //saca cero por el reloj
   HAL_Delay(500);
}

uint8_t HX710_pulso()
{
uint8_t lei;
 HAL_GPIO_WritePin(HX710_CK_GPIO_Port, HX710_CK_Pin, 1); //saca uno por el reloj
 Delay_us(delay_pulso);
  lei=HAL_GPIO_ReadPin(HX710_DAT_GPIO_Port, HX710_DAT_Pin);
  HAL_GPIO_WritePin(HX710_CK_GPIO_Port, HX710_CK_Pin, 0); //saca cero por el reloj
   Delay_us(delay_pulso);
  return lei;
}

int32_t HX710_lectura(uint8_t frecuencia)
{

int16_t	  HX710_cont;
int32_t HX710_dato=0;

//si el sensor esta ocupado le da 10ms para que se desocupe y no hacer
//el codigo bloqueante
waitforlow(HX710_DAT_GPIO_Port, HX710_DAT_Pin,10);

  //da los primeros 24 pulsos
  for (HX710_cont=23;HX710_cont>=0;HX710_cont--)
  {
     if (HX710_pulso()==1)
     {
       HX710_dato=HX710_dato +( 1L<<HX710_cont);
     }
  }     //end for
      Delay_us(delay_pulso);

     if (frecuencia==10) HX710_pulso();

    /* else if (frecuencia==32)
     {
     HX710_pulso();
      Delay_us(delay_pulso);
     HX710_pulso();

      }*/
     else if (frecuencia==40)
     {
     HX710_pulso();
      Delay_us(delay_pulso);
     HX710_pulso();
      Delay_us(delay_pulso);
     HX710_pulso();
      }


  // Verifica si es negativo
    if ((HX710_dato & 0x800000)!=0)
    {
    HX710_dato=HX710_dato | 0xff000000;  //lo convierte a 32 bit con signo
    return  (-HX710_dato);// Se coloca - si no se quiere regersar valor negado
    }
    else     return  (HX710_dato);

}

int32_t HX710_promedio(uint8_t veces,uint8_t frecuencia)   //numero de datos, a que frecuencia
 {
        int32_t prom = 0;
        uint8_t cont;

        for (cont = 0; cont < veces; cont++)
        {
          prom =prom+  HX710_lectura(frecuencia);
           Delay_ms(delay_promedio);
        }
        return (prom / cont);
}

void HX710_tarar(uint8_t veces,uint8_t frecuencia)   //numero de datos, a que frecuencia
 {
         offset_hx=HX710_promedio(veces,frecuencia);

}


