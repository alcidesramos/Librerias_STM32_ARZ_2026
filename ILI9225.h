
/*MODIFICADA POR ALCIDES RAMOS 08/09/2025*/

/* 
 * File:   ILI9225.h
 * Author: tommy
 *
 * Created on 2 July 2019, 7:33 PM
 */

#ifndef ILI9225_H
#define	ILI9225_H

#ifdef	__cplusplus
extern "C" {
#endif

	//STM32 library for your particular microcontroller
	#include "main.h"

    //coloque tamaño del display  en posicion portrait (vertical)
    #define WIDTH_      176
    #define HEIGHT_     220
//define si se quiere orientacion horizontal sino comente
   #define LANDSCAPE

#define tft_spi_port hspi1  //coloque puerto spi a usar

/*
// DEFINIR SI NO SE HACE SE FORMA GRAFICA
#define TFT_CMD_Pin GPIO_PIN_0
#define TFT_CMD_GPIO_Port GPIOB
#define TFT_RST_Pin GPIO_PIN_1
#define TFT_RST_GPIO_Port GPIOB
#define TFT_CS_Pin GPIO_PIN_2
#define TFT_CS_GPIO_Port GPIOB
#define TFT_LED_Pin GPIO_PIN_10
#define TFT_LED_GPIO_Port GPIOB
 */

/*
 FORMA DE USO



  HAL_GPIO_WritePin(TFT_LED_GPIO_Port, TFT_LED_Pin,1);//ENCIENDE LUZ
  //Initialise the LCD
  lcd_init();
   draw_fill_rectangle(0, 0, WIDTH, HEIGHT,COLOR_WHITE);
  // draw_line(0,0 , 30,30, COLOR_BLACK);

   //draw_rectangle_borde(0, 0, 100, 100,COLOR_RED,4);

   //draw_fill_rectangle(0, 0,  30, 100,COLOR_CYAN);

//   draw_triangulo(50, 30,100, 80,30, 90,COLOR_BLUE);
 //  draw_triangulo_borde(50, 30,100, 80,30, 90,COLOR_BLUE,3);
  // draw_fill_triangulo(50, 30,100, 80,30, 90,COLOR_BLUE);

   //draw_circle(100, 100, 30, COLOR_DARKGREEN);
//   draw_fill_circle(100, 100, 30, COLOR_DARKGREEN);
   //draw_circle_borde(100, 100, 30, COLOR_CYAN,4);

//   draw_string_fondo(0, 0, "Alcides", COLOR_RED, COLOR_BLACK, font1);

  // draw_string(0, 100, "Alcides", COLOR_RED, font4);
  HAL_Delay(1000);
 //  draw_bitmap(0, 0,1, yo);

 draw_bitmap(0, 0,1, zorro);
  //escribe sobre fondo negro
   // draw_string_fondo(0, 0, "ALCIDES", COLOR_RED, COLOR_BLACK, font1);

 //draw_string_fondo(0, 0, "ARZ", COLOR_RED, COLOR_BLACK, font4);

  //imprime de forma trsaprente sin fondo
     draw_string(0, 100, "Alcides", COLOR_RED, font4);
 */

/* ILI9225 LCD Registers */
    #define ILI9225_DRIVER_OUTPUT_CTRL      (0x01u)  // Driver Output Control
    #define ILI9225_LCD_AC_DRIVING_CTRL     (0x02u)  // LCD AC Driving Control
    #define ILI9225_ENTRY_MODE              (0x03u)  // Entry Mode
    #define ILI9225_DISP_CTRL1              (0x07u)  // Display Control 1
    #define ILI9225_BLANK_PERIOD_CTRL1      (0x08u)  // Blank Period Control
    #define ILI9225_FRAME_CYCLE_CTRL        (0x0Bu)  // Frame Cycle Control
    #define ILI9225_INTERFACE_CTRL          (0x0Cu)  // Interface Control
    #define ILI9225_OSC_CTRL                (0x0Fu)  // Osc Control
    #define ILI9225_POWER_CTRL1             (0x10u)  // Power Control 1
    #define ILI9225_POWER_CTRL2             (0x11u)  // Power Control 2
    #define ILI9225_POWER_CTRL3             (0x12u)  // Power Control 3
    #define ILI9225_POWER_CTRL4             (0x13u)  // Power Control 4
    #define ILI9225_POWER_CTRL5             (0x14u)  // Power Control 5
    #define ILI9225_VCI_RECYCLING           (0x15u)  // VCI Recycling
    #define ILI9225_RAM_ADDR_SET1           (0x20u)  // Horizontal GRAM Address Set
    #define ILI9225_RAM_ADDR_SET2           (0x21u)  // Vertical GRAM Address Set
    #define ILI9225_GRAM_DATA_REG           (0x22u)  // GRAM Data Register
    #define ILI9225_GATE_SCAN_CTRL          (0x30u)  // Gate Scan Control Register
    #define ILI9225_VERTICAL_SCROLL_CTRL1   (0x31u)  // Vertical Scroll Control 1 Register
    #define ILI9225_VERTICAL_SCROLL_CTRL2   (0x32u)  // Vertical Scroll Control 2 Register
    #define ILI9225_VERTICAL_SCROLL_CTRL3   (0x33u)  // Vertical Scroll Control 3 Register
    #define ILI9225_PARTIAL_DRIVING_POS1    (0x34u)  // Partial Driving Position 1 Register
    #define ILI9225_PARTIAL_DRIVING_POS2    (0x35u)  // Partial Driving Position 2 Register
    #define ILI9225_HORIZONTAL_WINDOW_ADDR1 (0x36u)  // Horizontal Address Start Position
    #define ILI9225_HORIZONTAL_WINDOW_ADDR2 (0x37u)  // Horizontal Address End Position
    #define ILI9225_VERTICAL_WINDOW_ADDR1   (0x38u)  // Vertical Address Start Position
    #define ILI9225_VERTICAL_WINDOW_ADDR2   (0x39u)  // Vertical Address End Position
    #define ILI9225_GAMMA_CTRL1             (0x50u)  // Gamma Control 1
    #define ILI9225_GAMMA_CTRL2             (0x51u)  // Gamma Control 2
    #define ILI9225_GAMMA_CTRL3             (0x52u)  // Gamma Control 3
    #define ILI9225_GAMMA_CTRL4             (0x53u)  // Gamma Control 4
    #define ILI9225_GAMMA_CTRL5             (0x54u)  // Gamma Control 5
    #define ILI9225_GAMMA_CTRL6             (0x55u)  // Gamma Control 6
    #define ILI9225_GAMMA_CTRL7             (0x56u)  // Gamma Control 7
    #define ILI9225_GAMMA_CTRL8             (0x57u)  // Gamma Control 8
    #define ILI9225_GAMMA_CTRL9             (0x58u)  // Gamma Control 9
    #define ILI9225_GAMMA_CTRL10            (0x59u)  // Gamma Control 10

    #define ILI9225C_INVOFF  0x20
    #define ILI9225C_INVON   0x21

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


    //SPI Bus status register
    //Set this to suit your particular microcontroller
	extern SPI_HandleTypeDef tft_spi_port;//ojo el puerto spi
	extern uint16_t   WIDTH;
	extern uint16_t   HEIGHT;

    void spi_write(unsigned char data);
    void lcd_write_command(unsigned char data);
    void lcd_write_data(unsigned char data);
    void lcd_write_register(unsigned int reg, unsigned int data);
    void lcd_init(void);
    void swap_int(int *num1, int *num2);
    void swap_char(char *num1, char *num2);
    void lcd_init_command_list(void);
    void draw_pixel(uint16_t x, uint16_t y, uint16_t colour);
    void set_draw_window(uint16_t  x1, uint16_t  y1, uint16_t  x2, uint16_t  y2);
    void draw_line_fast(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t colour);
    void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t colour);
    void draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);
    void draw_fill_rectangle(uint16_t x1, uint16_t  y1, uint16_t  x2, uint16_t  y2, uint16_t  colour);
    void draw_rectangle_borde(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour, uint16_t borde);
    void draw_triangulo(uint16_t x0, uint16_t y0,uint16_t x1, uint16_t y1,uint16_t x2, uint16_t y2,uint16_t colour);
    void draw_triangulo_borde(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,uint16_t x2, uint16_t y2,uint16_t colour,uint16_t borde);
    void draw_fill_triangulo(uint16_t x0, uint16_t y0,uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t colour);
    void draw_circle(uint16_t xc, uint16_t yc, uint16_t r, uint16_t colour);
    void draw_fill_circle(uint16_t xc, uint16_t yc, uint16_t r, uint16_t colour);
    void draw_circle_borde(uint16_t xc, uint16_t yc, uint16_t r, uint16_t colour, uint16_t borde);
    void draw_char(uint16_t x_pos, uint16_t y_pos, char ch, uint16_t color, const unsigned char *font_t);
    void draw_char_fondo(uint16_t x_pos, uint16_t y_pos, char ch, uint16_t color, uint16_t bg_color, const uint8_t *font_t);
    void draw_string(uint16_t x_pos, uint16_t y_pos,const char *str,uint16_t color,const unsigned char *font_t);
    void draw_string_fondo(uint16_t x_pos, uint16_t y_pos, const char *str, uint16_t color, uint16_t bg_color, const uint8_t *font_t);
    void draw_bitmap(uint16_t  x, uint16_t  y, int scale, const unsigned int *bmp);
#ifdef	__cplusplus
}
#endif

#endif	/* ILI9225_H */

