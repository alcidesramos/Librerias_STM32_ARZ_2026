//MODIFICADA POR ALCIDES RAMOS 19/09/2025

/*
 * ili9488.h
 *
 *  Created on: Dec 14, 2021
 *      Author: timagr615
 */

#ifndef INC_ILI9488_H_
#define INC_ILI9488_H_

#include <math.h>
#include <stdbool.h>
#include "main.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define RST_A() HAL_GPIO_WritePin(TFT_RST_GPIO_Port,TFT_RST_Pin,GPIO_PIN_RESET)
#define RST_D() HAL_GPIO_WritePin(TFT_RST_GPIO_Port,TFT_RST_Pin,GPIO_PIN_SET)
#define CS_A() HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin,GPIO_PIN_RESET)
#define CS_D() HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin,GPIO_PIN_SET)
#define DC_COMMAND() HAL_GPIO_WritePin(TFT_DC_GPIO_Port,TFT_DC_Pin,GPIO_PIN_RESET)
#define DC_DATA() HAL_GPIO_WritePin(TFT_DC_GPIO_Port,TFT_DC_Pin,GPIO_PIN_SET)
#define TFT_SPI  hspi1
#define ILI9488_TFTWIDTH  320
#define ILI9488_TFTHEIGHT 480

#define horizontal 1
#define  vertical 2
#define  vertical_inv  0
#define horizontal_inv 3

#define rotationNum vertical

/*
#define TFT_CS_Pin GPIO_PIN_0
#define TFT_CS_GPIO_Port GPIOB
#define TFT_RST_Pin GPIO_PIN_1
#define TFT_RST_GPIO_Port GPIOB
#define TFT_DC_Pin GPIO_PIN_2
#define TFT_DC_GPIO_Port GPIOB
#define TFT_LED_Pin GPIO_PIN_10
#define TFT_LED_GPIO_Port GPIOB
*/
extern SPI_HandleTypeDef TFT_SPI;

#define ILI9488_PIXEL_COUNT	ILI9488_TFTWIDTH * ILI9488_TFTHEIGHT
#define ILI9488_NOP     0x00
#define ILI9488_SWRESET 0x01
#define ILI9488_RDDID   0x04
#define ILI9488_RDDST   0x09

#define ILI9488_SLPIN   0x10
#define ILI9488_SLPOUT  0x11
#define ILI9488_PTLON   0x12
#define ILI9488_NORON   0x13

#define ILI9488_RDMODE  0x0A
#define ILI9488_RDMADCTL  0x0B
#define ILI9488_RDPIXFMT  0x0C
#define ILI9488_RDIMGFMT  0x0D
#define ILI9488_RDSELFDIAG  0x0F

#define ILI9488_INVOFF  0x20
#define ILI9488_INVON   0x21
#define ILI9488_GAMMASET 0x26
#define ILI9488_DISPOFF 0x28
#define ILI9488_DISPON  0x29

#define ILI9488_CASET   0x2A
#define ILI9488_PASET   0x2B
#define ILI9488_RAMWR   0x2C
#define ILI9488_RAMRD   0x2E

#define ILI9488_PTLAR   0x30
#define ILI9488_MADCTL  0x36
#define ILI9488_PIXFMT  0x3A

#define ILI9488_FRMCTR1 0xB1
#define ILI9488_FRMCTR2 0xB2
#define ILI9488_FRMCTR3 0xB3
#define ILI9488_INVCTR  0xB4
#define ILI9488_DFUNCTR 0xB6

#define ILI9488_PWCTR1  0xC0
#define ILI9488_PWCTR2  0xC1
#define ILI9488_PWCTR3  0xC2
#define ILI9488_PWCTR4  0xC3
#define ILI9488_PWCTR5  0xC4
#define ILI9488_VMCTR1  0xC5
#define ILI9488_VMCTR2  0xC7

#define ILI9488_RDID1   0xDA
#define ILI9488_RDID2   0xDB
#define ILI9488_RDID3   0xDC
#define ILI9488_RDID4   0xDD

#define ILI9488_GMCTRP1 0xE0
#define ILI9488_GMCTRN1 0xE1
/*
#define ILI9488_PWCTR6  0xFC
*/

// Color definitions
#define ILI9488_BLACK      			0x0000      /*   0,   0,   0 */
#define ILI9488_NAVY				0x000F      /*   0,   0, 128 */
#define ILI9488_DARKGREEN   		0x03E0      /*   0, 128,   0 */
#define ILI9488_DARKCYAN    		0x03EF      /*   0, 128, 128 */
#define ILI9488_MAROON      		0x7800      /* 128,   0,   0 */
#define ILI9488_PURPLE      		0x780F      /* 128,   0, 128 */
#define ILI9488_OLIVE       			0x7BE0      /* 128, 128,   0 */
#define ILI9488_LIGHTGREY   		0xC618      /* 192, 192, 192 */
#define ILI9488_DARKGREY    		0x7BEF      /* 128, 128, 128 */
#define ILI9488_BLUE        			0x001F      /*   0,   0, 255 */
#define ILI9488_GREEN       		0x07E0      /*   0, 255,   0 */
#define ILI9488_CYAN        			0x07FF      /*   0, 255, 255 */
#define ILI9488_RED         			0xF800      /* 255,   0,   0 */
#define ILI9488_MAGENTA     		0xF81F      /* 255,   0, 255 */
#define ILI9488_YELLOW      		0xFFE0      /* 255, 255,   0 */
#define ILI9488_WHITE       			0xFFFF      /* 255, 255, 255 */
#define ILI9488_ORANGE      		0xFD20      /* 255, 165,   0 */
#define ILI9488_GREENYELLOW 	0xAFE5      /* 173, 255,  47 */
#define ILI9488_PINK        			0xF81F

#define	TFT9341_BLACK   0x0000
#define	TFT9341_BLUE    0x001F
#define	TFT9341_RED     0xF800
#define	TFT9341_GREEN   0x07E0
#define TFT9341_CYAN    0x07FF
#define TFT9341_MAGENTA 0xF81F
#define TFT9341_YELLOW  0xFFE0
#define TFT9341_WHITE   0xFFFF
/* RGB 16-bit color table definition (RG565) */
   #define COLOR_BLACK          0x0000      /*   0,   0,   0 */
   #define COLOR_WHITE          0xFFFF      /* 255, 255, 255 */
   #define COLOR_BLUE           0x001F      /*   0,   0, 255 */
   #define COLOR_GREEN          0x07E0      /*   0, 255,   0 */
   #define COLOR_RED            0xF800      /* 255,   0,   0 */
   #define COLOR_NAVY           0x000F      /*   0,   0, 128 */
   #define COLOR_DARKBLUE       0x0011      /*   0,   0, 139 */
   #define COLOR_DARKGREEN      0x03E0      /*   0, 128,   0 */
   #define COLOR_DARKCYAN       0x03EF      /*   0, 128, 128 */
   #define COLOR_CYAN           0x07FF      /*   0, 255, 255 */
   #define COLOR_TURQUOISE      0x471A      /*  64, 224, 208 */
   #define COLOR_INDIGO         0x4810      /*  75,   0, 130 */
   #define COLOR_DARKRED        0x8000      /* 128,   0,   0 */
   #define COLOR_OLIVE          0x7BE0      /* 128, 128,   0 */
   #define COLOR_GRAY           0x8410      /* 128, 128, 128 */
   #define COLOR_GREY           0x8410      /* 128, 128, 128 */
   #define COLOR_SKYBLUE        0x867D      /* 135, 206, 235 */
   #define COLOR_BLUEVIOLET     0x895C      /* 138,  43, 226 */
   #define COLOR_LIGHTGREEN     0x9772      /* 144, 238, 144 */
   #define COLOR_DARKVIOLET     0x901A      /* 148,   0, 211 */
   #define COLOR_YELLOWGREEN    0x9E66      /* 154, 205,  50 */
   #define COLOR_BROWN          0xA145      /* 165,  42,  42 */
   #define COLOR_DARKGRAY       0x7BEF      /* 128, 128, 128 */
   #define COLOR_DARKGREY       0x7BEF      /* 128, 128, 128 */
   #define COLOR_SIENNA         0xA285      /* 160,  82,  45 */
   #define COLOR_LIGHTBLUE      0xAEDC      /* 172, 216, 230 */
   #define COLOR_GREENYELLOW    0xAFE5      /* 173, 255,  47 */
   #define COLOR_SILVER         0xC618      /* 192, 192, 192 */
   #define COLOR_LIGHTGRAY      0xC618      /* 192, 192, 192 */
   #define COLOR_LIGHTGREY      0xC618      /* 192, 192, 192 */
   #define COLOR_LIGHTCYAN      0xE7FF      /* 224, 255, 255 */
   #define COLOR_VIOLET         0xEC1D      /* 238, 130, 238 */
   #define COLOR_AZUR           0xF7FF      /* 240, 255, 255 */
   #define COLOR_BEIGE          0xF7BB      /* 245, 245, 220 */
   #define COLOR_MAGENTA        0xF81F      /* 255,   0, 255 */
   #define COLOR_TOMATO         0xFB08      /* 255,  99,  71 */
   #define COLOR_GOLD           0xFEA0      /* 255, 215,   0 */
   #define COLOR_ORANGE         0xFD20      /* 255, 165,   0 */
   #define COLOR_SNOW           0xFFDF      /* 255, 250, 250 */
   #define COLOR_YELLOW         0xFFE0      /* 255, 255,   0 */

#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04


#define swap(a, b) { int16_t t = a; a = b; b = t; }
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define min(a,b) (((a)<(b))?(a):(b))
#define  set_draw_window setAddrWindow
//***** Functions prototypes *****//
//1. Write Command to LCD
void ILI9341_SendCommand(uint8_t com);
//2. Write data to LCD
void ILI9341_SendData(uint8_t data);
//2.2 Write multiple/DMA
void ILI9488_SendData_Multi(uint8_t *buff, size_t buff_size);
void SendByte(uint8_t data);
void WaitLastData();
//void ILI9488_Init(SPI_HandleTypeDef *spiLcdHandle, GPIO_TypeDef *csPORT, uint16_t csPIN, GPIO_TypeDef *dcPORT, uint16_t dcPIN, GPIO_TypeDef *resetPORT, uint16_t resetPIN);
void ILI9488_Init();
void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void setScrollArea(uint16_t topFixedArea, uint16_t bottomFixedArea);
void scroll(uint16_t pixels);
void pushColor(uint16_t color);
void pushColors(uint16_t *data, uint8_t len, uint8_t first);
void drawImage(const uint8_t* img, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void fillScreen(uint16_t color);

void drawPixel(int16_t x, int16_t y, uint16_t color);
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void draw_linefast(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint16_t color);
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);


void setRotation(uint8_t r);
void invertDisplay(uint8_t  i);
uint16_t color565(uint8_t r, uint8_t g, uint8_t b);
void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
void ILI9488_printText(char text[], int16_t x, int16_t y, uint16_t color, uint16_t bg, uint8_t size);
void write16BitColor(uint16_t color);
void draw_char(uint16_t x_pos, uint16_t y_pos, char ch,uint16_t color, const uint8_t *font_t);
void draw_char1(uint16_t x_pos, uint16_t y_pos, char ch,uint16_t color, const uint8_t *font_t, uint8_t size);
void draw_string(uint16_t x_pos, uint16_t y_pos, const char *str, uint16_t color, const uint8_t *font_t);
void draw_char_fondo(uint16_t x_pos, uint16_t y_pos, char ch,uint16_t color, uint16_t bg_color, const uint8_t *font_t);
void draw_string_fondo(uint16_t x_pos, uint16_t y_pos, const char *str, uint16_t color, uint16_t bg_color, const uint8_t *font_t);
void draw_bitmap(uint16_t x1, uint16_t  y1, int scale, const unsigned int *bmp);
void draw_fill_rectangle(uint16_t  x1, uint16_t  y1, uint16_t  x2, uint16_t  y2, uint16_t  colour);
void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t colour, uint16_t borde);
void draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour, uint16_t borde);
void draw_triangulo(uint16_t x0, uint16_t y0,uint16_t x1, uint16_t y1,uint16_t x2, uint16_t y2,uint16_t colour, uint16_t borde);
void draw_fill_triangulo(uint16_t x0, uint16_t y0,uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t colour);
void draw_fill_circulo(uint16_t xc, uint16_t yc, uint16_t r, uint16_t colour);
void draw_circulo(uint16_t xc, uint16_t yc, uint16_t r, uint16_t colour, uint16_t borde);
void draw_fill_rectangulo_red(uint16_t x, uint16_t y,uint16_t w, uint16_t h,uint16_t r, uint16_t colour);
void draw_arco(uint16_t x0, uint16_t y0, uint16_t r,uint8_t corner, uint16_t colour, uint16_t borde);
void draw_rectangulo_red(uint16_t x, uint16_t y,uint16_t w, uint16_t h,uint16_t r, uint16_t colour, uint16_t borde);
void draw_elipse(int16_t xc, int16_t yc,int16_t rx, int16_t ry,uint16_t colour, uint16_t borde);
void draw_fill_elipse(int16_t xc, int16_t yc,int16_t rx, int16_t ry,uint16_t colour);















#endif /* INC_ILI9488_H_ */
