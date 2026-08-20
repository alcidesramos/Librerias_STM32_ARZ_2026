/*
 * Teclado.c
 *
 *  Created on: 26/06/2023
 *      Author: alcid
 */


#include "Teclado.h"
#include "stdbool.h"



bool res_=res;

uint16_t retardo=100;// retardo arranque
char retardo_cad[5];
enum teclado_estado {n1,n2,n3,n4,en};
char estado_teclado=n1;



uint8_t t_tecla1()
{
   HAL_GPIO_WritePin(TF1_GPIO_Port, TF1_Pin, !res_);//activa solo la fila 1
   HAL_GPIO_WritePin(TF2_GPIO_Port, TF2_Pin, res_);
   HAL_GPIO_WritePin(TF3_GPIO_Port, TF3_Pin, res_);
   HAL_GPIO_WritePin(TF4_GPIO_Port, TF4_Pin, res_);

     if (te_c1==!res_   ) return(1);
     else if (te_c2==!res_) return(2);
     else if (te_c3==!res_) return(3);
    else if (te_c4==!res_) return(4);   //comentar si es 3*4
    else return(0);
}

uint8_t t_tecla2()
{
   HAL_GPIO_WritePin(TF1_GPIO_Port, TF1_Pin, res_);
   HAL_GPIO_WritePin(TF2_GPIO_Port, TF2_Pin, !res_);//activa solo la fila 2
   HAL_GPIO_WritePin(TF3_GPIO_Port, TF3_Pin, res_);
   HAL_GPIO_WritePin(TF4_GPIO_Port, TF4_Pin, res_);

     if (te_c1==!res_   ) return(5);
     else if (te_c2==!res_) return(6);
     else if (te_c3==!res_) return(7);
    else if (te_c4==!res_) return(8);   //comentar si es 3*4
    else return(0);
}


uint8_t t_tecla3()
{
   HAL_GPIO_WritePin(TF1_GPIO_Port, TF1_Pin, res_);
   HAL_GPIO_WritePin(TF2_GPIO_Port, TF2_Pin, res_);
   HAL_GPIO_WritePin(TF3_GPIO_Port, TF3_Pin, !res_);//activa solo la fila 3
   HAL_GPIO_WritePin(TF4_GPIO_Port, TF4_Pin, res_);

     if (te_c1==!res_   ) return(9);
     else if (te_c2==!res_) return(10);
     else if (te_c3==!res_) return(11);
    else if (te_c4==!res_) return(12);   //comentar si es 3*4
    else return(0);
}


uint8_t t_tecla4()
{
   HAL_GPIO_WritePin(TF1_GPIO_Port, TF1_Pin, res_);
   HAL_GPIO_WritePin(TF2_GPIO_Port, TF2_Pin, res_);
   HAL_GPIO_WritePin(TF3_GPIO_Port, TF3_Pin, res_);
   HAL_GPIO_WritePin(TF4_GPIO_Port, TF4_Pin, !res_);//activa solo la fila 3

     if (te_c1==!res_   ) return(13);
     else if (te_c2==!res_) return(14);
     else if (te_c3==!res_) return(15);
    else if (te_c4==!res_) return(16);   //comentar si es 3*4
    else return(0);
}


//Funcion para ver si hay una tecla presionada
uint8_t tecla_presionada()
{
  uint8_t te;

  te=t_tecla1();
  if (te!=0) return (te);
  HAL_Delay(5);
  te=t_tecla2();
  if (te!=0) return (te);
  HAL_Delay(5);
  te=t_tecla3();
  if (te!=0) return (te);
  HAL_Delay(5);
  te=t_tecla4();
  if (te!=0) return (te);
}



//Función para decodificar el teclado.
uint8_t tecla_deco()
{
//Estructura switch case para evaluar los valores_ retornados
//por la librería del teclado.
  switch( tecla_presionada () )
  {
  case 1: return '7'; break;
  case 2: return '8'; break;
  case 3: return '9'; break;
  case 4: return '%'; break;
  case 5: return '4';  break;
  case 6: return '5'; break;
  case 7: return '6'; break;
  case 8: return '*'; break;
  case 9: return '1'; break;
  case 10: return '2'; break;
  case 11: return '3'; break;
  case 12: return '-'; break;
  case 13: return 'C';  break;
  case 14: return '0';  break;
  case 15: return '=';  break;
  case 16: return '+';  break;
  default: return 0; break; //Tecla no pulsada.
  }
}
void teclado_estado()
{
    uint8_t tecla=tecla_deco();

    switch (estado_teclado)
    {
      case n1:

              if ( (tecla!=0)  && (isdigit(tecla)))
              {
            HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 1);
            retardo_cad[0]=tecla;
              estado_teclado=n2;
              Lcd_Chr(2,8,tecla);
              Delay_ms(400); // retardo antirrebote
              }
            break;

      case n2:
                   if ((tecla!=0)  && (isdigit(tecla)))
                   {
                   retardo_cad[1]=tecla;
                   estado_teclado=n3;
                   Lcd_Chr(2,9,tecla);
                   Delay_ms(400); // retardo antirrebote
                   }
                 break;


           case n3:

                   if ((tecla!=0)  && (isdigit(tecla)))
                   {
                   retardo_cad[2]=tecla;
                   estado_teclado=n4;
                   Lcd_Chr(2,10,tecla);
                   Delay_ms(400); // retardo antirrebote
                   }
                 break;


           case n4:
                   if ((tecla!=0)  && (isdigit(tecla)))
                   {
                   retardo_cad[3]=tecla;
                   estado_teclado=en;
                   Lcd_Chr(2,11,tecla);
                   Delay_ms(400); // retardo antirrebote

                   }
                 break;

             case en:

                    if ((tecla!=0)  && (tecla!='='))  // no se presiono el igual
                   {
                  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 0);
                  estado_teclado=n1; // vuelve al estado de  captura del primer numero
                  Lcd_Text(2,8,"      ");// borra la pantalla
                  Delay_ms(600); // retardo antirrebote


                   }

                    // si si se presiono el enter
                                else if ((tecla!=0)  && (tecla=='='))  //si se presiono el igual
                                {
                               HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 0);
                               retardo_cad[4]=0;  //final de cadena
                                retardo=atoi(retardo_cad); // lo pasa a numero y calcula el retardo deseado
                                estado_teclado=n1; // vuelve al estado de caprura del primer numero.
                                Lcd_Text(2,8,"      ");// borra la pantalla
                               Delay_ms(600); // retardo antirrebote
                                }

                                break;




    }

 }
void teclado_init()
{
delay_Init();
}

void teclado_retardo(int16_t valor)
{
	uint32_t tiem1,tiem2;
    tiem1=HAL_GetTick();
  while(HAL_GetTick()-tiem1<valor)  teclado_estado();


}

