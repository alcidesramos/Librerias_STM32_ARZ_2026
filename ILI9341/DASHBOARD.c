/*
 * DASHBOARD.c
 *
 *  Created on: Sep 21, 2025
 *      Author: ALCIDES_RAMOS
 */
#include "dashboard.h"
#include "ili9341.h"

//prueba onda seno

float data_seno[200] = {
    50.00, 54.67, 59.29, 63.81, 68.17, 72.33, 76.24, 79.85, 83.11, 86.00,
    88.48, 90.51, 92.08, 93.18, 93.77, 93.87, 93.47, 92.56, 91.17, 89.32,
    87.03, 84.35, 81.31, 77.95, 74.33, 70.49, 66.50, 62.40, 58.26, 54.12,
    50.00, 45.88, 41.74, 37.60, 33.50, 29.51, 25.67, 22.05, 18.69, 15.65,
    12.97, 10.68, 8.83, 7.44, 6.53, 6.13, 6.23, 6.82, 7.92, 9.49,
    11.52, 14.00, 16.89, 20.15, 23.76, 27.67, 31.83, 36.19, 40.71, 45.33,
    50.00, 54.67, 59.29, 63.81, 68.17, 72.33, 76.24, 79.85, 83.11, 86.00,
    88.48, 90.51, 92.08, 93.18, 93.77, 93.87, 93.47, 92.56, 91.17, 89.32,
    87.03, 84.35, 81.31, 77.95, 74.33, 70.49, 66.50, 62.40, 58.26, 54.12,
    50.00, 45.88, 41.74, 37.60, 33.50, 29.51, 25.67, 22.05, 18.69, 15.65,
    12.97, 10.68, 8.83, 7.44, 6.53, 6.13, 6.23, 6.82, 7.92, 9.49,
    11.52, 14.00, 16.89, 20.15, 23.76, 27.67, 31.83, 36.19, 40.71, 45.33,
    50.00, 54.67, 59.29, 63.81, 68.17, 72.33, 76.24, 79.85, 83.11, 86.00,
    88.48, 90.51, 92.08, 93.18, 93.77, 93.87, 93.47, 92.56, 91.17, 89.32,
    87.03, 84.35, 81.31, 77.95, 74.33, 70.49, 66.50, 62.40, 58.26, 54.12,
    50.00, 45.88, 41.74, 37.60, 33.50, 29.51, 25.67, 22.05, 18.69, 15.65,
    12.97, 10.68, 8.83, 7.44, 6.53, 6.13, 6.23, 6.82, 7.92, 9.49,
    11.52, 14.00, 16.89, 20.15, 23.76, 27.67, 31.83, 36.19, 40.71, 45.33,
    50.00, 54.67, 59.29, 63.81, 68.17, 72.33, 76.24, 79.85, 83.11, 86.00,
    88.48, 90.51, 92.08, 93.18, 93.77, 93.87, 93.47, 92.56, 91.17, 89.32
};
//gauge a usar
Gauge_ARZ gauge1 = {
//centro
    .x = 230,
    .y = 120,
    .r = 80,//radio
    .w = 12,//ancho
    .vmin = 0,//valor minimo
    .vmax = 100,//valor maximo
    .value = 0, //valor inicial variable
    .Ang_ini = -225, //angulo inicial
    .Ang_fin = 45,//angulo final;
    .Res_ang = 5, //pasos

	//fondo arco
	.fondo = COLOR_WHITE,

	//DEGRaDADO
	.colorIni = COLOR_GREEN, //color inicio
    .colorMed   = COLOR_YELLOW,//color centro
    .colorFin   = COLOR_RED,//color final;
	//del texto
    .fondo    = COLOR_DARKGRAY, //color de fondo
    .showValue  = true, //muestra el valor
    .textColor  = COLOR_WHITE, //color del texto
    .font       = font2   // font a usar
};

//define los led a usar
Led led1=
{
.x=50,
.y=180,
.color_off=COLOR_GREY,
.color_on=COLOR_RED,
.r=20,
};


Led led2=
{
.x=40,
.y=80,
.color_off=COLOR_WHITE,
.color_on=COLOR_GREEN,
.r=30,
};


//barras de progersop horizontales
Barra barrah1=
{
.x = 0,
.y = 0,
.w = 150,
.h = 20,
.vmin = 0,
.vmax = 50,
.value = 0,
.colorFondo = COLOR_LIGHTGRAY,
.colorFill  = COLOR_BLUE,
.colorMarco = COLOR_BLACK,
.showValue = 1,
.textColor = COLOR_BLACK,
.text_fondo = COLOR_WHITE,
.font = font1,
.numTicks = 5  // # de rayitas
};

//barras de progreso verticales
Barra barrav1=
{
.x = 110,
.y = 100,
.w = 30,
.h = 100,
.vmin = 0,
.vmax = 50,
.value = 0,
.colorFondo = COLOR_LIGHTGRAY,
.colorFill  = COLOR_RED,
.colorMarco = COLOR_BLACK,
.showValue = 1,
.textColor = COLOR_BLACK,
.text_fondo = COLOR_WHITE,
.font = font1,
.numTicks = 5  // # de rayitas
};

Grafica grafica1=
{
.x =0, .y = 20,
.w = 200, .h = 150,//   /.w  es nuemro dd datos a gtraficar
.vmin = 0, .vmax = 100,
.colorFondo = COLOR_BLACK,
.colorMarco = COLOR_WHITE,
.colorGrid = COLOR_DARKGRAY,
.colorLinea = COLOR_GREEN,
.numTicksY = 4
};


Grafica grafica2=
{
.x =0, .y = 20,
.w = 200, .h = 150,//   /.w  es nuemro dd datos a gtraficar
.vmin = 0, .vmax = 3.3,
.colorFondo = COLOR_BLACK,
.colorMarco = COLOR_WHITE,
.colorGrid = COLOR_DARKGRAY,
.colorLinea = COLOR_GREEN,
.numTicksY = 2
};


//gradiente
uint16_t rainbow(uint16_t value)
{
  // Value is expected to be in range 0-127
  // The value is converted to a spectrum colour from 0 = blue through to 127 = red

	uint16_t red = 0; // Red is the top 5 bits of a 16 bit colour value
	uint16_t green = 0;// Green is the middle 6 bits
	uint16_t blue = 0; // Blue is the bottom 5 bits

	uint16_t quadrant = value / 32;

  if (quadrant == 0) {
    blue = 31;
    green = 2 * (value % 32);
    red = 0;
  }
  if (quadrant == 1) {
    blue = 31 - (value % 32);
    green = 63;
    red = 0;
  }
  if (quadrant == 2) {
    blue = 0;
    green = 63;
    red = value % 32;
  }
  if (quadrant == 3) {
    blue = 0;
    green = 63 - 2 * (value % 32);
    red = 31;
  }
  return (red << 11) + (green << 5) + blue;
}



// Función para interpolar color (RGB565)
static uint16_t colorInterpolate(uint16_t c1, uint16_t c2, float t) {
    uint8_t r1 = (c1 >> 11) & 0x1F;
    uint8_t g1 = (c1 >> 5) & 0x3F;
    uint8_t b1 = c1 & 0x1F;

    uint8_t r2 = (c2 >> 11) & 0x1F;
    uint8_t g2 = (c2 >> 5) & 0x3F;
    uint8_t b2 = c2 & 0x1F;

    uint8_t r = r1 + (r2 - r1) * t;
    uint8_t g = g1 + (g2 - g1) * t;
    uint8_t b = b1 + (b2 - b1) * t;

    return (r << 11) | (g << 5) | b;
}

void gauge_draw_arc(Gauge_ARZ *g)
{
    int angleRange = g->Ang_fin - g->Ang_ini;
    int vAngle = g->Ang_ini + (g->value - g->vmin) * angleRange / (g->vmax - g->vmin);

    // --- Fondo del arco ---
    for (int i = g->Ang_ini; i < g->Ang_fin; i += g->Res_ang) {
        float rad1 = i * 0.0174533;
        float rad2 = (i + g->Res_ang) * 0.0174533;

        int x0 = g->x + cos(rad1) * (g->r - g->w);
        int y0 = g->y + sin(rad1) * (g->r - g->w);
        int x1 = g->x + cos(rad1) * g->r;
        int y1 = g->y + sin(rad1) * g->r;
        int x2 = g->x + cos(rad2) * (g->r - g->w);
        int y2 = g->y + sin(rad2) * (g->r - g->w);
        int x3 = g->x + cos(rad2) * g->r;
        int y3 = g->y + sin(rad2) * g->r;

        draw_fill_triangulo(x0, y0, x1, y1, x2, y2, g->colorBase);
        draw_fill_triangulo(x1, y1, x2, y2, x3, y3, g->colorBase);
    }

    // --- Arco de progreso (degradado) ---
    for (int i = g->Ang_ini; i < vAngle; i += g->Res_ang) {
        float t = (float)(i - g->Ang_ini) / angleRange; // 0..1
        uint16_t c;
        if (t < 0.5f) {
            c = colorInterpolate(g->colorIni, g->colorMed, t * 2);
        } else {
            c = colorInterpolate(g->colorMed, g->colorFin, (t - 0.5f) * 2);
        }

        float rad1 = i * 0.0174533;
        float rad2 = (i + g->Res_ang) * 0.0174533;

        int x0 = g->x + cos(rad1) * (g->r - g->w);
        int y0 = g->y + sin(rad1) * (g->r - g->w);
        int x1 = g->x + cos(rad1) * g->r;
        int y1 = g->y + sin(rad1) * g->r;
        int x2 = g->x + cos(rad2) * (g->r - g->w);
        int y2 = g->y + sin(rad2) * (g->r - g->w);
        int x3 = g->x + cos(rad2) * g->r;
        int y3 = g->y + sin(rad2) * g->r;

        draw_fill_triangulo(x0, y0, x1, y1, x2, y2, c);
        draw_fill_triangulo(x1, y1, x2, y2, x3, y3, c);
    }

    // --- Valor en el centro ---
    if (g->showValue) {
        char buf[16];
        sprintf(buf, "%3d%", g->value);

        draw_string_fondo(
            g->x - (strlen(buf) * g->font[0]) / 2,
            g->y - (g->font[1] * 4),
            buf,
            g->textColor,
            g->fondo,     // <-- este es el color de fondo del texto
            g->font
        );
    }
}
void draw_degree_symbol(int x, int y, uint16_t color)
{
    draw_fill_circulo(x, y, 2, color); // circulo grados
}

// termometor
//Dibuja un led
void led_draw1(Led *led)
{
    uint16_t color = (led->estado) ? led->color_on : led->color_off;

    // Círculo principal base
    draw_fill_circulo(led->x, led->y, led->r, color);

    // Borde negro
    draw_circulo(led->x, led->y, led->r, 0x0000, 2);

    // Efecto de brillo radial (simulación)
    int16_t steps = 4; // cantidad de círculos concéntricos
    for (int i = 0; i < steps; i++) {
        int16_t radius = led->r - (i * led->r / steps);
        uint16_t brightColor;

        // Interpolamos hacia blanco para el efecto de luz
        if (led->estado) {
            // Mezcla simple: más blanco en el centro superior izquierdo
            brightColor = ((i * 0xFFFF) / steps) | (color & 0x7BEF);
        } else {
            // LED apagado: círculos más oscuros
            brightColor = ((i * 0x4208) / steps) | (color & 0x3186);
        }

        draw_fill_circulo(
            led->x - i/2, // desplazamiento hacia arriba-izquierda
            led->y - i/2,
            radius,
            brightColor
        );
    }

    // Sombra muy sutil inferior derecha
    int16_t sh_r = led->r / 6;
    draw_fill_circulo(
        led->x + sh_r,
        led->y + sh_r,
        sh_r,
        0x8410
    );
}
void led_draw(Led *led)
{
    uint16_t color = (led->estado) ? led->color_on : led->color_off;

    // Círculo principal
    draw_fill_circulo(led->x, led->y, led->r, color);

    // Borde
    draw_circulo(led->x, led->y, led->r, 0x0000, 2); // borde negro de 2 px

    // Efecto de brillo  divide el radio en un factor para brillo
    //  círculo más pequeño en la parte superior izquierda simulando reflejo de luz
    int16_t br_r = led->r / 4;//factor radio del circulo blanco
    draw_fill_circulo(led->x - br_r/2, led->y - br_r/2, br_r, 0xFFFF); // blanco

    // Sombra inferior derecha (más pequeña para profundidad sutil)
    draw_fill_circulo(led->x + br_r/4, led->y + br_r/4, br_r/6, 0x8410); // gris oscuro
}

void barrah_draw(Barra *p)
 {
    // Borde
    draw_rectangle(
        p->x, p->y,
        p->x + p->w, p->y + p->h,
        p->colorMarco, 1
    );

    // Fondo
    draw_fill_rectangle(
        p->x + 1, p->y + 1,
        p->x + p->w - 1, p->y + p->h - 1,
        p->colorFondo
    );

    //Nivel según valor
    float rango = p->vmax - p->vmin;
    if (rango <= 0) rango = 1;

    int ancho = (p->value - p->vmin) * p->w / rango;
    if (ancho > 0) {
        draw_fill_rectangle(
            p->x + 1, p->y + 1,
            p->x + ancho - 1, p->y + p->h - 1,
            p->colorFill
        );
    }

    // Rayitas y números
    for (int i = 0; i <= p->numTicks; i++) {
        float valorTick = p->vmin + i * rango / p->numTicks;
        int tickX = p->x + i * p->w / p->numTicks;
        int tickY = p->y + p->h;
        char buf[8];
        sprintf(buf, "%3d", (uint16_t)valorTick);

        // Rayita
        draw_line(tickX, tickY, tickX, tickY + 5, p->colorMarco, 1);

        // Número
        int textX = tickX - (strlen(buf) * p->font[0]) / 2;
        int textY = tickY + 6;
        draw_string_fondo(textX, textY, buf, p->textColor, p->text_fondo, p->font);
    }

    //Valor actual
    if (p->showValue) {
        char buf[8];
        sprintf(buf, "%5.1f", p->value);

        int textX = p->x + (p->w / 2) - (strlen(buf) * p->font[0]) / 2;
        int textY = p->y + p->h + 20;
        draw_string_fondo(textX, textY, buf, p->textColor, p->text_fondo, p->font);
    }
}
//barra vertical texto debajo de la barra
void barrav_draw(Barra *p)
{
    // Borde
    draw_rectangle(
        p->x, p->y,
        p->x + p->w, p->y + p->h,
        p->colorMarco, 1
    );

    // Fondo
    draw_fill_rectangle(
        p->x + 1, p->y + 1,
        p->x + p->w - 1, p->y + p->h - 1,
        p->colorFondo
    );

    // Nivel según valor
    float rango = p->vmax - p->vmin;
    if (rango <= 0) rango = 1;

    int alto = (p->value - p->vmin) * p->h / rango;
    if (alto > 0) {
        draw_fill_rectangle(
            p->x + 1, p->y + p->h - alto,
            p->x + p->w - 1, p->y + p->h - 1,
            p->colorFill
        );
    }

    // Rayitas y números según numTicks
    int numTicks = (p->numTicks > 0) ? p->numTicks : 10; // por si no se pone
    for (int i = 0; i <= numTicks; i++)
    {
        int yTick = p->y + p->h - (i * p->h / numTicks);

        // Rayita
        draw_line(p->x - 4, yTick, p->x, yTick, p->colorMarco, 1);

        // Valor del tick
        int valorTick = p->vmin + (i * rango / numTicks);
        char buf[8];
        sprintf(buf, "%d", valorTick);

        // Número al lado de la barra
        int textX = p->x - 4 - strlen(buf) * p->font[0] - 2;
        int textY = yTick - (p->font[1] * 4) / 2;
        draw_string_fondo(textX, textY, buf, p->textColor, p->text_fondo, p->font);
    }

    // Valor actual debajo de la barra
    if (p->showValue)
    {
        char buf[8];
        sprintf(buf, "%5.1f", p->value);

        int textX = p->x + (p->w / 2) - (strlen(buf) * p->font[0]) / 2; // centrado horizontal
        int textY = p->y + p->h + 5; // debajo de la barra con margen de 5 px
        draw_string_fondo(textX, textY, buf, p->textColor, p->text_fondo, p->font);
    }
}
//barra vertical texto a la derecha
void barrav_draw1(Barra *p)
{
    // Borde
    draw_rectangle(
        p->x, p->y,
        p->x + p->w, p->y + p->h,
        p->colorMarco, 1
    );

    // Fondo
    draw_fill_rectangle(
        p->x + 1, p->y + 1,
        p->x + p->w - 1, p->y + p->h - 1,
        p->colorFondo
    );

    // Nivel según valor
    float rango = p->vmax - p->vmin;
    if (rango <= 0) rango = 1;

    int alto = (p->value - p->vmin) * p->h / rango;
    if (alto > 0) {
        draw_fill_rectangle(
            p->x + 1, p->y + p->h - alto,
            p->x + p->w - 1, p->y + p->h - 1,
            p->colorFill
        );
    }

    // Rayitas y números según numTicks
    int numTicks = (p->numTicks > 0) ? p->numTicks : 10; // por si no se pone
    for (int i = 0; i <= numTicks; i++)
    {
        int yTick = p->y + p->h - (i * p->h / numTicks);

        // Rayita
        draw_line(p->x - 4, yTick, p->x, yTick, p->colorMarco, 1);

        // Valor del tick
        int valorTick = p->vmin + (i * rango / numTicks);
        char buf[8];
        sprintf(buf, "%d", valorTick);

        // Número al lado de la barra
        int textX = p->x - 4 - strlen(buf) * p->font[0] - 2;
        int textY = yTick - (p->font[1] * 4) / 2;
        draw_string_fondo(textX, textY, buf, p->textColor, p->text_fondo, p->font);
    }

    // Valor actual
    if (p->showValue)
    {
        char buf[8];
        sprintf(buf, "%5.1f", p->value);

        int textX = p->x + p->w + 5;
        int textY = p->y + p->h / 2 - (p->font[1] * 4) / 2;
        draw_string_fondo(textX, textY, buf, p->textColor, p->text_fondo, p->font);
    }
}


void grafica_clear_fondo_y_grid(Grafica *g)
{
    // Fondo
    draw_fill_rectangle(
        g->x + 1,
        g->y + 1,
        g->x + g->w - 1,
        g->y + g->h - 1,
        g->colorFondo
    );

    // Grid horizontal
    for (int i = 0; i <= g->numTicksY; i++) {
        int yTick = g->y + g->h - (i * g->h / g->numTicksY);
        draw_line(
            g->x + 1,
            yTick,
            g->x + g->w - 1,
            yTick,
            g->colorGrid,
            1
        );
    }
}

void grafica_draw_rango(Grafica *g)
{
    char buf[12];

    for (int i = 0; i <= g->numTicksY; i++) {

        // Posición Y del tick
        int yTick = g->y + g->h - (i * g->h / g->numTicksY);

        // Valor correspondiente al rango
        float valor = g->vmin +
            (i * (g->vmax - g->vmin) / g->numTicksY);

        // ajusto tetxo a mostrar
        sprintf(buf, "%.1f", valor);

        // Texto a la derecha del marco
        draw_string(
            g->x + g->w + 4,
            yTick - 5,
            buf,
            COLOR_WHITE,
            font1
        );
    }
}


void grafica_init(Grafica *g)
{
    // Fondo completo
    draw_fill_rectangle(g->x, g->y, g->x + g->w, g->y + g->h, g->colorFondo);

    // Marco
    draw_rectangle(g->x, g->y, g->x + g->w, g->y + g->h, g->colorMarco, 1);

    // Grid horizontal
    for (int i = 0; i <= g->numTicksY; i++) {
        int yTick = g->y + g->h - (i * g->h / g->numTicksY);
        draw_line(g->x, yTick, g->x + g->w, yTick, g->colorGrid, 1);
    }

    g->indice = 0;
}

void grafica_add_point(Grafica *g, float nuevoValor)
{
    float rango = g->vmax - g->vmin;
    int x_actual = g->x + g->indice;

    // cargo el dato
    g->datos[g->indice] = nuevoValor;

    if (g->indice > 0) {
        int x_prev = g->x + (g->indice - 1);

        int y_prev = g->y + g->h - ((g->datos[g->indice - 1] - g->vmin) * g->h / rango);
        int y_now  = g->y + g->h - ((g->datos[g->indice] - g->vmin) * g->h / rango);

        // Restricciones para no dibujar fuera del área vertical
        if (y_prev < g->y) y_prev = g->y;
        if (y_now  < g->y) y_now  = g->y;
        if (y_prev > g->y + g->h) y_prev = g->y + g->h;
        if (y_now  > g->y + g->h) y_now  = g->y + g->h;

        draw_line(x_prev, y_prev, x_actual, y_now, g->colorLinea, 1);
    }

    g->indice++;

    // Control del Buffer Circular basado en el ANCHO de CADA gráfica
    if (g->indice >= g->w)
{
        g->indice = 0;

        // innio
        grafica_clear_fondo_y_grid(g);


        draw_rectangle(g->x, g->y, g->x + g->w, g->y + g->h, g->colorMarco, 1);
    }
}



void grafica_draw_titulo(Grafica *g, char *texto, uint16_t color, const uint8_t *font, uint8_t pos_v, uint8_t pos_h)
{
    int x_label, y_label;



    uint8_t ancho_letra = font[0];//tomo el ancho del font
    uint8_t alto_letra  = font[1] * 8;


    int ancho_texto = strlen(texto) * ancho_letra;

    if (pos_v == titulo_arriba) {

        y_label = g->y - alto_letra - 2;

        if (y_label < 0) y_label = 0;
    } else {

        y_label = g->y + g->h + 5;
    }

    switch (pos_h) {
        case titulo_izquierda:

            x_label = g->x;
            break;

        case  titulo_centro:

            x_label = g->x + (g->w / 2) - (ancho_texto / 2);
            break;

        case titulo_derecha:

            x_label = (g->x + g->w) - ancho_texto;
            break;

        default:
            x_label = g->x;
            break;
    }

    draw_string(x_label, y_label, texto, color,  font);
}
