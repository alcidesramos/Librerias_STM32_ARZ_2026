/*
 * LCD_I2C.H
 *
 *  Created on: Feb 26, 2023
 *      Author: Alcides Ramos
 */

#ifndef LIBRERIA_LCD_I2C_H_
#define LIBRERIA_LCD_I2C_H_
#include "main.h"

extern I2C_HandleTypeDef i2c_pin;  //i2c a utilzar

#define _LCD_FIRST_ROW          0x80     //Move cursor to the 1st row
#define _LCD_SECOND_ROW         0xC0     //Move cursor to the 2nd row
#define _LCD_FIRST_ROW          0x80     //Move cursor to the 1st row
#define _LCD_SECOND_ROW         0xC0     //Move cursor to the 2nd row
#define _LCD_THIRD_ROW          0x90     //Move cursor to the 3rd row
#define _LCD_FOURTH_ROW         0xD0     //Move cursor to the 4th row
//#define _LCD_THIRD_ROW          0x94     //Move cursor to the 3rd row
//#define _LCD_FOURTH_ROW         0xD4     //Move cursor to the 4th row

#define _LCD_CLEAR              0x01     //Clear display
#define _LCD_RETURN_HOME        0x02     //Return cursor to home position, returns a shifted display to
                                         //its original position. Display data RAM is unaffected.
#define _LCD_CURSOR_OFF         0x0C     //Turn off cursor
#define _LCD_UNDERLINE_ON       0x0E     //Underline cursor on
#define _LCD_BLINK_CURSOR_ON    0x0F     //Blink cursor on
#define _LCD_MOVE_CURSOR_LEFT   0x10     //Move cursor left without changing display data RAM
#define _LCD_MOVE_CURSOR_RIGHT  0x14     //Move cursor right without changing display data RAM
#define _LCD_TURN_ON            0x0C     //Turn Lcd display on
#define _LCD_TURN_OFF           0x08     //Turn Lcd display off
#define _LCD_SHIFT_LEFT         0x18     //Shift display left without changing display data RAM
#define _LCD_SHIFT_RIGHT        0x1E     //Shift display right without changing display data RAM

// PINES USADOS  EN EL PCF8574
// P7, P6, P5, P4 = Data, P3=Backlight (BL), P2=E, P1=RW, P0=RS

#define LCD_ADDR 0x4E        //direccion del i2c 0b0100     A2A1A0     1
//#define LCD_ADDR 0x40      //direccion del i2c     0b0100     A2A1A0     0


#define LCD_BL 0x08
#define LCD_EN 0x04
#define LCD_RS 0x01


void I2C_Lcd_Cmd(char out_char);
void I2C_Lcd_Init();
void I2C_Lcd_Chr(char row, char column, char out_char);
void I2C_Lcd_Chr_Cp(char out_char);
void I2C_Lcd_Text(char row, char col, char *text);
void I2C_Lcd_Text_Cp(char *text);
void  I2C_Lcd_chr_propio(uint8_t fila,uint8_t columna,const uint8_t cual,const char *vect);









#endif /* LIBRERIA_LCD_I2C_H_ */
