#include "main.h"
#include "string.h"
#include "stdio.h"
#include "delays.h"

#define SSD1306_I2C_Address                          0X78// 0X7A //0x78    //Arduino 3C=78*2
//#define SSD1306_I2C_Address                          0X7A// 0X7A //0x78    //Arduino 3C=78*2

#define SSD1306_I2C  							   hi2c1//I2C USADO


extern unsigned char buffer[];


#ifdef   reset
extern sbit RST;
#endif


#define Set_Lower_Column_Start_Address_CMD        0x00 
#define Set_Higher_Column_Start_Address_CMD       0x10
#define Set_Memory_Addressing_Mode_CMD            0x20
#define Set_Column_Address_CMD                    0x21
#define Set_Page_Address_CMD                      0x22

#define ACTIVATE_SCROLL                           0x2F
#define DEACTIVATE_SCROLL                         0x2E
#define SET_VERTICAL_SCROLL_AREA                  0xA3
#define RIGHT_HORIZONTAL_SCROLL                   0x26
#define LEFT_HORIZONTAL_SCROLL                    0x27
#define VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL      0x29
#define VERTICAL_AND_LEFT_HORIZONTAL_SCROLL      0x2A


#define Set_Display_Start_Line_CMD                0x40
#define Set_Contrast_Control_CMD                  0x81
#define Set_Charge_Pump_CMD                       0x8D
#define Set_Segment_Remap_CMD                     0xA0
#define Set_Entire_Display_ON_CMD                 0xA4
#define Set_Normal_or_Inverse_Display_CMD         0xA6
#define Set_Multiplex_Ratio_CMD                   0xA8
#define Set_Display_ON_or_OFF_CMD                 0xAE
#define Set_Page_Start_Address_CMD                0xB0
#define Set_COM_Output_Scan_Direction_CMD         0xC0
#define Set_Display_Offset_CMD                    0xD3
#define Set_Display_Clock_CMD                     0xD5
#define Set_Pre_charge_Period_CMD                 0xD9
#define Set_Common_HW_Config_CMD                  0xDA
#define Set_VCOMH_Level_CMD                       0xDB
#define Set_NOP_CMD                               0xE3

#define Horizontal_Addressing_Mode                0x00
#define Vertical_Addressing_Mode                  0x01
#define Page_Addressing_Mode                      0x02

#define Disable_Charge_Pump                       0x00
#define Enable_Charge_Pump                        0x04
                                                                             
#define Column_Address_0_Mapped_to_SEG0           0x00
#define Column_Address_0_Mapped_to_SEG127         0x01

#define Normal_Display                            0x00
#define Entire_Display_ON                         0x01
                                                    
#define Non_Inverted_Display                      0x00
#define Inverted_Display                          0x01
                                               
#define Display_OFF                               0x00
#define Display_ON                                0x01

#define Scan_from_COM0_to_63                      0x00
#define Scan_from_COM63_to_0                      0x08

 //tamaño de la pantalla
#define x_size                                    128
#define x_max                                     x_size
#define x_min                                     0
#define y_size                                   64
#define y_max                                     8
#define y_min                                     0

#define ON                                        1
#define OFF                                       0
#define on                                        1
#define off                                       0

#define NOR                                       1
#define INV                                       0
#define nor                                       1
#define inv                                       0


#define FILL                                       1
#define NOFILL                                      0


#define YES                                       1
#define NO                                        0
#define SI                                       1



#define yes                                       1
#define no                                       0
#define si                                       1


#define ROUND                                     1
#define SQUARE                                    0

#define round                                     1
#define square                                    0


#define SSD1306_NORMALDISPLAY       0xA6
#define SSD1306_INVERTDISPLAY       0xA7

#define buffer_size                               1024//(x_max * y_max)
extern I2C_HandleTypeDef hi2c1;

void OLED_init();
void OLED_reset_sequence();
void OLED_write(unsigned char value, unsigned char type);
void OLED_gotoxy(unsigned char x_pos, unsigned char y_pos);
void OLED_fill(unsigned char bmp_data);
void OLED_clear_screen();
void OLED_set_screen();
void OLED_clear_buffer();
void OLED_set_buffer();
void OLED_cursor(unsigned char x_pos, unsigned char y_pos);
void OLED_print_char(unsigned char x_pos, unsigned char y_pos, unsigned char ch,const unsigned char *font_t);
void OLED_print_char_inv(unsigned char x_pos, unsigned char y_pos, unsigned char ch,const unsigned char *font_t);
void OLED_print_text(unsigned char x_pos, unsigned char y_pos, unsigned char *ch,const unsigned char *font_t);
void OLED_print_text_inv(unsigned char x_pos, unsigned char y_pos, unsigned char *ch,const unsigned char *font_t);
void OLED_Draw_Pixel(unsigned char x_pos, unsigned char y_pos, unsigned char colour);
void OLED_Draw_Line(signed int x1, signed int y1, signed int x2, signed int y2, unsigned char colour);
void OLED_Draw_Line1(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, unsigned char c);
void OLED_Draw_Rectangle(signed int x1, signed int y1, signed int x2, signed int y2, unsigned char fill, unsigned char colour, unsigned char type);
void OLED_Draw_Triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint8_t color);
void OLED_Draw_Circle(signed int xc, signed int yc, signed int radius, unsigned char fill, unsigned char colour);
void OLED_scroll_der(unsigned char inicio, unsigned char fin);
void OLED_scroll_izq(unsigned char inicio, unsigned char fin);
void OLED_scroll_diag_der(unsigned char inicio, unsigned char fin);
void OLED_scroll_diag_izq(unsigned char inicio, unsigned char fin);
void OLED_stop_scroll();
//void OLED_UpdateScreen();
void OLED_DrawBitmap(int16_t x, int16_t y,int16_t w, int16_t h, const unsigned char* bitmap, unsigned char pixel,unsigned char clean);

