#include "SSD1306.h"

uint8_t 	DAT=0x40;
uint8_t    CMD=0x00;

unsigned char columnas,columnas_med;
unsigned char buffer[buffer_size];

void OLED_init()
{
   
   #ifdef   reset
   RST_direction=0; // salida pin de reset;
#endif



 //   I2C__Init();
    HAL_Delay(100);
    OLED_clear_buffer();
    OLED_reset_sequence();
   //  OLED_write(0xAF, CMD);
     //OLED_write(0xA4, CMD);


     OLED_write(0X3C, CMD);


    OLED_write((Set_Display_ON_or_OFF_CMD + Display_OFF) , CMD);

    OLED_write(Set_Display_Clock_CMD, CMD);
    OLED_write(0x80, CMD);

    OLED_write(Set_Multiplex_Ratio_CMD, CMD);
    OLED_write(0x3F, CMD);

    OLED_write(Set_Display_Offset_CMD, CMD);
    OLED_write(0x00, CMD);

    OLED_write((Set_Display_Start_Line_CMD | 0x00), CMD);

    OLED_write(Set_Charge_Pump_CMD, CMD);
    OLED_write((Set_Higher_Column_Start_Address_CMD | Enable_Charge_Pump), CMD);

    OLED_write(Set_Memory_Addressing_Mode_CMD, CMD);
    OLED_write(Page_Addressing_Mode, CMD);

    OLED_write((Set_Segment_Remap_CMD | Column_Address_0_Mapped_to_SEG127), CMD);

    OLED_write((Set_COM_Output_Scan_Direction_CMD | Scan_from_COM63_to_0), CMD);

    OLED_write(Set_Common_HW_Config_CMD, CMD);
    OLED_write(0x12, CMD);

    OLED_write(Set_Contrast_Control_CMD, CMD);
    OLED_write(0xCF, CMD);

    OLED_write(Set_Pre_charge_Period_CMD, CMD);
    OLED_write(0xF1, CMD);

    OLED_write(Set_VCOMH_Level_CMD, CMD);
    OLED_write(0x40, CMD);

    OLED_write((Set_Entire_Display_ON_CMD | Normal_Display), CMD);

    OLED_write((Set_Normal_or_Inverse_Display_CMD | Non_Inverted_Display), CMD);

    OLED_write((Set_Display_ON_or_OFF_CMD | Display_ON) , CMD);


    OLED_gotoxy(0, 0);

    OLED_clear_screen();
    OLED_clear_buffer();
}


void OLED_reset_sequence()
{
   #ifdef   reset
       RST = 0;
    delay_us(40);
    RST = 1;
   delay_us(4);

#endif

}

void OLED_write(unsigned char value, unsigned char control_byte)
{    
	uint8_t dt[2];
	dt[0] =  control_byte;
	dt[1] = value;
	HAL_I2C_Master_Transmit(&SSD1306_I2C,SSD1306_I2C_Address , &dt, 2, 100);
}


void OLED_gotoxy(unsigned char x_pos, unsigned char y_pos)
{
    OLED_write((Set_Page_Start_Address_CMD + y_pos), CMD);
     OLED_write(((x_pos & 0x0F) | Set_Lower_Column_Start_Address_CMD), CMD);
     OLED_write((((x_pos & 0xF0) >> 0x04) | Set_Higher_Column_Start_Address_CMD), CMD);

}

//  0   limpia  255 rellena
void OLED_fill(unsigned char bmp_data)
{
    unsigned char x_pos = 0x01;
    unsigned char page = 0x00;
  buffer[0]=DAT;



 for(page = 0; page < y_max; page++)
    {
        OLED_gotoxy(x_min, page);

        buffer[0]=DAT;

        for(x_pos = x_min+1; x_pos < x_max+1; x_pos++)
        {
        	buffer[x_pos]=bmp_data;
        }

      HAL_I2C_Master_Transmit(&SSD1306_I2C,SSD1306_I2C_Address , buffer,x_max +1, 300);

    }
}






void OLED_clear_buffer()
{
     unsigned int s ;

     for(s = 0; s < buffer_size; s++)
     {
          buffer[s] = 0x00;
     }
}

void OLED_set_buffer()
{
     unsigned int s ;

     for(s = 0; s < buffer_size; s++)
     {
          buffer[s] = 0xff;
     }
}

void OLED_clear_screen()
{
	 OLED_clear_buffer();
	  OLED_UpdateScreen();
}

void OLED_set_screen()
{
	 OLED_set_buffer();
	  OLED_UpdateScreen();
}

void OLED_cursor(unsigned char x_pos, unsigned char y_pos)
{
    unsigned char s = 0x00;

    if(y_pos != 0x00)
    {
        if(x_pos == 1)
        {
            OLED_gotoxy(0x00, (y_pos + 0x02));
        }
        else
        {
            OLED_gotoxy((0x50 + ((x_pos - 0x02) * columnas)), (y_pos + 0x02));
        }

        for(s = 0x00; s < columnas; s++)
        {
            OLED_write(0xFF, DAT);
        }
    }
}
void OLED_manda_linea(uint8_t* buf_dato, uint16_t count)
{
	uint8_t dat_temp[256];
	dat_temp[0] = DAT;
	uint8_t i;
	for(i = 0; i < count; i++)	dat_temp[i+1] = buf_dato[i];
	HAL_I2C_Master_Transmit(&SSD1306_I2C,SSD1306_I2C_Address, dat_temp, count+1, 10);

}



void OLED_UpdateScreen()
{

	uint8_t fil;

	for (fil = 0; fil< 8; fil++)
	{
        OLED_write(0xB0+fil,CMD);
        OLED_write(0x00,CMD);
        OLED_write(0x10,CMD);

        ///DEBE ESCRIBIR
          uint16_t i;
        for(i = 0; i < x_max; i++) buffer[i+1] = buffer[i];
        //manda linea
        OLED_manda_linea(&buffer[x_max * fil], x_max);
      }
}



void OLED_DrawPoint(uint16_t x, uint16_t y,  unsigned char pixel)
{

	//buffer[x + (y / 8) * x_size] |= 1 << (y % 8);

	//if (pixel==ON)
//buffer[x + (y / 8) * x_size]=~buffer[x + (y / 8) * x_max];
	if (pixel == OFF)
	{
		buffer[x + (y / 8) * x_size] |= 1 << (y % 8);
	}

	else
	{
		buffer[x + (y / 8) * x_size] &= ~(1 << (y % 8));
     }
}

void OLED_DrawBitmap(int16_t x, int16_t y,int16_t w, int16_t h, const unsigned char* bitmap, unsigned char pixel,unsigned char clean)
{

    int16_t byteWidth = (w + 7) / 8;
    uint8_t byte = 0;
    pixel=!pixel;

   if (clean==SI)
   {

   if (pixel==OFF) OLED_clear_buffer();
   else OLED_set_buffer();

   }
   for(int16_t j=0; j<h; j++, y++)
    {
        for(int16_t i=0; i<w; i++)
        {
            if(i & 7)
            {
               byte <<= 1;
            }
            else
            {
               byte = (*(const unsigned char *)(&bitmap[j * byteWidth + i / 8]));
            }
            if(byte & 0x80) OLED_DrawPoint(x+i, y, pixel);
        }
    }
    OLED_UpdateScreen();
   }



void OLED_print_char(unsigned char x_pos, unsigned char y_pos, unsigned char ch,const unsigned char *font_t)
{
    unsigned char chr = 0x00;
    unsigned long s;
    unsigned char s1;
     unsigned long  posi;
    unsigned char page_colum;
    
    columnas=font_t[0] *font_t[1] +1;
    columnas_med=font_t[0];
    page_colum=font_t[1] ;

    
    
    
    
    chr = (ch - 32);   // reste offset de asccis



   if(x_pos > (x_max - columnas_med))
    {
        x_pos = 0x00;
        y_pos++;
    }


    if(y_pos > (y_max - page_colum))
    {
        x_pos = 0x00;
        y_pos=0;
    }
           posi=chr*columnas+3;   // calcula la ubicaci�n de inicio del caracter deseado

                 x_pos--;
           for(s = 0; s < columnas_med; s++)    // for para las filas
           {   
                 x_pos++;
                for (s1=y_pos;s1<y_pos+page_colum;s1++)   // for por pagina del oles en el eje y
                {
                   OLED_gotoxy(x_pos,s1);
                   OLED_write(font_t[posi++], DAT);
                }
           }
   }

void OLED_print_char_inv(unsigned char x_pos, unsigned char y_pos, unsigned char ch,const unsigned char *font_t)
{
    unsigned char chr = 0x00;
    unsigned long s;
    unsigned char s1;
     unsigned long  posi;
    unsigned char page_colum;

    columnas=font_t[0] *font_t[1] +1;
    columnas_med=font_t[0];
    page_colum=font_t[1] ;





    chr = (ch - 32);   // reste offset de asccis



   if(x_pos > (x_max - columnas_med))
    {
        x_pos = 0x00;
        y_pos++;
    }


    if(y_pos > (y_max - page_colum))
    {
        x_pos = 0x00;
        y_pos=0;
    }
           posi=chr*columnas+3;   // calcula la ubicaci�n de inicio del caracter deseado

                 x_pos--;
           for(s = 0; s < columnas_med; s++)    // for para las filas
           {
                 x_pos++;
                for (s1=y_pos;s1<y_pos+page_colum;s1++)   // for por pagina del oles en el eje y
                {
                   OLED_gotoxy(x_pos,s1);
                   OLED_write(~font_t[posi++], DAT);
                }
           }
   }


void OLED_print_text_inv(unsigned char x_pos, unsigned char y_pos, unsigned char *ch,const unsigned char *font_t)
{
    unsigned char s = 0x00;

    do
    {
        OLED_print_char_inv(x_pos, y_pos, ch[s++],font_t);
        x_pos += columnas_med;
     }while((ch[s] >= 0x20) && (ch[s] <= 0x7F));
}

void OLED_print_text(unsigned char x_pos, unsigned char y_pos, unsigned char *ch,const unsigned char *font_t)
{
    unsigned char s = 0x00;

    do
    {
        OLED_print_char(x_pos, y_pos, ch[s++],font_t);
        x_pos += columnas_med;
     }while((ch[s] >= 0x20) && (ch[s] <= 0x7F));
}



void  OLED_Draw_Pixel(unsigned char x_pos, unsigned char y_pos, unsigned char colour)
{
    unsigned char value = 0x00;
    unsigned char page = 0x00;
    unsigned char bit_pos = 0x00;

    page = (y_pos / y_max);
    bit_pos = (y_pos - (page * y_max));
    value = buffer[((page * x_max) + x_pos)];

    if((colour & YES) != NO)
    {
        value |= (1 << bit_pos);
    }
    else
    {
        value &= (~(1 << bit_pos));
    }

    buffer[((page * x_max) + x_pos)] = value;
    OLED_gotoxy(x_pos, page);
    OLED_write(value, DAT);
}


void OLED_Draw_Line(signed int x1, signed int y1, signed int x2, signed int y2, unsigned char colour)
{
    signed int dx = 0x0000;
    signed int dy = 0x0000;
    signed int stepx = 0x0000;
    signed int stepy = 0x0000;
    signed int fraction = 0x0000;

     dy = (y2 - y1);
    dx = (x2 - x1);

    if (dy < 0)
    {
        dy = -dy;
        stepy = -1;
    }
    else
    {
        stepy = 1;
    }

    if (dx < 0)
    {
        dx = -dx;
        stepx = -1;
    }
    else
    {
        stepx = 1;
    }

    dx <<= 1;
    dy <<= 1;

    OLED_Draw_Pixel(x1, y1, colour);

    if(dx > dy)
    {
        fraction = (dy - (dx >> 1));
        while (x1 != x2)
        {
            if(fraction >= 0)
            {
                y1 += stepy;
                fraction -= dx;
            }

            x1 += stepx;
            fraction += dy;

            OLED_Draw_Pixel(x1, y1, colour);
        }
    }
    else
    {
        fraction = (dx - (dy >> 1));
        while (y1 != y2)
        {
            if (fraction >= 0)
            {
                x1 += stepx;
                fraction -= dy;
            }

            y1 += stepy;
            fraction += dx;

            OLED_Draw_Pixel(x1, y1, colour);
        }
    }
}

void OLED_Draw_Line1(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, unsigned char c)
{
	OLED_clear_buffer();
	int16_t dx, dy, sx, sy, err, e2, i, tmp;

	/* Check for overflow */
	if (x0 >= x_max) {
		x0 = x_max - 1;
	}
	if (x1 >= x_max) {
		x1 = x_max - 1;
	}
	if (y0 >= y_size) {
		y0 = y_size - 1;
	}
	if (y1 >= y_size) {
		y1 = y_size - 1;
	}

	dx = (x0 < x1) ? (x1 - x0) : (x0 - x1);
	dy = (y0 < y1) ? (y1 - y0) : (y0 - y1);
	sx = (x0 < x1) ? 1 : -1;
	sy = (y0 < y1) ? 1 : -1;
	err = ((dx > dy) ? dx : -dy) / 2;

	if (dx == 0) {
		if (y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}

		if (x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}

		/* Vertical line */
		for (i = y0; i <= y1; i++) {
			OLED_Draw_Pixel(x0, i, c);
		}

		/* Return from function */
		return;
	}

	if (dy == 0) {
		if (y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}

		if (x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}

		/* Horizontal line */
		for (i = x0; i <= x1; i++) {
			OLED_Draw_Pixel(i, y0, c);
		}

		/* Return from function */
		return;
	}

	while (1) {
		OLED_Draw_Pixel(x0, y0, c);
		if (x0 == x1 && y0 == y1) {
			break;
		}
		e2 = err;
		if (e2 > -dx) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dy) {
			err += dx;
			y0 += sy;
		}
	}
}
void  OLED_Draw_Rectangle(signed int x1, signed int y1, signed int x2, signed int y2, unsigned char fill, unsigned char colour, unsigned char type)
{
     unsigned short i = 0x00;
     unsigned short xmin = 0x00;
     unsigned short xmax = 0x00;
     unsigned short ymin = 0x00;
     unsigned short ymax = 0x00;


  if(fill != NO)
     {
        if(x1 < x2)
        {
           xmin = x1;
           xmax = x2;
        }
        else
        {
           xmin = x2;
           xmax = x1;
        }

        if(y1 < y2)
        {
           ymin = y1;
           ymax = y2;
        }
        else
        {
           ymin = y2;
           ymax = y1;
        }

        for(; xmin <= xmax; ++xmin)
        {
           for(i = ymin; i <= ymax; ++i)
           {
              OLED_Draw_Pixel(xmin, i, colour);
           }
         }
     }

     else
     {
        OLED_Draw_Line(x1, y1, x2, y1, colour);
        OLED_Draw_Line(x1, y2, x2, y2, colour);
        OLED_Draw_Line(x1, y1, x1, y2, colour);
        OLED_Draw_Line(x2, y1, x2, y2, colour);
     }

     if(type != SQUARE)
     {
        OLED_Draw_Pixel(x1, y1, ~colour);
        OLED_Draw_Pixel(x1, y2, ~colour);
        OLED_Draw_Pixel(x2, y1, ~colour);
        OLED_Draw_Pixel(x2, y2, ~colour);
     }
}

void OLED_Draw_Triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint8_t color)
{

	OLED_Draw_Line(x1, y1, x2, y2, color);
	OLED_Draw_Line(x2, y2, x3, y3, color);
	OLED_Draw_Line(x3, y3, x1, y1, color);
}

void  OLED_Draw_Circle(signed int xc, signed int yc, signed int radius, unsigned char fill, unsigned char colour)
{
   signed int a = 0x0000;
   signed int b = 0x0000;
   signed int P = 0x0000;

   b = radius;
   P = (1 - b);
   do
   {
        if(fill != NO)
        {
           OLED_Draw_Line((xc - a), (yc + b), (xc + a), (yc + b), colour);
           OLED_Draw_Line((xc - a), (yc - b), (xc + a), (yc - b), colour);
           OLED_Draw_Line((xc - b), (yc + a), (xc + b), (yc + a), colour);
           OLED_Draw_Line((xc - b), (yc - a), (xc + b), (yc - a), colour);
        }
        else
        {
          OLED_Draw_Pixel((xc + a), (yc + b), colour);
          OLED_Draw_Pixel((xc + b), (yc + a), colour);
          OLED_Draw_Pixel((xc - a), (yc + b), colour);
          OLED_Draw_Pixel((xc - b), (yc + a), colour);
          OLED_Draw_Pixel((xc + b), (yc - a), colour);
          OLED_Draw_Pixel((xc + a), (yc - b), colour);
          OLED_Draw_Pixel((xc - a), (yc - b), colour);
          OLED_Draw_Pixel((xc - b), (yc - a), colour);
        }

        if(P < 0)
        {
           P += (3 + (2 * a++));
        }
        else
        {
           P += (5 + (2 * ((a++) - (b--))));
        }
    }while(a <= b);
}

void  OLED_scroll_der(unsigned char inicio, unsigned char fin)
{
     OLED_write(RIGHT_HORIZONTAL_SCROLL, CMD);
     OLED_write(00, CMD);
     OLED_write(inicio, CMD);
     OLED_write(0, CMD);
     OLED_write(fin, CMD);
     OLED_write(0, CMD);
     OLED_write(0xff, CMD);
     OLED_write(ACTIVATE_SCROLL, CMD);

}

void  OLED_scroll_izq(unsigned char inicio, unsigned char fin)
{
     OLED_write(LEFT_HORIZONTAL_SCROLL, CMD);
     OLED_write(00, CMD);
     OLED_write(inicio, CMD);
     OLED_write(0, CMD);
     OLED_write(fin, CMD);
     OLED_write(0, CMD);
     OLED_write(0xff, CMD);
     OLED_write(ACTIVATE_SCROLL, CMD);

}

void  OLED_scroll_diag_der(unsigned char inicio, unsigned char fin)
{
     OLED_write(SET_VERTICAL_SCROLL_AREA, CMD);
     OLED_write(0x0, CMD);
     OLED_write(y_size, CMD);
     OLED_write(VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL, CMD);
     OLED_write(0x0, CMD);
     OLED_write(inicio, CMD);
     OLED_write(0x0, CMD);
     OLED_write(fin, CMD);
     OLED_write(0x1, CMD);
     OLED_write(ACTIVATE_SCROLL, CMD);
}


void  OLED_scroll_diag_izq(unsigned char inicio, unsigned char fin)
{
     OLED_write(SET_VERTICAL_SCROLL_AREA, CMD);
     OLED_write(0x0, CMD);
     OLED_write(y_size, CMD);
     OLED_write(VERTICAL_AND_LEFT_HORIZONTAL_SCROLL, CMD);
     OLED_write(0x0, CMD);
     OLED_write(inicio, CMD);
     OLED_write(0x0, CMD);
     OLED_write(fin, CMD);
     OLED_write(0x1, CMD);
     OLED_write(ACTIVATE_SCROLL, CMD);

}

void  OLED_stop_scroll()
{
     OLED_write(DEACTIVATE_SCROLL, CMD);

}


