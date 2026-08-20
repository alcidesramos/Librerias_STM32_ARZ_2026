//Por alcides Ramos
// 10/01/2026


#include <ILI9341.h>
#include "stdbool.h"
#include "math.h"



#define TFT_CS_CLR HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET)
#define TFT_CS_SET HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET)
#define TFT_DC_CLR HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_RESET)
#define TFT_DC_SET HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET)
#define TFT_RES_CLR HAL_GPIO_WritePin(TFT_RST_GPIO_Port, TFT_RST_Pin, GPIO_PIN_RESET)
#define TFT_RES_SET HAL_GPIO_WritePin(TFT_RST_GPIO_Port, TFT_RST_Pin, GPIO_PIN_SET)
#define DELAY HAL_Delay

_TFT_dev TFTdev;

void TFT_RESET(void)
{
	TFT_RES_CLR;
	DELAY(100);
	TFT_RES_SET;
	DELAY(50);
}

void SendCommand(uint8_t reg)
{
	TFT_DC_CLR;
	TFT_CS_CLR;
	HAL_SPI_Transmit(&TFT_SPI, &reg, 1, 100);
	TFT_CS_SET;
}

void SendData(uint8_t data)
{
	TFT_DC_SET;
	TFT_CS_CLR;
	HAL_SPI_Transmit(&TFT_SPI, &data, 1, 100);
	TFT_CS_SET;
}

void TFT_ReadData(uint8_t *data, uint16_t length)
{
	TFT_DC_SET;
	TFT_CS_CLR;
	HAL_SPI_Receive(&TFT_SPI, data, length, 100);
	TFT_CS_SET;
}

void WriteReg(uint8_t reg, uint16_t regdata)
{
	SendCommand(reg);
	SendData(regdata);
}

void WriteRAM_Prepare(void)
{
	SendCommand(TFTdev.wramcmd);
}

void write16BitColor(uint16_t Data)
{
	uint8_t buf[2];
	TFT_CS_CLR;
	TFT_DC_SET;
	buf[0] = Data >> 8;
	buf[1] = Data & 0xff;
	HAL_SPI_Transmit(&TFT_SPI, buf, 2, 100);
	TFT_CS_SET;
}

void ILI9341_direction(uint8_t direction)
{
	TFTdev.setxcmd = 0x2A;
	TFTdev.setycmd = 0x2B;
	TFTdev.wramcmd = 0x2C;
	switch (direction)
	{
	case 0:
		TFTdev.width = TFT_W;
		TFTdev.height = TFT_H;
		WriteReg(0x36, (1 << 3) | (0 << 6) | (0 << 7)); // BGR==1,MY==0,MX==0,MV==0
		break;
	case 1:
		TFTdev.width = TFT_H;
		TFTdev.height = TFT_W;
		WriteReg(0x36, (1 << 3) | (0 << 7) | (1 << 6) | (1 << 5)); // BGR==1,MY==1,MX==0,MV==1
		break;
	case 2:
		TFTdev.width = TFT_W;
		TFTdev.height = TFT_H;
		WriteReg(0x36, (1 << 3) | (1 << 6) | (1 << 7)); // BGR==1,MY==0,MX==0,MV==0
		break;
	case 3:
		TFTdev.width = TFT_H;
		TFTdev.height = TFT_W;
		WriteReg(0x36, (1 << 3) | (1 << 7) | (1 << 5)); // BGR==1,MY==1,MX==0,MV==1
		break;
	default:
		break;
	}
}

void  setAddrWindow(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd)
{
	SendCommand(TFTdev.setxcmd);
	SendData(xStar >> 8);
	SendData(0x00FF & xStar);
	SendData(xEnd >> 8);
	SendData(0x00FF & xEnd);

	SendCommand(TFTdev.setycmd);
	SendData(yStar >> 8);
	SendData(0x00FF & yStar);
	SendData(yEnd >> 8);
	SendData(0x00FF & yEnd);

	WriteRAM_Prepare(); // Iniciar escritura en GRAM
}

void ILI9341_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
	 setAddrWindow(Xpos, Ypos, Xpos, Ypos);
}

void ILI9341_Clear(uint16_t Color)
{
	unsigned int i, m;
	uint8_t buf[80];

	for (i = 0; i < 40; i++)
	{
		buf[2 * i] = Color >> 8;
		buf[2 * i + 1] = Color & 0xff;
	}

	 setAddrWindow(0, 0, TFTdev.width - 1, TFTdev.height - 1);
	TFT_CS_CLR;
	TFT_DC_SET;
	for (i = 0; i < TFTdev.height; i++)
	{
		for (m = 0; m < TFTdev.width;)
		{
			m += 40;
			HAL_SPI_Transmit(&TFT_SPI, buf, 80, 100);
		}
	}
	TFT_CS_SET;
}

// Rellenar un área específica con un color
void ILI9341_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{
	uint16_t i, j;
	 setAddrWindow(xsta, ysta, xend - 1, yend - 1); // Establecer rango de visualización
	for (i = ysta; i < yend; i++)
	{
		for (j = xsta; j < xend; j++)
		{
			write16BitColor(color);
		}
	}
}

// Dibujar un punto en una posición específica
void ILI9341_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
	 setAddrWindow(x, y, x, y); // Establecer posición del cursor
	write16BitColor(color);
}




// Dibujar una línea
void ILI9341_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	uint16_t t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;
	delta_x = x2 - x1;
	delta_y = y2 - y1;
	uRow = x1;
	uCol = y1;
	if (delta_x > 0) incx = 1;
	else if (delta_x == 0) incx = 0;
	else { incx = -1; delta_x = -delta_x; }

	if (delta_y > 0) incy = 1;
	else if (delta_y == 0) incy = 0;
	else { incy = -1; delta_y = -delta_y; }

	if (delta_x > delta_y) distance = delta_x;
	else distance = delta_y;

	for (t = 0; t < distance + 1; t++)
	{
		ILI9341_DrawPoint(uRow, uCol, color);
		xerr += delta_x;
		yerr += delta_y;
		if (xerr > distance)
		{
			xerr -= distance;
			uRow += incx;
		}
		if (yerr > distance)
		{
			yerr -= distance;
			uCol += incy;
		}
	}
}

// Dibujar un rectángulo
void ILI9341_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	ILI9341_DrawLine(x1, y1, x2, y1, color);
	ILI9341_DrawLine(x1, y1, x1, y2, color);
	ILI9341_DrawLine(x1, y2, x2, y2, color);
	ILI9341_DrawLine(x2, y1, x2, y2, color);
}

// Dibujar un círculo
void ILI9341_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color)
{
	int a, b;
	a = 0;
	b = r;
	while (a <= b)
	{
		ILI9341_DrawPoint(x0 - b, y0 - a, color);
		ILI9341_DrawPoint(x0 + b, y0 - a, color);
		ILI9341_DrawPoint(x0 - a, y0 + b, color);
		ILI9341_DrawPoint(x0 - a, y0 - b, color);
		ILI9341_DrawPoint(x0 + b, y0 + a, color);
		ILI9341_DrawPoint(x0 + a, y0 - b, color);
		ILI9341_DrawPoint(x0 + a, y0 + b, color);
		ILI9341_DrawPoint(x0 - b, y0 + a, color);
		a++;
		if ((a * a + b * b) > (r * r))
		{
			b--;
		}
	}
}
/*
// Mostrar un solo carácter
void ILI9341_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
	uint8_t temp, sizex, t, m = 0;
	uint16_t i, TypefaceNum;
	uint16_t x0 = x;
	sizex = sizey / 2;
	TypefaceNum = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * sizey;
	num = num - ' ';
	 setAddrWindow(x, y, x + sizex - 1, y + sizey - 1);
	for (i = 0; i < TypefaceNum; i++)
	{
		if (sizey == 12) temp = ascii_1206[num][i];
		else if (sizey == 16) temp = ascii_1608[num][i];
		else if (sizey == 24) temp = ascii_2412[num][i];
		else if (sizey == 32) temp = ascii_3216[num][i];
		else return;

		for (t = 0; t < 8; t++)
		{
			if (!mode) // Modo normal
			{
				if (temp & (0x01 << t)) write16BitColor(fc);
				else write16BitColor(bc);
				m++;
				if (m % sizex == 0)
				{
					m = 0;
					break;
				}
			}
			else // Modo superpuesto
			{
				if (temp & (0x01 << t)) ILI9341_DrawPoint(x, y, fc);
				x++;
				if ((x - x0) == sizex)
				{
					x = x0;
					y++;
					break;
				}
			}
		}
	}
}

// Mostrar una cadena de texto
void ILI9341_ShowString(uint16_t x, uint16_t y, const uint8_t *p, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
	while (*p != '\0')
	{
		ILI9341_ShowChar(x, y, *p, fc, bc, sizey, mode);
		x += sizey / 2;
		p++;
	}
}

// Función auxiliar para potencias
uint32_t mypow(uint8_t m, uint8_t n)
{
	uint32_t result = 1;
	while (n--)
		result *= m;
	return result;
}

// Mostrar una variable de tipo entero
void ILI9341_ShowIntNum(uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey)
{
	uint8_t t, temp;
	uint8_t enshow = 0;
	uint8_t sizex = sizey / 2;
	for (t = 0; t < len; t++)
	{
		temp = (num / mypow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1))
		{
			if (temp == 0)
			{
				ILI9341_ShowChar(x + t * sizex, y, ' ', fc, bc, sizey, 0);
				continue;
			}
			else
				enshow = 1;
		}
		ILI9341_ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
	}
}

// Mostrar una variable flotante con dos decimales
void ILI9341_ShowFloatNum1(uint16_t x, uint16_t y, float num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey)
{
	uint8_t t, temp, sizex;
	uint16_t num1;
	sizex = sizey / 2;
	num1 = num * 100;
	for (t = 0; t < len; t++)
	{
		temp = (num1 / mypow(10, len - t - 1)) % 10;
		if (t == (len - 2))
		{
			ILI9341_ShowChar(x + (len - 2) * sizex, y, '.', fc, bc, sizey, 0);
			t++;
			len += 1;
		}
		ILI9341_ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
	}
}
*/
// Mostrar una imagen desde un arreglo
void ILI9341_ShowPicture(uint16_t x, uint16_t y, uint16_t length, uint16_t width, const unsigned int pic[])
{
	uint16_t i, j;
	uint32_t k = 0;
	 setAddrWindow(x, y, x + length - 1, y + width - 1);
	for (i = 0; i < length; i++)
	{
		for (j = 0; j < width; j++)
		{
			SendData(pic[k * 2]);
			SendData(pic[k * 2 + 1]);
			k++;
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
uint32_t TFT_Get_Id(void)
{
	uint32_t id;
	id = 0;
	uint8_t buf = 0x04;
	TFT_DC_CLR;
	TFT_CS_CLR;
	HAL_SPI_Transmit(&TFT_SPI, &buf, 1, 100);
	TFT_DC_SET;
	HAL_SPI_Receive(&TFT_SPI, (uint8_t *)&id, 3, 100);
	TFT_CS_SET;

	return id;
}

void _st7789_init(void)
{
	// Configuración de Frame rate ST7789S
	SendCommand(0xb2);
	SendData(0x0c);
	SendData(0x0c);
	SendData(0x00);
	SendData(0x33);
	SendData(0x33);
	SendCommand(0xb7);
	SendData(0x35);

	// Configuración de energía ST7789S
	SendCommand(0xbb);
	SendData(0x35);
	SendCommand(0xc0);
	SendData(0x0c);
	SendCommand(0xc2);
	SendData(0x01);
	SendCommand(0xc3);
	SendData(0x13);
	SendCommand(0xc4);
	SendData(0x20);
	SendCommand(0xc6);
	SendData(0x0f);
	SendCommand(0xca);
	SendData(0x0f);
	SendCommand(0xc8);
	SendData(0x08);
	SendCommand(0x55);
	SendData(0x90);
	SendCommand(0xd0);
	SendData(0xa4);
	SendData(0xa1);

	SendCommand(0x36); // Control de acceso a memoria
	SendData(0x00);

	SendCommand(0x3A);
	SendData(0x05);

	// Configuración Gamma ST7789S
	SendCommand(0xe0);
	SendData(0xd0);
	SendData(0x00);
	SendData(0x06);
	SendData(0x09);
	SendData(0x0b);
	SendData(0x2a);
	SendData(0x3c);
	SendData(0x55);
	SendData(0x4b);
	SendData(0x08);
	SendData(0x16);
	SendData(0x14);
	SendData(0x19);
	SendData(0x20);
	SendCommand(0xe1);
	SendData(0xd0);
	SendData(0x00);
	SendData(0x06);
	SendData(0x09);
	SendData(0x0b);
	SendData(0x29);
	SendData(0x36);
	SendData(0x54);
	SendData(0x4b);
	SendData(0x0d);
	SendData(0x16);
	SendData(0x14);
	SendData(0x21);
	SendData(0x20);
	SendCommand(0x29);
}

static void _ili9341_init(void)
{
	// Secuencia de inicialización ILI9341
	SendCommand(0xCF);
	SendData(0x00);
	SendData(0xD9);
	SendData(0X30);

	SendCommand(0xED);
	SendData(0x64);
	SendData(0x03);
	SendData(0X12);
	SendData(0X81);

	SendCommand(0xE8);
	SendData(0x85);
	SendData(0x10);
	SendData(0x78);

	SendCommand(0xCB);
	SendData(0x39);
	SendData(0x2C);
	SendData(0x00);
	SendData(0x34);
	SendData(0x02);

	SendCommand(0xF7);
	SendData(0x20);

	SendCommand(0xEA);
	SendData(0x00);
	SendData(0x00);

	SendCommand(0xC0);  // Control de energía
	SendData(0x21);

	SendCommand(0xC1);
	SendData(0x12);

	SendCommand(0xC5); // Control VCM
	SendData(0x32);
	SendData(0x3C);

	SendCommand(0xC7);
	SendData(0XC1);

	SendCommand(0x36); // Control de acceso a memoria
	SendData(0xA8);

	SendCommand(0x3A);
	SendData(0x55);

	SendCommand(0xB1);
	SendData(0x00);
	SendData(0x18);

	SendCommand(0xB6); // Control de función de pantalla
	SendData(0x0A);
	SendData(0xA2);

	SendCommand(0xF2); // Deshabilitar Gamma 3
	SendData(0x00);

	SendCommand(0x26); // Curva Gamma seleccionada
	SendData(0x01);

	SendCommand(0xE0); // Configurar Gamma
	SendData(0x0F);
	SendData(0x20);
	SendData(0x1E);
	SendData(0x09);
	SendData(0x12);
	SendData(0x0B);
	SendData(0x50);
	SendData(0XBA);
	SendData(0x44);
	SendData(0x09);
	SendData(0x14);
	SendData(0x05);
	SendData(0x23);
	SendData(0x21);
	SendData(0x00);

	SendCommand(0XE1);
	SendData(0x00);
	SendData(0x19);
	SendData(0x19);
	SendData(0x00);
	SendData(0x12);
	SendData(0x07);
	SendData(0x2D);
	SendData(0x28);
	SendData(0x3F);
	SendData(0x02);
	SendData(0x0A);
	SendData(0x08);
	SendData(0x25);
	SendData(0x2D);
	SendData(0x0F);
	SendCommand(0x29);
}
//MIAS   MIS LIBERRIAS
void ILI9341_Init()
{	TFT_RESET();
	// Iniciar secuencia de encendido
	SendCommand(0x11); // Salir de modo sleep
	DELAY(120);
	_ili9341_init();
	ILI9341_direction(USE_HORIZONTAL);
	ILI9341_Clear(BLACK);
}

void drawPixel(int16_t x, int16_t y, uint16_t color)
{
	if ((x < 0) || (x >=TFTdev.width) || (y < 0) || (y >= TFTdev.height))
		return;

	setAddrWindow(x, y, x + 1, y + 1);
	HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin, GPIO_PIN_RESET);

	write16BitColor(color);
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin, GPIO_PIN_SET);

}
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{

	if ((x >= TFTdev.width) || (y >= TFTdev.height))
		return;

	if ((y + h - 1) >= TFTdev.height)
		h = TFTdev.height - y;

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

	if ((x >= TFTdev.width) || (y >= TFTdev.height))
		return;
	if ((x + w - 1) >= TFTdev.width)
		w = TFTdev.width - x;
	setAddrWindow(x, y, x + w - 1, y);
	HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin, GPIO_PIN_RESET);
	while (w--)
	{
		write16BitColor(color);
	}
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin, GPIO_PIN_SET);
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
        	SendData(colour >> 8);
        	SendData(colour & 0xFF);
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
	setAddrWindow(x1, y1, x2, y2);

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

//dibuja elipse
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

void draw_char(uint16_t x_pos, uint16_t y_pos, char ch,uint16_t color, const uint8_t *font_t)
{
    uint8_t ancho      = font_t[0];           // ancho en píxeles
    uint8_t alto_bytes = font_t[1];           // alto en bytes
    uint8_t alto_px    = alto_bytes * 8;      // alto en píxeles
    uint16_t columnas  = ancho * alto_bytes + 1;
    uint16_t posi      = (ch - 32) * columnas + 3;

    // chequeo de límites básicos
    if ((x_pos >=  TFTdev.width) || (y_pos >=  TFTdev.height)) return;
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
    if ((x_pos >=  TFTdev.width) || (y_pos >=  TFTdev.height)) return;
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
    //uint8_t alto_bytes = font_t[1];
  //  uint8_t alto_px = alto_bytes * 8;

    while(*str)
    {
        // Dibuja cada caracter en la posición actual
        draw_char_fondo(x_pos, y_pos, *str, color, bg_color, font_t);

        // Avanza en X para el siguiente caracter
        x_pos += ancho;

        str++;
    }
}
