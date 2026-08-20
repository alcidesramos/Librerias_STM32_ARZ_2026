/*
 * lcd.h
 *
 *  Created on: Feb 27, 2023
 *      Author: Alcides Ramos
 */
#include "main.h"
#include "delays.h"
/*
#define LCD_RS_Pin GPIO_PIN_12
#define LCD_RS_GPIO_Port GPIOB
#define LCD_EN_Pin GPIO_PIN_13
#define LCD_EN_GPIO_Port GPIOB
#define LCD_D4_Pin GPIO_PIN_14
#define LCD_D4_GPIO_Port GPIOB
#define LCD_D5_Pin GPIO_PIN_15
#define LCD_D5_GPIO_Port GPIOB
#define LCD_D6_Pin GPIO_PIN_8
#define LCD_D6_GPIO_Port GPIOA
#define LCD_D7_Pin GPIO_PIN_9
#define LCD_D7_GPIO_Port GPIOA
*/

#ifndef LIBRERIA_LCD_H_
#define LIBRERIA_LCD_H_
#define _LCD_FIRST_ROW          0x80     //Move cursor to the 1st row
#define _LCD_SECOND_ROW         0xC0     //Move cursor to the 2nd row
//#define _LCD_THIRD_ROW          0x90     //Move cursor to the 3rd row
//#define _LCD_FOURTH_ROW         0xD0     //Move cursor to the 4th row
#define _LCD_THIRD_ROW          0x94     //Move cursor to the 3rd row
#define _LCD_FOURTH_ROW         0xD4     //Move cursor to the 4th row

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

void Lcd_Cmd(uint8_t coman);
void Lcd_Gotoxy(uint8_t fila, uint8_t columna);
void Lcd_Init();
void Lcd_Chr(uint8_t fila,uint8_t columna,char dat);
void Lcd_Chr_Cp(char dat);
void Lcd_Text(uint8_t fila,uint8_t columna,char *text1);
void Lcd_Text_Cp(char *text1);
void  Lcd_chr_propio(uint8_t fila,uint8_t columna,uint8_t cual,const char *vect);








#endif /* LIBRERIA_LCD_H_ */
