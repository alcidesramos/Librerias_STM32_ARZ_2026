//MODIFICADA POR ALCIDES RAMOS 19/09/2025

/*
 * ili9488.c
 *
 *  Created on: Dec 14, 2021
 *      Author: timagr615
 */
#include "ili9488.h"
#include <math.h>


uint16_t width;
uint16_t height;

//***** Functions prototypes *****//

void SendByte(uint8_t data)
{
  //while((SPI1->SR & SPI_SR_TXE) == RESET);
  //SPI1->DR = data;
//mas universal
	HAL_SPI_Transmit(&TFT_SPI, &data, 1, HAL_MAX_DELAY);
}
//1. Write Command to LCD
void ILI9488_SendCommand(uint8_t com)
{
	//*(__IO uint8_t *)(0x60000000) = com;
	uint8_t tmpCmd = com;
	//Set DC HIGH for COMMAND mode
	//HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_RESET);
	DC_COMMAND();
	//Put CS LOW
	//HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin, GPIO_PIN_RESET);
	CS_A();
	//Write byte using SPI
	HAL_SPI_Transmit(&TFT_SPI, &tmpCmd, 1, 1);
	//SendByte(tmpCmd);
	//WaitLastData();
	CS_D();
	//Bring CS HIGH
	//HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin, GPIO_PIN_SET);
}

//2. Write data to LCD
void ILI9488_SendData(uint8_t data)
{
	//*(__IO uint8_t *)(0x60040000) = data;
	uint8_t tmpCmd = data;
	//Set DC LOW for DATA mode
	//HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET);
	DC_DATA();
	//Put CS LOW
	//HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin, GPIO_PIN_RESET);
	CS_A();
	//Write byte using SPI
	HAL_SPI_Transmit(&TFT_SPI, &tmpCmd, 1, 1);
	//SendByte(tmpCmd);
	//WaitLastData();
	CS_D();

	//Bring CS HIGH
	//HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin, GPIO_PIN_SET);
}
//2.2 Write multiple/DMA
void ILI9488_SendData_Multi(uint8_t *buff, size_t buff_size){
	DC_DATA();
	/*CS_A();
	for (uint32_t i = 0; i < buff_size; i++)
	  {
	    SendByte(*buff);
	    buff++;
	  }

	  WaitLastData();
	  CS_D();*/
	CS_A();
	while (buff_size > 0)
	{

		uint16_t chunk_size = buff_size > 32768 ? 32768 : buff_size;
		HAL_SPI_Transmit(&TFT_SPI, buff, chunk_size, HAL_MAX_DELAY);
		buff += chunk_size;
		buff_size -= chunk_size;
	}
	CS_D();

}
//la quite
/*
void WaitLastData()
{
	while(!(SPI1->SR & SPI_SR_TXE));
		while(SPI1->SR & SPI_SR_BSY);

}
//
/*void ILI9488_SendData_Multi(uint16_t Colordata, uint32_t size)
{
	uint8_t colorL,colorH;

	//Set DC LOW for DATA mode
	//HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET);
	DC_DATA();
	//Put CS LOW
	//HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin, GPIO_PIN_RESET);
	CS_A();
	//Write byte using SPI
	HAL_SPI_Transmit(&lcdSPIhandle, (uint8_t *)&Colordata, size, 1000);
	//Wait for end of DMA transfer
	//Bring CS HIGH
	CS_D();
	//HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin, GPIO_PIN_SET);
}*/

//void ILI9488_Init(SPI_HandleTypeDef *spiLcdHandle, GPIO_TypeDef *csPORT, uint16_t csPIN, GPIO_TypeDef *dcPORT, uint16_t dcPIN, GPIO_TypeDef *resetPORT, uint16_t resetPIN)
void ILI9488_Init()
{
	CS_D();
	RST_A();
		 HAL_Delay(10);
		 RST_D();
	 width=ILI9488_TFTWIDTH;
	 height=ILI9488_TFTHEIGHT;
	 //CS_A();
	 ILI9488_SendCommand(0xE0);
	 ILI9488_SendData(0x00);
	 ILI9488_SendData(0x03);
	 ILI9488_SendData(0x09);
	 ILI9488_SendData(0x08);
	 ILI9488_SendData(0x16);
	 ILI9488_SendData(0x0A);
	 ILI9488_SendData(0x3F);
	 ILI9488_SendData(0x78);
	 ILI9488_SendData(0x4C);
	 ILI9488_SendData(0x09);
	 ILI9488_SendData(0x0A);
	 ILI9488_SendData(0x08);
	 ILI9488_SendData(0x16);
	 ILI9488_SendData(0x1A);
	 ILI9488_SendData(0x0F);

	 	ILI9488_SendCommand(0XE1);
	 	ILI9488_SendData(0x00);
	 	ILI9488_SendData(0x16);
	 	ILI9488_SendData(0x19);
	 	ILI9488_SendData(0x03);
	 	ILI9488_SendData(0x0F);
	 	ILI9488_SendData(0x05);
	 	ILI9488_SendData(0x32);
	 	ILI9488_SendData(0x45);
	 	ILI9488_SendData(0x46);
	 	ILI9488_SendData(0x04);
	 	ILI9488_SendData(0x0E);
	 	ILI9488_SendData(0x0D);
	 	ILI9488_SendData(0x35);
	 	ILI9488_SendData(0x37);
	 	ILI9488_SendData(0x0F);

	 	ILI9488_SendCommand(0XC0);      //Power Control 1
	 	ILI9488_SendData(0x17);    //Vreg1out
	 	ILI9488_SendData(0x15);    //Verg2out

	 	ILI9488_SendCommand(0xC1);      //Power Control 2
	 	ILI9488_SendData(0x41);    //VGH,VGL

	 	ILI9488_SendCommand(0xC5);      //Power Control 3
	 	ILI9488_SendData(0x00);
	 	ILI9488_SendData(0x12);    //Vcom
	 	ILI9488_SendData(0x80);

	 	ILI9488_SendCommand(0x36);      //Memory Access
	 	ILI9488_SendData(0x48);

	 	ILI9488_SendCommand(0x3A);      // Interface Pixel Format
	 	ILI9488_SendData(0x66); 	  //18 bit

	 	ILI9488_SendCommand(0XB0);      // Interface Mode Control
	 	ILI9488_SendData(0x80);     			 //SDO NOT USE

	 	ILI9488_SendCommand(0xB1);      //Frame rate
	 	ILI9488_SendData(0xA0);    //60Hz

	 	ILI9488_SendCommand(0xB4);      //Display Inversion Control
	 	ILI9488_SendData(0x02);    //2-dot

	 	ILI9488_SendCommand(0XB6); //Display Function Control  RGB/MCU Interface Control

	 	ILI9488_SendData(0x02);    //MCU
	 	ILI9488_SendData(0x02);    //Source,Gate scan dieection

	 	ILI9488_SendCommand(0XE9);      // Set Image Functio
	 	ILI9488_SendData(0x00);    // Disable 24 bit data

	 	ILI9488_SendCommand(0xF7);      // Adjust Control
	 	ILI9488_SendData(0xA9);
	 	ILI9488_SendData(0x51);
	 	ILI9488_SendData(0x2C);
	 	ILI9488_SendData(0x82);    // D7 stream, loose

	 	ILI9488_SendCommand(ILI9488_SLPOUT);    //Exit Sleep

	 	HAL_Delay(120);

	 	ILI9488_SendCommand(ILI9488_DISPON);    //Display on
	 	HAL_Delay(200);
}

void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	/*ILI9488_SendCommand(ILI9488_CASET); // Column addr set
	ILI9488_SendData(x0 >> 8);
	ILI9488_SendData(x0 & 0xFF);     // XSTART
	ILI9488_SendData(x1 >> 8);
	ILI9488_SendData(x1 & 0xFF);     // XEND
	ILI9488_SendCommand(ILI9488_PASET); // Row addr set
	ILI9488_SendData(y0 >> 8);
	ILI9488_SendData(y0 & 0xff);     // YSTART
	ILI9488_SendData(y1 >> 8);
	ILI9488_SendData(y1 & 0xff);     // YEND
	ILI9488_SendCommand(ILI9488_RAMWR); // write to RAM*/
	ILI9488_SendCommand(ILI9488_CASET); // Column addr set
		{
		uint8_t data[] = {(x0 >> 8) & 0xFF, x0 & 0xFF, (x1 >> 8) & 0xFF, x1 & 0xFF};
		ILI9488_SendData_Multi(data, sizeof(data));
		}
		ILI9488_SendCommand(ILI9488_PASET);
		{
		uint8_t data[] = {(y0 >> 8) & 0xFF, y0 & 0xFF, (y1 >> 8) & 0xFF, y1 & 0xFF};
		ILI9488_SendData_Multi(data, sizeof(data));
		}
		ILI9488_SendCommand(ILI9488_RAMWR); // write to RAM*/
}

void setScrollArea(uint16_t topFixedArea, uint16_t bottomFixedArea)
{
	ILI9488_SendCommand(0x33); // Vertical scroll definition
	ILI9488_SendData(topFixedArea >> 8);
	ILI9488_SendData(topFixedArea);
	ILI9488_SendData((height - topFixedArea - bottomFixedArea) >> 8);
	ILI9488_SendData(height - topFixedArea - bottomFixedArea);
	ILI9488_SendData(bottomFixedArea >> 8);
	ILI9488_SendData(bottomFixedArea);
}

void scroll(uint16_t pixels)
{
	ILI9488_SendCommand(0x37); // Vertical scrolling start address
	ILI9488_SendData(pixels >> 8);
	ILI9488_SendData(pixels);
}
void pushColor(uint16_t color)
{
	HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin, GPIO_PIN_RESET);
	write16BitColor(color);
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin, GPIO_PIN_SET);

}

void pushColors(uint16_t *data, uint8_t len, uint8_t first)
{
	uint16_t color;
	uint8_t buff[len * 3 + 1];
	uint16_t count = 0;
	uint8_t lencount = len;
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin, GPIO_PIN_RESET);

	if (first == 1) {
		HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET);
	}
	while (lencount--)
	{
		color = *data++;
		buff[count] = (((color & 0xF800) >> 11) * 255) / 31;
		count++;
		buff[count] = (((color & 0x07E0) >> 5) * 255) / 63;
		count++;
		buff[count] = ((color & 0x001F) * 255) / 31;
		count++;
	}
	HAL_SPI_Transmit(&TFT_SPI, buff, len * 3, 100);
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin, GPIO_PIN_SET);
}

void drawImage(const uint8_t* img, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{

	if ((x >= width) || (y >= height))
		return;
	if ((x + w - 1) >= width)
		w = width - x;
	if ((y + h - 1) >= height)
		h = height - y;
	setAddrWindow(x, y, x + w - 1, y + h - 1);
	HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin, GPIO_PIN_RESET);

	uint8_t linebuff[w * 3 + 1];
	uint32_t count = 0;
	for (uint16_t i = 0; i < h; i++) {
		uint16_t pixcount = 0;
		for (uint16_t o = 0; o < w; o++) {
			uint8_t b1 = img[count];
			count++;
			uint8_t b2 = img[count];
			count++;
			uint16_t color = b1 << 8 | b2;
			linebuff[pixcount] = (((color & 0xF800) >> 11) * 255)
					/ 31;
			pixcount++;
			linebuff[pixcount] = (((color & 0x07E0) >> 5) * 255)
					/ 63;
			pixcount++;
			linebuff[pixcount] = ((color & 0x001F) * 255) / 31;
			pixcount++;
		}
		HAL_SPI_Transmit(&TFT_SPI, linebuff, w * 3, 100);

	}

	HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin, GPIO_PIN_SET);
}


void drawPixel(int16_t x, int16_t y, uint16_t color)
{
	if ((x < 0) || (x >= width) || (y < 0) || (y >= height))
		return;

	setAddrWindow(x, y, x + 1, y + 1);
	HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin, GPIO_PIN_RESET);

	write16BitColor(color);
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin, GPIO_PIN_SET);

}

void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{

	if ((x >= width) || (y >= height))
		return;

	if ((y + h - 1) >= height)
		h = height - y;

	setAddrWindow(x, y, x, y + h - 1);
	HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin, GPIO_PIN_RESET);

	while (h--) {
		write16BitColor(color);
	}
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin, GPIO_PIN_SET);

}

void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{

	if ((x >= width) || (y >= height))
		return;
	if ((x + w - 1) >= width)
		w = width - x;
	setAddrWindow(x, y, x + w - 1, y);
	HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin, GPIO_PIN_RESET);
	while (w--)
	{
		write16BitColor(color);
	}
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin, GPIO_PIN_SET);
}

void draw_linefast(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
	 if (x0 == x1) {
	    if (y0 > y1)
	      swap(y0, y1);
	    drawFastVLine(x0, y0, y1 - y0 + 1, color);
	  } else if (y0 == y1) {
	    if (x0 > x1)
	      swap(x0, x1);
	    drawFastHLine(x0, y0, x1 - x0 + 1, color);
	  } else {

	    writeLine(x0, y0, x1, y1, color);

	  }

}

void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {

  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
	  swap(x0, x1);
	  swap(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0 <= x1; x0++) {
    if (steep) {
      drawPixel(y0, x0, color);
    } else {
      drawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}
//6. Fill the entire screen with a background color

void fillScreen(uint16_t color)
{
	fillRect(0, 0,  width, height, color);
}

void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{

	uint32_t i, n, cnt, buf_size;
	if ((x >= width) || (y >= height))
		return;
	if ((x + w - 1) >= width)
		w = width - x;
	if ((y + h - 1) >= height)
		h = height - y;
	setAddrWindow(x, y, x + w - 1, y + h - 1);
	uint8_t r = (color & 0xF800) >> 11;
	uint8_t g = (color & 0x07E0) >> 5;
	uint8_t b = color & 0x001F;

	r = (r * 255) / 31;
	g = (g * 255) / 63;
	b = (b * 255) / 31;

	n = w*h*3;
	if (n <= 65535){
		cnt = 1;
		buf_size = n;
	}
	else {
		cnt = n/3;
		buf_size = 3;
		uint8_t min_cnt = n/65535+1;
		for (i=min_cnt; i < n/3; i++){
			if(n%i == 0){
				cnt = i;
				buf_size = n/i;
				break;
			}
		}
	}
	uint8_t frm_buf[buf_size];
	for (i=0; i < buf_size/3; i++)
	{
		frm_buf[i*3] = r;
		frm_buf[i*3+1] = g;
		frm_buf[i*3+2] = b;
	}
	DC_DATA();
	CS_A();
		while(cnt>0)
		{
			HAL_SPI_Transmit(&TFT_SPI, frm_buf, buf_size, HAL_MAX_DELAY);

			cnt -= 1;
		}
		CS_D();

}


void setRotation(uint8_t r)
{

	ILI9488_SendCommand(ILI9488_MADCTL);
	uint8_t rotation = r % 4; // can't be higher than 3
	switch (rotation) {
	case 0:
		ILI9488_SendData(MADCTL_MX | MADCTL_BGR);
		width = ILI9488_TFTWIDTH;
		height = ILI9488_TFTHEIGHT;
		break;
	case 1:
		ILI9488_SendData(MADCTL_MV | MADCTL_BGR);
		width = ILI9488_TFTHEIGHT;
		height = ILI9488_TFTWIDTH;
		break;
	case 2:
		ILI9488_SendData(MADCTL_MY | MADCTL_BGR);
		width = ILI9488_TFTWIDTH;
		height = ILI9488_TFTHEIGHT;
		break;
	case 3:
		ILI9488_SendData(MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
		width = ILI9488_TFTHEIGHT;
		height = ILI9488_TFTWIDTH;
		break;
	}

}

void invertDisplay(uint8_t i)
{

	ILI9488_SendCommand(i ? ILI9488_INVON : ILI9488_INVOFF);

}
uint16_t color565(uint8_t r, uint8_t g, uint8_t b)
{
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}


void draw_char(uint16_t x_pos, uint16_t y_pos, char ch,uint16_t color, const uint8_t *font_t)
{
    uint8_t ancho      = font_t[0];           // ancho en píxeles
    uint8_t alto_bytes = font_t[1];           // alto en bytes
    uint8_t alto_px    = alto_bytes * 8;      // alto en píxeles
    uint16_t columnas  = ancho * alto_bytes + 1;
    uint16_t posi      = (ch - 32) * columnas + 3;

    // chequeo de límites básicos
    if ((x_pos >= width) || (y_pos >= height)) return;
      if ((x_pos + ancho - 1) < 0 || (y_pos + alto_px - 1) < 0) return;

    // recorre filas y columnas de la fuente
    for (uint8_t row = 0; row < alto_px; row++) {
        for (uint8_t col = 0; col < ancho; col++) {
            uint16_t base = posi + col * alto_bytes;
            uint8_t byte_data = font_t[base + (row / 8)];

            if (byte_data & (1 << (row % 8))) {
                drawPixel(x_pos + col, y_pos + row, color);
            }
        }
    }
}


void draw_char_fondo(uint16_t x_pos, uint16_t y_pos, char ch,uint16_t color, uint16_t bg_color, const uint8_t *font_t)
{
    uint8_t ancho      = font_t[0];        // ancho en píxeles
    uint8_t alto_bytes = font_t[1];        // alto en bytes
    uint8_t alto_px    = alto_bytes * 8;   // alto en píxeles
    uint16_t columnas  = ancho * alto_bytes + 1;
    uint16_t posi      = (ch - 32) * columnas + 3;

    // chequeo de límites
    if ((x_pos >= width) || (y_pos >= height)) return;
    if ((x_pos + ancho - 1) < 0 || (y_pos + alto_px - 1) < 0) return;

    // ventana de escritura: ancho x alto
    setAddrWindow(x_pos, y_pos, x_pos + ancho - 1, y_pos + alto_px - 1);

    // recorre filas y columnas
    for (uint8_t row = 0; row < alto_px; row++) {
        for (uint8_t col = 0; col < ancho; col++) {
            uint16_t base      = posi + col * alto_bytes;
            uint8_t byte_data  = font_t[base + (row / 8)];
            uint16_t c         = (byte_data & (1 << (row % 8))) ? color : bg_color;

            HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin, GPIO_PIN_RESET);

            write16BitColor(c);

            HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin, GPIO_PIN_SET);
        }
    }
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

//dibuja linea rapidas horizontal o vertical
void draw_line_fast(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t colour)
{

    if (y0 == y1) // línea horizontal
    {
        uint16_t start = x0 < x1 ? x0 : x1;
        uint16_t end   = x0 > x1 ? x0 : x1;
        setAddrWindow(start, y0, end, y0);

        HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET);
        for(uint16_t i = 0; i <= end - start; i++)
        {
        	ILI9488_SendData(colour >> 8);
        	ILI9488_SendData(colour & 0xFF);
        }
        HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET);
    }
    else if (x0 == x1) // línea vertical
    {
        uint16_t start = y0 < y1 ? y0 : y1;
        uint16_t end   = y0 > y1 ? y0 : y1;
        setAddrWindow(x0, start, x0, end);

        HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET);
        for(uint16_t i = 0; i <= end - start; i++)
        {
        	write16BitColor(colour);
        }
        HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET);
    }
    else
    {

    }
}


void draw_fill_rectangle(uint16_t  x1, uint16_t  y1, uint16_t  x2, uint16_t  y2, uint16_t  colour)
{
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

        	write16BitColor(colour);
        }
    }
    //Return TFT_CS to high
    HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET);
}

//dibuja una lines con borde

void draw_line(uint16_t x0, uint16_t y0,uint16_t x1, uint16_t y1,uint16_t colour, uint16_t borde)
{
    float dx = x1 - x0;
    float dy = y1 - y0;
    float len = sqrtf(dx*dx + dy*dy);
    if (len == 0) return;

    // vector perpendicular normalizado
    float px = -dy / len;
    float py = dx / len;

    float half = borde / 2.0f;

    // coordenadas de los 4 vértices del rectángulo
    float x0a = x0 + px * half;
    float y0a = y0 + py * half;
    float x0b = x0 - px * half;
    float y0b = y0 - py * half;

    float x1a = x1 + px * half;
    float y1a = y1 + py * half;
    float x1b = x1 - px * half;
    float y1b = y1 - py * half;
    uint16_t xmin = fminf(fminf(x0a, x0b), fminf(x1a, x1b));
    uint16_t xmax = fmaxf(fmaxf(x0a, x0b), fmaxf(x1a, x1b));
    uint16_t ymin = fminf(fminf(y0a, y0b), fminf(y1a, y1b));
    uint16_t ymax = fmaxf(fmaxf(y0a, y0b), fmaxf(y1a, y1b));

    for (uint16_t y = ymin; y <= ymax; y++) {
        for (uint16_t x = xmin; x <= xmax; x++) {
            // prueba si (x,y) está dentro del polígono
            // usando regla del área o cross product
            float c1 = (x1a-x0a)*(y-y0a) - (y1a-y0a)*(x-x0a);
            float c2 = (x1b-x1a)*(y-y1a) - (y1b-y1a)*(x-x1a);
            float c3 = (x0b-x1b)*(y-y1b) - (y0b-y1b)*(x-x1b);
            float c4 = (x0a-x0b)*(y-y0b) - (y0a-y0b)*(x-x0b);

            if ((c1>=0 && c2>=0 && c3>=0 && c4>=0) ||
                (c1<=0 && c2<=0 && c3<=0 && c4<=0)) {
                draw_fill_rectangle(x, y, x, y, colour); // píxel
            }
        }
    }
}
//rectangulo con borde
void draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour, uint16_t borde)
{
    draw_line(x1, y1, x2, y1, colour, borde);
    draw_line(x2, y1, x2, y2, colour, borde);
    draw_line(x2, y2, x1, y2, colour, borde);
    draw_line(x1, y2, x1, y1, colour, borde);
}

//dibuja triangulo
// Dibuja un triángulo usando tres vértices (x0,y0), (x1,y1), (x2,y2)
// Dibuja un triángulo con borde grueso
void draw_triangulo(uint16_t x0, uint16_t y0,uint16_t x1, uint16_t y1,uint16_t x2, uint16_t y2,uint16_t colour, uint16_t borde)
{
    draw_line(x0, y0, x1, y1, colour, borde);
    draw_line(x1, y1, x2, y2, colour, borde);
    draw_line(x2, y2, x0, y0, colour, borde);
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

//dibuja circulo relleno
void draw_fill_circulo(uint16_t xc, uint16_t yc, uint16_t r, uint16_t colour)
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
//dibujka circulo
//circulo d eun espesor o borde deseado
void draw_circulo(uint16_t xc, uint16_t yc, uint16_t r, uint16_t colour, uint16_t borde)
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
void draw_arco(uint16_t x0, uint16_t y0, uint16_t r,uint8_t corner, uint16_t colour, uint16_t borde)
{
    int16_t x = 0;
    int16_t y = r;
    int16_t d = 3 - 2 * r;

    while (y >= x) {
        // Dependiendo de la esquina (0=superior izq, 1=superior der, 2=inferior izq, 3=inferior der)
        if (corner == 0) { // sup izq
        	drawPixel(x0 - x, y0 - y, colour);
        	drawPixel(x0 - y, y0 - x, colour);
        }
        else if (corner == 1) { // sup der
        	drawPixel(x0 + x, y0 - y, colour);
        	drawPixel(x0 + y, y0 - x, colour);
        }
        else if (corner == 2) { // inf izq
        	drawPixel(x0 - y, y0 + x, colour);
        	drawPixel(x0 - x, y0 + y, colour);
        }
        else if (corner == 3) { // inf der
        	drawPixel(x0 + x, y0 + y, colour);
        	drawPixel(x0 + y, y0 + x, colour);
        }

        if (d < 0) {
            d += 4 * x + 6;
        } else {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

//rectangulo redondeado relleno
//r = radio de las esquinas
void draw_fill_rectangulo_red(uint16_t x, uint16_t y,uint16_t w, uint16_t h,uint16_t r, uint16_t colour)
{
    // Rectángulo central (sin esquinas)
    draw_fill_rectangle(x + r, y, x + w - r, y + h, colour);

    // Rectángulos laterales
    draw_fill_rectangle(x, y + r, x + r, y + h - r, colour);
    draw_fill_rectangle(x + w - r, y + r, x + w, y + h - r, colour);

    // Esquinas redondeadas
    draw_fill_circulo(x + r,     y + r,     r, colour); // sup izq
    draw_fill_circulo(x + w - r, y + r,     r, colour); // sup der
    draw_fill_circulo(x + r,     y + h - r, r, colour); // inf izq
    draw_fill_circulo(x + w - r, y + h - r, r, colour); // inf der
}
/// rectangulo redondiado con borde o espesor

void draw_rectangulo_red(uint16_t x, uint16_t y,uint16_t w, uint16_t h,uint16_t r, uint16_t colour, uint16_t borde)
{
    // Líneas rectas entre esquinas
    draw_line(x + r, y,         x + w - r, y,         colour, borde); // arriba
    draw_line(x + r, y + h,     x + w - r, y + h,     colour, borde); // abajo
    draw_line(x,     y + r,     x,         y + h - r, colour, borde); // izquierda
    draw_line(x + w, y + r,     x + w,     y + h - r, colour, borde); // derecha

    // Esquinas redondeadas (arcos de 90°)
    draw_arco(x + r,     y + r,     r, 0, colour, borde); // sup izq
    draw_arco(x + w - r, y + r,     r, 1, colour, borde); // sup der
    draw_arco(x + r,     y + h - r, r, 2, colour, borde); // inf izq
    draw_arco(x + w - r, y + h - r, r, 3, colour, borde); // inf der
}

void write16BitColor(uint16_t color)
{
	  uint8_t r = (color & 0xF800) >> 11;
	  uint8_t g = (color & 0x07E0) >> 5;
	  uint8_t b = color & 0x001F;

	  r = (r * 255) / 31;
	  g = (g * 255) / 63;
	  b = (b * 255) / 31;
	  uint8_t data[3] = {r, g, b};
	  ILI9488_SendData_Multi(data, 3);
	  //HAL_SPI_Transmit(&TFT_SPI, (uint8_t *)&r, 1, 10);
	  //HAL_SPI_Transmit(&TFT_SPI, (uint8_t *)&g, 1, 10);
	  //HAL_SPI_Transmit(&TFT_SPI, (uint8_t *)&b, 1, 10);

}
//dibuja elepse
//centro radio en x radio en y color y borde
void draw_elipse(int16_t xc, int16_t yc,int16_t rx, int16_t ry,uint16_t colour, uint16_t borde)
{
    int32_t x, y;
    int32_t rx2 = rx * rx;
    int32_t ry2 = ry * ry;
    int32_t tworx2 = 2 * rx2;
    int32_t twory2 = 2 * ry2;
    int32_t px, py;

    x = 0;
    y = ry;
    px = 0;
    py = tworx2 * y;

    int32_t p = (int32_t)(ry2 - (rx2 * ry) + (0.25 * rx2));

    // Región 1
    while (px < py) {
        for (int dx = -borde/2; dx <= borde/2; dx++) {
            for (int dy = -borde/2; dy <= borde/2; dy++) {
                drawPixel(xc + x + dx, yc + y + dy, colour);
                drawPixel(xc - x + dx, yc + y + dy, colour);
                drawPixel(xc + x + dx, yc - y + dy, colour);
                drawPixel(xc - x + dx, yc - y + dy, colour);
            }
        }

        x++;
        px += twory2;
        if (p < 0) {
            p += ry2 + px;
        } else {
            y--;
            py -= tworx2;
            p += ry2 + px - py;
        }
    }

    // Región 2
    p = (int32_t)(ry2 * (x + 0.5) * (x + 0.5) +
                  rx2 * (y - 1) * (y - 1) - rx2 * ry2);

    while (y >= 0) {
        for (int dx = -borde/2; dx <= borde/2; dx++) {
            for (int dy = -borde/2; dy <= borde/2; dy++) {
                drawPixel(xc + x + dx, yc + y + dy, colour);
                drawPixel(xc - x + dx, yc + y + dy, colour);
                drawPixel(xc + x + dx, yc - y + dy, colour);
                drawPixel(xc - x + dx, yc - y + dy, colour);
            }
        }

        y--;
        py -= tworx2;
        if (p > 0) {
            p += rx2 - py;
        } else {
            x++;
            px += twory2;
            p += rx2 - py + px;
        }
    }
}
//elipse rellena o con fondo
void draw_fill_elipse(int16_t xc, int16_t yc,int16_t rx, int16_t ry,uint16_t colour)
{
    int32_t x, y;
    int32_t rx2 = rx * rx;
    int32_t ry2 = ry * ry;
    int32_t tworx2 = 2 * rx2;
    int32_t twory2 = 2 * ry2;
    int32_t px, py;

    x = 0;
    y = ry;
    px = 0;
    py = tworx2 * y;

    // Región 1
    int32_t p = (int32_t)(ry2 - (rx2 * ry) + (0.25 * rx2));
    while (px < py) {
        draw_fill_rectangle(xc - x, yc + y, xc + x, yc + y, colour);
        draw_fill_rectangle(xc - x, yc - y, xc + x, yc - y, colour);

        x++;
        px += twory2;
        if (p < 0) {
            p += ry2 + px;
        } else {
            y--;
            py -= tworx2;
            p += ry2 + px - py;
        }
    }

    // Región 2
    p = (int32_t)(ry2 * (x + 0.5) * (x + 0.5) +
                  rx2 * (y - 1) * (y - 1) - rx2 * ry2);
    while (y >= 0) {
        draw_fill_rectangle(xc - x, yc + y, xc + x, yc + y, colour);
        draw_fill_rectangle(xc - x, yc - y, xc + x, yc - y, colour);

        y--;
        py -= tworx2;
        if (p > 0) {
            p += rx2 - py;
        } else {
            x++;
            px += twory2;
            p += rx2 - py + px;
        }
    }
}


void draw_bitmap(uint16_t x1, uint16_t  y1, int scale, const unsigned int *bmp)
{
    int width  = bmp[0];   // Ancho
    int height = bmp[1];   // Alto
    unsigned int this_byte;


    int x2 = x1 + (width * scale) - 1;
    int y2 = y1 + (height * scale) - 1;

    // configurar ventana de dibujo en el TFT
    setAddrWindow(x1, y1, x2, y2);

    HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET);

    // recorr0 todos los píxeles
    for (int i = 0; i < height; i++) {
        for (int sv = 0; sv < scale; sv++) {
            for (int j = 0; j < width; j++) {
                this_byte = bmp[2 + (i * width) + j];

                for (int sh = 0; sh < scale; sh++) {
                    write16BitColor(this_byte);
                }
            }
        }
    }

    HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET);
}
