/*MODIFICADA POR ALCIDES RAMOS 08/09/2025*/

/*
 * LCD driver for the ILI9225 TFT LCD chips. 
 * 
 * This driver works with OTM2201A, and ILI9926 controller chips.
 * 
 * Feel free to use, change, distribute this code as desired. Use under
 * GPLv3 open-source license.
 * 
 * File:   ILI9225.c
 * Author: tommy
 *
 * Created on 2 July 2019, 7:32 PM
 */


#include "ILI9225.h"
#include "delays.h"
#include "stdbool.h"

//de acuedo ubicacion define ancho y largo
#ifdef LANDSCAPE
uint16_t   WIDTH=HEIGHT_;
uint16_t   HEIGHT=WIDTH_;
#else
uint16_t   WIDTH=WIDTH_;
uint16_t   HEIGHT=HEIGHT_;

#endif


void spi_write(unsigned char data) {
    //TODO: Update these buffer labels according to your device.

	HAL_SPI_Transmit(&tft_spi_port, &data, 1, 100);
}

/*
 * Writes a data byte to the display. Pulls CS low as required.
 */
void lcd_write_data(unsigned char data) {
    //CS LOW
	   HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET);
    //DC HIGH
    HAL_GPIO_WritePin(TFT_CMD_GPIO_Port, TFT_CMD_Pin, GPIO_PIN_SET);
    //Send data to the SPI register
    spi_write(data);
    //CS HIGH
    HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET);
}

/*
 * Writes a command byte to the display
 */
void lcd_write_command(unsigned char data) {
    //Pull the command AND chip select lines LOW
    HAL_GPIO_WritePin(TFT_CMD_GPIO_Port, TFT_CMD_Pin, GPIO_PIN_RESET);
    //TFT_CS = 0;
    HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET);
    spi_write(data);
    //Return the control lines to HIGH
    HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET);
}

/*
 * Writes data to a specific register.
 * Accepts a 16-bit register address, and 16-bits of data.
 */
void lcd_write_register(unsigned int reg, unsigned int data) {
    //Write each register byte, and each data byte seperately.
    lcd_write_command(reg >> 8); //regH
    lcd_write_command(reg & 0xFF); //regL
    lcd_write_data(data >> 8); //dataH
    lcd_write_data(data & 0xFF); //dataL
}

/*
 * Swaps two 16-bit integers
 */
void swap_int(int *num1, int *num2) {
    int temp = *num2;
    *num2 = *num1;
    *num1 = temp;
}

/*
 * Swaps two 8-bit integers
 */
void swap_char(char *num1, char *num2) {
    char temp = *num2;
    *num2 = *num1;
    *num1 = temp;
}




/*
 * Initialisation routine for the LCD
 * I got this from the one of the ebay sellers which make them.
 * From Open-Smart
 */
void lcd_init() {
    delay_Init();//uso mis retardos
    //SET control pins for the LCD HIGH (they are active LOW)
    HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(TFT_CMD_GPIO_Port, TFT_CMD_Pin, GPIO_PIN_RESET); //Data / command select, the datasheet isn't clear on that.
    HAL_GPIO_WritePin(TFT_RST_GPIO_Port, TFT_RST_Pin, GPIO_PIN_SET); //RESET pin HIGH
    
    //Cycle reset pin
    HAL_GPIO_WritePin(TFT_RST_GPIO_Port, TFT_RST_Pin, GPIO_PIN_RESET);
    delay_ms(500);//mis retardos
    HAL_GPIO_WritePin(TFT_RST_GPIO_Port, TFT_RST_Pin, GPIO_PIN_SET);
    delay_ms(500);//mis retardos
    
    lcd_init_command_list();
    
}

/**
 * This is the magic initialisation routine. Supplied by Open-Smart
 * who sell cheap modules on eBay.
 * This routine works with OTM2201A and ILI9925.
 */
void lcd_init_command_list(void)
{
    
    lcd_write_register(ILI9225_POWER_CTRL1, 0x0000); // Set SAP,DSTB,STB
    lcd_write_register(ILI9225_POWER_CTRL2, 0x0000); // Set APON,PON,AON,VCI1EN,VC
    lcd_write_register(ILI9225_POWER_CTRL3, 0x0000); // Set BT,DC1,DC2,DC3
    lcd_write_register(ILI9225_POWER_CTRL4, 0x0000); // Set GVDD
    lcd_write_register(ILI9225_POWER_CTRL5, 0x0000); // Set VCOMH/VCOML voltage
    
    delay_ms(10);
    
    lcd_write_register(ILI9225_POWER_CTRL2, 0xFFFF); // EVERYTHING ON
    lcd_write_register(ILI9225_POWER_CTRL3, 0x7000); // Set BT,DC1,DC2,DC3
    lcd_write_register(ILI9225_POWER_CTRL4, 0x006F); // Set GVDD   /*007F 0088 */
    lcd_write_register(ILI9225_POWER_CTRL5, 0x495F); // Set VCOMH/VCOML voltage
    lcd_write_register(ILI9225_POWER_CTRL1, 0x0F00); // Set SAP,DSTB,STB
    
    delay_ms(10);
    

    lcd_write_register(ILI9225_POWER_CTRL2, 0xFFFF); // Set APON,PON,AON,VCI1EN,VC

    delay_ms(50);


    lcd_write_register(ILI9225_DRIVER_OUTPUT_CTRL, 0x011C); // set the display line number and display direction
    lcd_write_register(ILI9225_LCD_AC_DRIVING_CTRL, 0x0100); // set 1 line inversion
    lcd_write_register(ILI9225_ENTRY_MODE, 0x1030); // set GRAM write direction and BGR=1.
    lcd_write_register(ILI9225_DISP_CTRL1, 0x0000); // Display off
    lcd_write_register(ILI9225_BLANK_PERIOD_CTRL1, 0x0202); // set the back porch and front porch (2 lines, minimum)
    lcd_write_register(ILI9225_FRAME_CYCLE_CTRL, 0x0000); // set the clocks number per line
    lcd_write_register(ILI9225_INTERFACE_CTRL, 0x0000); // CPU interface
    lcd_write_register(ILI9225_OSC_CTRL, 0x0F01); // Set Osc
    lcd_write_register(ILI9225_VCI_RECYCLING, 0x0000); // Set VCI recycling
    lcd_write_register(ILI9225_RAM_ADDR_SET1, 0x0000); // RAM Address
    lcd_write_register(ILI9225_RAM_ADDR_SET2, 0x0000); // RAM Address

    /* Set GRAM area */
    lcd_write_register(ILI9225_GATE_SCAN_CTRL, 0x0000); 
    lcd_write_register(ILI9225_VERTICAL_SCROLL_CTRL1, 0x00DB); 
    lcd_write_register(ILI9225_VERTICAL_SCROLL_CTRL2, 0x0000); 
    lcd_write_register(ILI9225_VERTICAL_SCROLL_CTRL3, 0x0000); 
    lcd_write_register(ILI9225_PARTIAL_DRIVING_POS1, 0x00DB); 
    lcd_write_register(ILI9225_PARTIAL_DRIVING_POS2, 0x0000); 
    lcd_write_register(ILI9225_HORIZONTAL_WINDOW_ADDR1, 0x00AF); 
    lcd_write_register(ILI9225_HORIZONTAL_WINDOW_ADDR2, 0x0000); 
    lcd_write_register(ILI9225_VERTICAL_WINDOW_ADDR1, 0x00DB); 
    lcd_write_register(ILI9225_VERTICAL_WINDOW_ADDR2, 0x0000); 

    /* Set GAMMA curve */
    lcd_write_register(ILI9225_GAMMA_CTRL1, 0x0000); 
    lcd_write_register(ILI9225_GAMMA_CTRL2, 0x0808); 
    lcd_write_register(ILI9225_GAMMA_CTRL3, 0x080A); 
    lcd_write_register(ILI9225_GAMMA_CTRL4, 0x000A); 
    lcd_write_register(ILI9225_GAMMA_CTRL5, 0x0A08); 
    lcd_write_register(ILI9225_GAMMA_CTRL6, 0x0808); 
    lcd_write_register(ILI9225_GAMMA_CTRL7, 0x0000); 
    lcd_write_register(ILI9225_GAMMA_CTRL8, 0x0A00); 
    lcd_write_register(ILI9225_GAMMA_CTRL9, 0x0710); 
    lcd_write_register(ILI9225_GAMMA_CTRL10, 0x0710); 

    lcd_write_register(ILI9225_DISP_CTRL1, 0x0012); 

    delay_ms(50); 
    
    lcd_write_register(ILI9225_DISP_CTRL1, 0x1017);

    
}

/*
 * Draws a single pixel to the LCD at position X, Y, with 
 * Colour.
 * 
 * 28 bytes per pixel. Use it wisely.
 */
void draw_pixel(uint16_t x, uint16_t y, uint16_t colour)
{
#ifdef LANDSCAPE
    // Rotación 90° a la derecha
    uint16_t temp = x;
    x = y;
    y = (WIDTH - 1) - temp;
#endif

    set_draw_window(x, y, x, y); // Dibuja 1 pixel
    lcd_write_data(colour >> 8);
    lcd_write_data(colour & 0xFF);
}


//dibuja linea rapidas horizontal o vertical
void draw_line_fast(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t colour)
{
#ifdef LANDSCAPE
    uint16_t tx0 = y0;
    uint16_t ty0 = WIDTH - 1 - x0;
    uint16_t tx1 = y1;
    uint16_t ty1 = WIDTH - 1 - x1;

    x0 = tx0; x1 = tx1;
    y0 = ty0; y1 = ty1;
#endif

    if (y0 == y1) // línea horizontal
    {
        uint16_t start = x0 < x1 ? x0 : x1;
        uint16_t end   = x0 > x1 ? x0 : x1;
        set_draw_window(start, y0, end, y0);

        HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET);
        for(uint16_t i = 0; i <= end - start; i++)
        {
            lcd_write_data(colour >> 8);
            lcd_write_data(colour & 0xFF);
        }
        HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET);
    }
    else if (x0 == x1) // línea vertical
    {
        uint16_t start = y0 < y1 ? y0 : y1;
        uint16_t end   = y0 > y1 ? y0 : y1;
        set_draw_window(x0, start, x0, end);

        HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET);
        for(uint16_t i = 0; i <= end - start; i++)
        {
            lcd_write_data(colour >> 8);
            lcd_write_data(colour & 0xFF);
        }
        HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET);
    }
    else
    {

    }
}

//mi procedimiento para hacer linea
void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t colour)
{
#ifdef LANDSCAPE
    // Rotación -90° para landscape
    uint16_t tx0 = y0;
    uint16_t ty0 = WIDTH - 1 - x0;
    uint16_t tx1 = y1;
    uint16_t ty1 = WIDTH - 1 - x1;

    x0 = tx0; x1 = tx1;
    y0 = ty0; y1 = ty1;
#endif

    int16_t dx = abs(x1 - x0);
    int16_t sx = x0 < x1 ? 1 : -1;
    int16_t dy = -abs(y1 - y0);
    int16_t sy = y0 < y1 ? 1 : -1;
    int16_t err = dx + dy; // error value

    unsigned char colour_high = colour >> 8;
    unsigned char colour_low  = colour & 0xFF;

    while (1)
    {
        // Dibuja un pixel en (x0, y0)
        set_draw_window(x0, y0, x0, y0);
        HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET);
        lcd_write_data(colour_high);
        lcd_write_data(colour_low);
        HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET);

        if (x0 == x1 && y0 == y1) break;

        int16_t e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

///dibuja una lines con borde
void draw_line_borde(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t colour, uint16_t borde)
{
    int16_t dx = abs(x1 - x0);
    int16_t sx = x0 < x1 ? 1 : -1;
    int16_t dy = -abs(y1 - y0);
    int16_t sy = y0 < y1 ? 1 : -1;
    int16_t err = dx + dy;

    while(1)
    {

        draw_fill_rectangle(x0 - borde/2, y0 - borde/2, x0 + borde/2, y0 + borde/2, colour);

        if (x0 == x1 && y0 == y1) break;
        int16_t e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}
//mi  funcion dibujar rectangulos

void draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{

    if (x2 < x1) { uint16_t tmp = x1; x1 = x2; x2 = tmp; }
    if (y2 < y1) { uint16_t tmp = y1; y1 = y2; y2 = tmp; }

    // Línea superior
    draw_line_fast(x1, y1, x2, y1, colour);

    // Línea inferior
    draw_line_fast(x1, y2, x2, y2, colour);

    // Línea izquierda
    draw_line_fast(x1, y1, x1, y2, colour);

    // Línea derecha
    draw_line_fast(x2, y1, x2, y2, colour);
}


/*
 * Fills a rectangle with a given colour
 */
void draw_fill_rectangle(uint16_t  x1, uint16_t  y1, uint16_t  x2, uint16_t  y2, uint16_t  colour)
{
    //If landscape view then translate everyting -90 degrees
#ifdef LANDSCAPE
	    swap_char(&x1, &y1);
        swap_char(&x2, &y2);
        y1 = WIDTH - y1;
        y2 = WIDTH - y2;
        swap_char(&y2, &y1);
#endif

    //Split the colour int in to two bytes
    unsigned char colour_high = colour >> 8;
    unsigned char colour_low = colour & 0xFF;

    //Set the drawing region
    set_draw_window(x1, y1, x2, y2);

    //We will do the SPI write manually here for speed
    //( the data sheet says it doesn't matter if TFT_CS changes between
    // data sections but I don't trust it.)
    //TFT_CS low to begin data
    HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET);

    //Write colour to each pixel
    for(int y = 0; y < y2-y1+1 ; y++) {
        for(int x = 0; x < x2-x1+1; x++) {
            lcd_write_data(colour_high);
            lcd_write_data(colour_low);
        }
    }
    //Return TFT_CS to high
    HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET);
}

//rectangulo con borde

void draw_rectangle_borde(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour, uint16_t borde)
{
    draw_line_borde(x1, y1, x2, y1, colour, borde);
    draw_line_borde(x2, y1, x2, y2, colour, borde);
    draw_line_borde(x2, y2, x1, y2, colour, borde);
    draw_line_borde(x1, y2, x1, y1, colour, borde);
}

//dibuja triangulo
// Dibuja un triángulo usando tres vértices (x0,y0), (x1,y1), (x2,y2)
void draw_triangulo(uint16_t x0, uint16_t y0,uint16_t x1, uint16_t y1,uint16_t x2, uint16_t y2,uint16_t colour)
{
    // Línea del vértice 0 al 1
    draw_line(x0, y0, x1, y1, colour);

    // Línea del vértice 1 al 2
    draw_line(x1, y1, x2, y2, colour);

    // Línea del vértice 2 al 0
    draw_line(x2, y2, x0, y0, colour);
}

//triangulo con grosor o borde
// Dibuja un triángulo con borde grueso
void draw_triangulo_borde(uint16_t x0, uint16_t y0,uint16_t x1, uint16_t y1,uint16_t x2, uint16_t y2,uint16_t colour, uint16_t borde)
{
    draw_line_borde(x0, y0, x1, y1, colour, borde);
    draw_line_borde(x1, y1, x2, y2, colour, borde);
    draw_line_borde(x2, y2, x0, y0, colour, borde);
}


// Triángulo relleno
void draw_fill_triangulo(uint16_t x0, uint16_t y0,uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t colour)
{
    // Ordenar los vértices por Y ascendente (y0 <= y1 <= y2)
    if (y0 > y1) { uint16_t tmp; tmp=y0; y0=y1; y1=tmp; tmp=x0; x0=x1; x1=tmp; }
    if (y1 > y2) { uint16_t tmp; tmp=y1; y1=y2; y2=tmp; tmp=x1; x1=x2; x2=tmp; }
    if (y0 > y1) { uint16_t tmp; tmp=y0; y0=y1; y1=tmp; tmp=x0; x0=x1; x1=tmp; }

    int16_t total_height = y2 - y0;

    for (int16_t i = 0; i <= total_height; i++)
    {
        bool second_half = i > (y1 - y0) || y1 == y0;
        int16_t segment_height = second_half ? (y2 - y1) : (y1 - y0);

        float alpha = (float)i / total_height;
        float beta  = (float)(i - (second_half ? y1 - y0 : 0)) / segment_height;

        int16_t ax = x0 + (x2 - x0) * alpha;
        int16_t bx = second_half ? x1 + (x2 - x1) * beta : x0 + (x1 - x0) * beta;
        int16_t y = y0 + i;

        if (ax > bx) { int16_t tmp = ax; ax = bx; bx = tmp; }

        draw_fill_rectangle(ax, y, bx, y, colour);
    }
}


//dibuja circulo
void draw_circle(uint16_t xc, uint16_t yc, uint16_t r, uint16_t colour)
{
    int16_t x = 0;
    int16_t y = r;
    int16_t d = 3 - 2 * r;

    while (y >= x)
    {

        draw_fill_rectangle(xc + x, yc + y, xc + x, yc + y, colour);
        draw_fill_rectangle(xc - x, yc + y, xc - x, yc + y, colour);
        draw_fill_rectangle(xc + x, yc - y, xc + x, yc - y, colour);
        draw_fill_rectangle(xc - x, yc - y, xc - x, yc - y, colour);
        draw_fill_rectangle(xc + y, yc + x, xc + y, yc + x, colour);
        draw_fill_rectangle(xc - y, yc + x, xc - y, yc + x, colour);
        draw_fill_rectangle(xc + y, yc - x, xc + y, yc - x, colour);
        draw_fill_rectangle(xc - y, yc - x, xc - y, yc - x, colour);

        x++;
        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
        {
            d = d + 4 * x + 6;
        }
    }
}
//dibuja circulo relleno
void draw_fill_circle(uint16_t xc, uint16_t yc, uint16_t r, uint16_t colour)
{
    int16_t x = 0;
    int16_t y = r;
    int16_t d = 3 - 2 * r;

    while (y >= x)
    {
        draw_fill_rectangle(xc - x, yc - y, xc + x, yc - y, colour);
        draw_fill_rectangle(xc - y, yc - x, xc + y, yc - x, colour);
        draw_fill_rectangle(xc - x, yc + y, xc + x, yc + y, colour);
        draw_fill_rectangle(xc - y, yc + x, xc + y, yc + x, colour);

        x++;
        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
        {
            d = d + 4 * x + 6;
        }
    }
}
//circulo d eun espesor o borde deseado
void draw_circle_borde(uint16_t xc, uint16_t yc, uint16_t r, uint16_t colour, uint16_t borde)
{
    if (borde == 0) return;

    for (uint16_t i = 0; i < borde; i++)
    {
        int16_t x = 0;
        int16_t y = r - i;
        int16_t d = 3 - 2 * (r - i);

        while (y >= x)
        {
            // Dibujar los 8 puntos simétricos
            draw_fill_rectangle(xc + x, yc + y, xc + x, yc + y, colour);
            draw_fill_rectangle(xc - x, yc + y, xc - x, yc + y, colour);
            draw_fill_rectangle(xc + x, yc - y, xc + x, yc - y, colour);
            draw_fill_rectangle(xc - x, yc - y, xc - x, yc - y, colour);
            draw_fill_rectangle(xc + y, yc + x, xc + y, yc + x, colour);
            draw_fill_rectangle(xc - y, yc + x, xc - y, yc + x, colour);
            draw_fill_rectangle(xc + y, yc - x, xc + y, yc - x, colour);
            draw_fill_rectangle(xc - y, yc - x, xc - y, yc - x, colour);

            x++;
            if (d > 0)
            {
                y--;
                d = d + 4 * (x - y) + 10;
            }
            else
            {
                d = d + 4 * x + 6;
            }
        }
    }
}

/*
 * Sets the X,Y position for following commands on the display.
 * Should only be called within a function that draws something
 * to the display.
 * 
 * NOTE: This is 26 bytes. Use it sparingly (see draw_bitmap())
 */
void set_draw_window(uint16_t  x1, uint16_t  y1, uint16_t  x2, uint16_t  y2)
{
    
    //Check that the values are in order
    if(x2 < x1)
        swap_char(&x2, &x1);
    if(y2 < y1)
        swap_char(&y2, &y1);

    lcd_write_register(ILI9225_HORIZONTAL_WINDOW_ADDR1,x2);
    lcd_write_register(ILI9225_HORIZONTAL_WINDOW_ADDR2,x1);

    lcd_write_register(ILI9225_VERTICAL_WINDOW_ADDR1,y2);
    lcd_write_register(ILI9225_VERTICAL_WINDOW_ADDR2,y1);

    lcd_write_register(ILI9225_RAM_ADDR_SET1,x1);
    lcd_write_register(ILI9225_RAM_ADDR_SET2,y1);

    lcd_write_command(0x00);
    lcd_write_command(0x22);
}

void draw_char1(uint16_t x_pos, uint16_t y_pos, char ch,uint16_t color, const uint8_t *font_t)
{
    uint8_t ancho = font_t[0];
    uint8_t alto_bytes = font_t[1];
    uint8_t alto_px = alto_bytes * 8;
    uint16_t columnas = ancho * alto_bytes + 1;
    uint16_t posi = (ch - 32) * columnas + 3;
    uint8_t byte_data;

    for (uint8_t col = 0; col < ancho; col++)
    {
        uint16_t base = posi + col * alto_bytes;
        for (uint8_t byte = 0; byte < alto_bytes; byte++)
        {
            byte_data = font_t[base + byte];
            for (uint8_t bit = 0; bit < 8; bit++)
            {
                if (byte_data & (1 << bit))
                {
                    draw_pixel(x_pos + col, y_pos + byte * 8 + bit, color);
                }
            }
        }
    }
}

void draw_char(uint16_t x_pos, uint16_t y_pos, char ch,uint16_t color, const uint8_t *font_t)
{
    uint8_t ancho = font_t[0];           // ancho en píxeles
    uint8_t alto_bytes = font_t[1];      // alto en bytes
    uint8_t alto_px = alto_bytes * 8;    // alto en píxeles
    uint16_t columnas = ancho * alto_bytes + 1;
    uint16_t posi = (ch - 32) * columnas + 3;

#ifdef LANDSCAPE
    // Ajustar coordenadas para rotación 90°
    uint16_t temp = x_pos;
    x_pos = y_pos;
    y_pos = (WIDTH - 1) - temp - (ancho - 1);

    for (int8_t col = ancho - 1; col >= 0; col--)
    {
        for (uint8_t row = 0; row < alto_px; row++)
        {
            uint16_t base = posi + col * alto_bytes;
            uint8_t byte_data = font_t[base + (row / 8)];

            if (byte_data & (1 << (row % 8))) {
                // Calcula la coordenada rotada de cada pixel
                uint16_t px = x_pos + row;
                uint16_t py = y_pos + (ancho - 1 - col);
                set_draw_window(px, py, px, py);
                lcd_write_data(color >> 8);
                lcd_write_data(color & 0xFF);
            }
        }
    }

#else
    // -------- MODO PORTRAIT --------
    for (uint8_t row = 0; row < alto_px; row++)
    {
        for (uint8_t col = 0; col < ancho; col++)
        {
            uint16_t base = posi + col * alto_bytes;
            uint8_t byte_data = font_t[base + (row / 8)];

            if (byte_data & (1 << (row % 8))) {
                uint16_t px = x_pos + col;
                uint16_t py = y_pos + row;
                set_draw_window(px, py, px, py);
                lcd_write_data(color >> 8);
                lcd_write_data(color & 0xFF);
            }
        }
    }
#endif
}

void draw_char_fondo(uint16_t x_pos, uint16_t y_pos, char ch,uint16_t color, uint16_t bg_color, const uint8_t *font_t)
{
    uint8_t ancho = font_t[0];           // ancho en píxeles
    uint8_t alto_bytes = font_t[1];      // alto en bytes
    uint8_t alto_px = alto_bytes * 8;    // alto en píxeles
    uint16_t columnas = ancho * alto_bytes + 1;
    uint16_t posi = (ch - 32) * columnas + 3;

#ifdef LANDSCAPE
    // Ajustar coordenadas para rotación 90°
    uint16_t temp = x_pos;
    x_pos = y_pos;
    y_pos = (WIDTH - 1) - temp - (ancho-1);


    set_draw_window(x_pos, y_pos, x_pos + alto_px - 1, y_pos + ancho - 1);


    for (int8_t col = ancho - 1; col >= 0; col--)
    {
        for (uint8_t row = 0; row < alto_px; row++)
        {
            uint16_t base = posi + col * alto_bytes;
            uint8_t byte_data = font_t[base + (row / 8)];
            uint16_t c = (byte_data & (1 << (row % 8))) ? color : bg_color;
            lcd_write_data(c >> 8);
            lcd_write_data(c & 0xFF);
        }
    }

#else
    // -------- MODO PORTRAIT --------
    set_draw_window(x_pos, y_pos, x_pos + ancho - 1, y_pos + alto_px - 1);

    for (uint8_t row = 0; row < alto_px; row++)
    {
        for (uint8_t col = 0; col < ancho; col++)
        {
            uint16_t base = posi + col * alto_bytes;
            uint8_t byte_data = font_t[base + (row / 8)];
            uint16_t c = (byte_data & (1 << (row % 8))) ? color : bg_color;
            lcd_write_data(c >> 8);
            lcd_write_data(c & 0xFF);
        }
    }
#endif
}


void draw_string(uint16_t x_pos, uint16_t y_pos, const char *str, uint16_t color, const uint8_t *font_t)
{
    uint8_t ancho = font_t[0];

    while(*str)
    {
        draw_char(x_pos, y_pos, *str, color, font_t);
        x_pos += ancho;  // Avanza para siguiente caracter
        str++;
    }
}



void draw_string_fondo(uint16_t x_pos, uint16_t y_pos, const char *str, uint16_t color, uint16_t bg_color, const uint8_t *font_t)
{
    uint8_t ancho = font_t[0];
    uint8_t alto_bytes = font_t[1];
    uint8_t alto_px = alto_bytes * 8;

    while(*str)
    {
        // Dibuja cada caracter en la posición actual
        draw_char_fondo(x_pos, y_pos, *str, color, bg_color, font_t);

        // Avanza en X para el siguiente caracter
        x_pos += ancho;

        str++;
    }
}



/*
 * Draws a bitmap by directly writing the byte stream to the LCD.
 * 
 * So the scaling is done strangely here because writing individual pixels 
 * has an overhead of 26 bytes each.
 */
void draw_bitmap(uint16_t x1, uint16_t  y1, int scale, const unsigned int *bmp)
{
	int width = bmp[0];
	int height = bmp[1];
	unsigned int this_byte;
	int x2 = x1 + (width * scale);
	int y2 = y1 + (height * scale);

	//If landscape view then translate everyting -90 degrees
#ifdef LANDSCAPE

		swap_char(&x1, &y1);
		swap_char(&x2, &y2);
		y1 = WIDTH - y1;
		y2 = WIDTH - y2;
		swap_char(&y2, &y1);
		swap_int(&width, &height);
#endif


	//Set the drawing region
	set_draw_window(x1, y1, x2 + scale - 1, y2);

	//We will do the SPI write manually here for speed
	//TFT_CS low to begin data
    HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET);

	//Write colour to each pixel
	for (int i = 0; i < height; i++) {
		//this loop does the vertical axis scaling (two of each line))
		for (int sv = 0; sv < scale; sv++) {
			for (int j = 0; j <= width; j++) {
				//Choose which byte to display depending on the screen orientation
				//NOTE: We add a byte because of the first two bytes being dimension data in the array
					#ifdef LANDSCAPE
					this_byte = bmp[(height * (j + 1)) - i + 1];
					#else
					this_byte = bmp[(width * (i)) + j + 1];
                    #endif
				//And this loop does the horizontal axis scale (two of each pixels on the line))
				for (int sh = 0; sh < scale; sh++) {
					//Draw this pixel
					lcd_write_data(this_byte >> 8);
					lcd_write_data(this_byte & 0xFF);
				}
			}
		}
	}

	//Return TFT_CS to high
    HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET);

}
