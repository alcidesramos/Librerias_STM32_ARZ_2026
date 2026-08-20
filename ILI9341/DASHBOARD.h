/*
 * DASHBOARD.h
 *
 *  Created on: Sep 21, 2025
 *      Author: ALCIDES_RAMOS
 */

#ifndef LIBRERIAS_DASHBOARD_H_
#define LIBRERIAS_DASHBOARD_H_
#include "main.h"
#include "stdbool.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "fonts.h"

typedef struct {
    int x;          // Coordenada X del centro
    int y;          // Coordenada Y del centro
    int r;          // Radio del gauge
    int w;          // Grosor del anillo

    int vmin;       // Valor mínimo
    int vmax;       // Valor máximo
    int value;      // Valor actual

    int Ang_ini;    // Ángulo inicial
    int Ang_fin;    // Ángulo final
    int Res_ang;    // Resolución angular (ej: 2°, 5°...)

    uint16_t colorIni;    // Color inicio (degradado)
    uint16_t colorMed;    // Color intermedio (degradado)
    uint16_t colorFin;    // Color final (degradado)
    uint16_t colorBase;   // Color de fondo del arco

    uint16_t fondo;       // Color de fondo del texto
    bool showValue;       // Muestra valor
    uint16_t textColor;   // Color del texto
    const unsigned char *font; // Fuente usada
} Gauge_ARZ;

typedef struct {
    uint16_t x;          // centro X del gauge
    uint16_t y;          // centro Y del gauge
    uint16_t r;          // radio exterior del arco
    uint16_t grosor;     // grosor del arco
    uint16_t start_angle;// ángulo inicial (grados)
    uint16_t end_angle;  // ángulo final (grados)
    uint16_t valor;      // valor actual (0-100%)
    uint16_t color_base; // color de fondo (ej: azul)
    uint16_t color_fill; // color que avanza (ej: blanco)
} GaugeArc;

// Estructura de un termómetro estilo LabVIEW
typedef struct {
    int x;          // Centro X de la base del termómetro
    int y;          // Y de la base del termómetro
    int w;          // Ancho total
    int h;          // Altura total

    int vmin;       // Valor mínimo
    int vmax;       // Valor máximo
    float value;      // Valor actual
    uint8_t  numTicks;     // cantidad de rayitas y números
    uint16_t colorFondo; // Color interior (ej: gris)
    uint16_t colorFill;  // Color de la columna activa (ej: rojo)
    uint16_t colorMarco; // Color del borde exterior

    bool showValue;       // Mostrar valor actual en el centro/base
    uint16_t textColor;   // Color del texto
    uint16_t text_fondo;   //color fonfo de texto

    const unsigned char *font; // Fuente usada

} Termo_ARZ;
typedef struct {
    uint16_t x;         // Posición X del centro
    uint16_t y;         // Posición Y del centro
    uint16_t r;         // Radio del LED
    uint16_t color_on;  // Color cuando está encendido
    uint16_t color_off; // Color cuando está apagado
    uint8_t  estado;    // 1 = encendido, 0 = apagado muedsta valornuemrico

} Led;

typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
    uint16_t vmin;
    uint16_t vmax;
    float value;
    uint16_t colorFondo;
    uint16_t colorFill;
    uint16_t colorMarco;
    uint8_t  showValue;    // Mostrar valor central
    uint16_t textColor;
    uint16_t text_fondo;
    const uint8_t *font;
    uint8_t  numTicks;     // cantidad de rayitas y números
} Barra;


#define titulo_arriba  1
#define titulo_abajo  0
#define titulo_izquierda  0
#define titulo_centro  1
#define titulo_derecha  2

//colocar anho d el pantalla  que es el limite
#define MAX_PUNTOS_GRAFICA 320

typedef struct {
    uint16_t x;          // Posición X superior izquierda
    uint16_t y;          // Posición Y superior izquierda
    uint16_t w;          // Ancho de la gráfica
    uint16_t h;          // Alto de la gráfica

    float vmin;          // Valor mínimo esperado (ej: 0.0)
    float vmax;          // Valor máximo esperado (ej: 100.0)

    uint16_t colorMarco; // Color de los ejes
    uint16_t colorGrid;  // Color de la cuadrícula
    uint16_t colorLinea; // Color de la serie de datos
    uint16_t colorFondo; // Color de fondo del área de dibujo

    float datos[MAX_PUNTOS_GRAFICA]; // Almacén de puntos
    uint16_t indice;                 // Índice actual para el buffer circular
    uint8_t numTicksY;               // Divisiones en el eje Y
   } Grafica;


//si se queiren inciar en el main declare los objetos a usar
extern Gauge_ARZ gauge1;
extern Led led1;
extern Led led2;
extern Barra barrah1;
extern Barra barrav1;
extern Grafica grafica1;
extern Grafica grafica2;

extern float data_seno[];

void gauge_draw_arc(Gauge_ARZ *g) ;
void led_draw(Led *led);
void led_draw1(Led *led);//otro opcion de led
void barrah_draw(Barra *p);
void barrav_draw(Barra *p);//mutsra texto abajo
void barrav_draw1(Barra *p);//muestra texto  a la derecha
void grafica_init(Grafica *g);
void grafica_add_point(Grafica *g, float nuevoValor) ;
void grafica_draw_rango(Grafica *g);
void grafica_draw_titulo(Grafica *g, char *texto, uint16_t color, const uint8_t *font, uint8_t pos_v, uint8_t pos_h);

#endif /* LIBRERIAS_DASHBOARD_H_ */
