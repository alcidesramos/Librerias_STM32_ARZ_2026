/*
 * lcd.c
 *
 *  Created on: Feb 26, 2023
 *      Author: Alcides Ramos
 */
#include "LCD_I2C.H"

//funcion que escribe en el PCF8574
void   I2C_PCF8574_Write(unsigned char value)
{
HAL_I2C_Master_Transmit(&i2c_pin, LCD_ADDR  ,&value, 1, 300);
}

 void I2C_Lcd_Cmd(char out_char)
 {
   unsigned char lcddata;
   //Coloca  4 bit alto
   lcddata = (out_char & 0xF0)|LCD_BL;
   I2C_PCF8574_Write(lcddata | LCD_EN);
   HAL_Delay(1);
   // RE
   I2C_PCF8574_Write(lcddata & ~LCD_EN);
   HAL_Delay(1);
     // Coloca los 4 bits bajo
     lcddata = ((out_char << 4) & 0xF0)|LCD_BL;
     I2C_PCF8574_Write(lcddata | LCD_EN);
   HAL_Delay(1);
     // ESCRIBE PULSO DE RE
     I2C_PCF8574_Write(lcddata & ~LCD_EN);
   HAL_Delay(1);

 }

 void I2C_Lcd_Init()
 {


     unsigned char lcddata;

   HAL_Delay(20); //retardo de inicializacion
   // INICIA PROCESO DE INICIALIZACION
 lcddata=0x30;
 I2C_PCF8574_Write(lcddata | LCD_EN); //envia comando de inicializacion
  HAL_Delay(1);
 I2C_PCF8574_Write(lcddata & ~LCD_EN);
  HAL_Delay(5);

 lcddata=0x30;
 I2C_PCF8574_Write(lcddata | LCD_EN); //envia comando de inicializacion
  HAL_Delay(1);
 I2C_PCF8574_Write(lcddata & ~LCD_EN);
  HAL_Delay(5);

 lcddata=0x30;
 I2C_PCF8574_Write(lcddata | LCD_EN); //envia comando de inicializacion
  HAL_Delay(1);
 I2C_PCF8574_Write(lcddata & ~LCD_EN);
  HAL_Delay(5);


 //modo a 4  bits
 lcddata=0x20;
 I2C_PCF8574_Write(lcddata | LCD_EN);
  HAL_Delay(1);
 I2C_PCF8574_Write(lcddata & ~LCD_EN);
  HAL_Delay(1);

 //modo a 4 lineas
 lcddata=0x20;
 I2C_PCF8574_Write(lcddata | LCD_EN);
  HAL_Delay(1);
 I2C_PCF8574_Write(lcddata & ~LCD_EN);
  HAL_Delay(1);
 lcddata=0x80;
 I2C_PCF8574_Write(lcddata | LCD_EN);
  HAL_Delay(1);
 I2C_PCF8574_Write(lcddata & ~LCD_EN);
  HAL_Delay(5);

 //Apaga el LCD
 lcddata=0x00;
 I2C_PCF8574_Write(lcddata | LCD_EN);
  HAL_Delay(1);
 I2C_PCF8574_Write(lcddata & ~LCD_EN);
  HAL_Delay(1);
 lcddata=0x80;
 I2C_PCF8574_Write(lcddata | LCD_EN);
  HAL_Delay(1);
 I2C_PCF8574_Write(lcddata & ~LCD_EN);
  HAL_Delay(1);


  //Prende el LCD
 lcddata=0x00;
 I2C_PCF8574_Write(lcddata | LCD_EN);
  HAL_Delay(1);
 I2C_PCF8574_Write(lcddata & ~LCD_EN);
  HAL_Delay(1);
 lcddata=0xC0;
 I2C_PCF8574_Write(lcddata | LCD_EN);
  HAL_Delay(1);
 I2C_PCF8574_Write(lcddata & ~LCD_EN);
  HAL_Delay(1);

  //Ajusta desplazamiento del cursor
 lcddata=0x00;
 I2C_PCF8574_Write(lcddata | LCD_EN);
  HAL_Delay(1);
 I2C_PCF8574_Write(lcddata & ~LCD_EN);
  HAL_Delay(1);
 lcddata=0x20 | LCD_BL;
 I2C_PCF8574_Write(lcddata | LCD_EN);
  HAL_Delay(1);
 I2C_PCF8574_Write(lcddata & ~LCD_EN);
  HAL_Delay(1);

  //Borra la pantalla
 lcddata=0x00;
 I2C_PCF8574_Write(lcddata | LCD_EN);
  HAL_Delay(1);
 I2C_PCF8574_Write(lcddata & ~LCD_EN);
  HAL_Delay(1);
 lcddata=0x10;
 I2C_PCF8574_Write(lcddata | LCD_EN);
  HAL_Delay(1);
 I2C_PCF8574_Write(lcddata & ~LCD_EN);
  HAL_Delay(1);
 }


 void I2C_Lcd_Chr(char row, char column, char out_char)
 {
     unsigned char lcddata;

     switch(row){

         case 1:
         I2C_Lcd_Cmd(0x80 + (column - 1));
         break;
         case 2:
         I2C_Lcd_Cmd(0xC0 + (column - 1));
         break;
         case 3:
        I2C_Lcd_Cmd(_LCD_THIRD_ROW  + (column - 1));
     	
        	 break;
         case 4:
      I2C_Lcd_Cmd(_LCD_FOURTH_ROW  + (column - 1));
         break;
     }


   lcddata = (out_char & 0xF0)| LCD_RS |LCD_BL;
   I2C_PCF8574_Write(lcddata | LCD_EN);
   HAL_Delay(1);
   I2C_PCF8574_Write(lcddata & ~LCD_EN);
   HAL_Delay(1);

   lcddata = ((out_char << 4) & 0xF0) |LCD_RS |LCD_BL;
   I2C_PCF8574_Write(lcddata | LCD_EN);
   HAL_Delay(1);
   I2C_PCF8574_Write(lcddata & ~LCD_EN);
   I2C_PCF8574_Write(lcddata & ~LCD_RS);
   HAL_Delay(1);


 }

 void I2C_Lcd_Chr_Cp(char out_char)
  {
    unsigned char lcddata;
   lcddata = (out_char & 0xF0)|LCD_RS |LCD_BL;
   I2C_PCF8574_Write(lcddata | LCD_EN);
   HAL_Delay(1);
   I2C_PCF8574_Write(lcddata & ~LCD_EN);
   HAL_Delay(1);

   lcddata = ((out_char << 4) & 0xF0)|LCD_RS |LCD_BL;
   I2C_PCF8574_Write(lcddata | LCD_EN);
   HAL_Delay(1);
   I2C_PCF8574_Write(lcddata & ~LCD_EN);
   I2C_PCF8574_Write(lcddata & ~LCD_RS);
   HAL_Delay(1);



 }


 void I2C_Lcd_Text(char row, char col, char *text) {
     while(*text)
          I2C_Lcd_Chr(row, col++, *text++);
 }

 void I2C_Lcd_Text_Cp(char *text) {
     while(*text)
          I2C_Lcd_Chr_Cp(*text++);
 }



 void  I2C_Lcd_chr_propio(uint8_t fila,uint8_t columna,const uint8_t cual,const char *vect)
 {
 	  char i;
 	  char pos[]={64,72,80,88,96,104,112,120};
 	  I2C_Lcd_Cmd(pos[cual-1]);
 	    for (i = 0; i<=7; i++) I2C_Lcd_Chr_Cp(*vect++);

 	   I2C_Lcd_Cmd(_LCD_RETURN_HOME);
 	  I2C_Lcd_Chr(fila,columna, cual-1);
 	}

