//Por alcides Ramos
// 10/01/2026



#include "main.h"

#ifndef __TFT_H__
#define __TFT_H__

#ifdef __cplusplus
extern "C"
{
#endif

// Puerto SPI a usar
#define TFT_SPI hspi1

// Orientación de la pantalla
#define Vertical 0
#define Horizontal 1
#define Vertical_Inv 2
#define Horizontal_Inv 3

extern SPI_HandleTypeDef TFT_SPI;

#include <stdint.h>

typedef struct
{
    uint16_t width;      // Ancho de la TFT
    uint16_t height;     // Alto de la TFT
    uint32_t id;         // ID del controlador TFT
    uint8_t dir;         // Orientación: 0 vertical, 1 horizontal
    uint16_t wramcmd;    // Comando para escribir en GRAM
    uint16_t setxcmd;    // Comando para configurar coordenada X
    uint16_t setycmd;    // Comando para configurar coordenada Y
} _TFT_dev;

// Parámetros principales de la TFT
extern _TFT_dev TFTdev;

///////////////////////////////////// ZONA DE CONFIGURACIÓN ///////////////////////////////////
#define USE_HORIZONTAL 0 // Rotación de la pantalla:
                         // 0 = 0°, 1 = 90°, 2 = 180°, 3 = 270°
///////////////////////////////////////////////////////////////////////////////////////////////

// Dimensiones de la pantalla
#define TFT_W 240
#define TFT_H 320

// Colores básicos
#define WHITE   0xFFFF
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x7FFF
#define YELLOW  0xFFE0
#define MAGENTA 0xF81F

#define BRED    0xF81F
#define GRED    0xFFE0
#define GBLUE   0x07FF

#define BROWN   0xBC40
#define BRRED   0xFC07
#define GRAY    0x8430

// Colores GUI
#define DARKBLUE    0x01CF
#define LIGHTBLUE   0x7D7C
#define GRAYBLUE    0x5458

#define LIGHTGREEN  0x841F
#define LIGHTGRAY   0xEF5B
#define LGRAY       0xC618
#define LGRAYBLUE   0xA651
#define LBBLUE      0x2B12


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

//mis colores
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
// Prototipos de funciones
void TFT_RESET(void);
void SendCommand(uint8_t reg);
void SendData(uint8_t data);
void WriteReg(uint8_t reg, uint16_t regdata);
void WriteRAM_Prepare(void);
void  write16BitColor(uint16_t Data);

void ILI9341_direction(uint8_t direction);
void  setAddrWindow(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd);
void ILI9341_SetCursor(uint16_t Xpos, uint16_t Ypos);

void ILI9341_Clear(uint16_t Color);
void ILI9341_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color);
void ILI9341_DrawPoint(uint16_t x, uint16_t y, uint16_t color);
void ILI9341_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void ILI9341_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void ILI9341_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);

uint32_t TFT_Get_Id(void);
void ILI9341_Init();
//MIAS
void drawPixel(int16_t x, int16_t y, uint16_t color);
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void draw_line_fast(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t colour);
void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint16_t color);
void draw_fill_rectangle(uint16_t  x1, uint16_t  y1, uint16_t  x2, uint16_t  y2, uint16_t  colour);
void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t colour, uint16_t borde);
void draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour, uint16_t borde);
void draw_triangulo(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour, uint16_t borde);
void draw_fill_triangulo(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);
void draw_fill_circulo(uint16_t xc, uint16_t yc, uint16_t r, uint16_t colour);
void draw_circulo(uint16_t xc, uint16_t yc, uint16_t r, uint16_t colour, uint16_t borde);
void draw_arco(uint16_t x0, uint16_t y0, uint16_t r, uint8_t corner, uint16_t colour, uint16_t borde);
void draw_fill_rectangulo_red(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t colour);
void draw_rectangulo_red(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t colour, uint16_t borde);
void draw_elipse(int16_t xc, int16_t yc, int16_t rx, int16_t ry, uint16_t colour, uint16_t borde);
void draw_fill_elipse(int16_t xc, int16_t yc, int16_t rx, int16_t ry, uint16_t colour);
void draw_char(uint16_t x_pos, uint16_t y_pos, char ch,uint16_t color, const uint8_t *font_t);
void draw_char_fondo(uint16_t x_pos, uint16_t y_pos, char ch,uint16_t color, uint16_t bg_color, const uint8_t *font_t);
void draw_string(uint16_t x_pos, uint16_t y_pos, const char *str, uint16_t color, const uint8_t *font_t);
void draw_string_fondo(uint16_t x_pos, uint16_t y_pos, const char *str, uint16_t color, uint16_t bg_color, const uint8_t *font_t);
void draw_bitmap(uint16_t x1, uint16_t  y1, int scale, const unsigned int *bmp);




#ifdef __cplusplus
}
#endif

#endif
