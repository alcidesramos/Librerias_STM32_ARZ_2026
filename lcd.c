/*
 * lcd.c
 *
 *  Created on: Feb 27, 2023
 *      Author: Alcides Ramos
 */
#include "lcd.h"

extern const char especial1[];
extern const char especial2[];
extern const char especial3[];
extern const char especial4[];
extern const char especial5[];
extern const char especial6[];
extern const char especial7[];
extern const char especial8[];






void Lcd_write(uint8_t lcd_dato)
{

	if(lcd_dato & 1)  // SI EL BIT 1 ES UNO LO PONE EN D4
		HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, 1);
	else
		HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, 0); // SI ES CERO PONE CERO EN D4


	if(lcd_dato & 2)
			HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, 1);

	else
			HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, 0);



	if(lcd_dato & 4)
			HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, 1);

	else
	         HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, 0);


	 if(lcd_dato & 8)
	 	HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, 1);

	 	else
	 	 HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, 0);

}


void Lcd_Cmd(uint8_t coman)
{
	 HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, 0);  // RS=0
	 Lcd_write(coman>>4);
	 HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, 1);  //EN=1
       HAL_Delay(4);
       HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, 0);  //EN=0

  	   Lcd_write(coman&0x0f);
       HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, 1);  //EN=1
        HAL_Delay(4);
         HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, 0);  //EN=0


}


void Lcd_Gotoxy(uint8_t fila, uint8_t columna)
{
	uint8_t temp;


    switch(fila)
    	{

        case 1:
        temp=0x80 + columna - 1;
        break;
        case 2:
        temp=0xC0 + columna - 1;
        break;
        case 3:
        temp=_LCD_THIRD_ROW + columna - 1;
        break;

        case 4:
        temp=_LCD_FOURTH_ROW  + columna - 1;
        break;
    	}
       Lcd_Cmd(temp );
}

void Lcd_Init()
{
	delay_us_dwt_init();
HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, 0);  // RS=0
HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, 0);  //EN=0

Lcd_write(0x03);
HAL_Delay(20);
//inicializacion a 4 bits
Lcd_Cmd(0x03);
HAL_Delay(5);

Lcd_Cmd(0x03);
  HAL_Delay(5);


  Lcd_Cmd(0x03);
	HAL_Delay(1);
	Lcd_Cmd(0x03);
// configuracion inicial

	Lcd_Cmd(0x02);
	Lcd_Cmd(0x02);

	Lcd_Cmd(0x08);
  HAL_Delay(1);
  Lcd_Cmd(0x00);
  Lcd_Cmd(0x01);
  HAL_Delay(3);

  Lcd_Cmd(0x28);
  Lcd_Cmd(0x06);
  HAL_Delay(1);

   //muestra el cuirsor  titilando por defecto
    Lcd_Cmd(0x0f);



}


void Lcd_Chr(uint8_t fila,uint8_t columna,char dat)
{
   uint8_t temp,y;


   switch(fila)
   	{

       case 1:
       temp=0x80 + columna - 1;
       break;
       case 2:
       temp=0xC0 + columna - 1;
       break;
       case 3:
       temp=_LCD_THIRD_ROW + columna - 1;
       break;
       case 4:
       temp=_LCD_FOURTH_ROW  + columna - 1;
       break;
   	}
      Lcd_Cmd(temp );


   temp = dat&0x0F;
   y = dat&0xF0;
   HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, 1);   //RS=1
  Lcd_write(y>>4);             //Data transfer
   HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, 1);  //EN=1
  delay_us(40);
   HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, 0);  //EN=0
   Lcd_write(temp);
   HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, 1);  //EN=1
   delay_us(40);
   HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, 0);  //EN=0
}

void Lcd_Chr_Cp(char dat)
{
   uint8_t temp,y;

   temp = dat&0x0F;
   y = dat&0xF0;
   HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, 1);   //RS=1
  Lcd_write(y>>4);             //Data transfer
   HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, 1);  //EN=1
  delay_us(40);
   HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, 0);  //EN=0
   Lcd_write(temp);
   HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, 1);  //EN=1
   delay_us(40);
   HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, 0);  //EN=0
}




void Lcd_Text(uint8_t fila,uint8_t columna,char *text1)
{
  while (*text1)
	 Lcd_Chr(fila,columna++,*text1++);

}

void Lcd_Text_Cp(char *text1)
{
  while (*text1)
	 Lcd_Chr_Cp(*text1++);

}


void  Lcd_chr_propio(uint8_t fila,uint8_t columna,const uint8_t cual,const char *vect)
{
	  char i;
	  char pos[]={64,72,80,88,96,104,112,120};
	    Lcd_Cmd(pos[cual-1]);
	    for (i = 0; i<=7; i++) Lcd_Chr_Cp(*vect++);
	    		    Lcd_Cmd(_LCD_RETURN_HOME);
	    Lcd_Chr(fila,columna, cual-1);
	}






