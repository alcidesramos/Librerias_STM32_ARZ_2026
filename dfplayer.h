#include "main.h"
#include "built_in.h"
#include "uart.h"

extern UART_HandleTypeDef huart3; //DEFINE EL UART A USAR
#define dfplayer_uart huart3
extern uint8_t play;
/*
 el el main
 HAL_Delay(500);
  mp3_inicializa();
  HAL_Delay(200);
  mp3_play_x(bienvenida);

 */

/*
 en el while  //define los sonidos en lo  define
 mp3_volumen_x(20);
 mp3_play_x(suiche_pres);
 */

void mp3_next();
void mp3_previo();
void mp3_play_x(uint16_t cancion);  //toca la cancion x  x va hasta 2999
void mp3_volumen_sube();
void mp3_volumen_baja();
void mp3_volumen_x(uint8_t vol);
void mp3_equializa(uint8_t equal);
void mp3_play_mode(uint8_t como);
void mp3_fuente(uint8_t como);
void mp3_modo_off();
void mp3_modo_normal();
void mp3_modo_reset();
void mp3_modo_play();
void mp3_modo_pausa();
void mp3_suena(uint8_t carp,uint8_t fila); //reprocuce una carpeta y un arcCHivo
void mp3_volumen_set(uint8_t prend,uint8_t nivel);//1=habilita el volumen deseado
void mp3_repite_ya(uint8_t como);    //1=repite  0=stop
void mp3_q_estado_usb();
void mp3_q_estado_sd();
void mp3_q_estado_flahs();
void mp3_inicializa();
void mp3_retransmite();
void mp3_responder(); //averiguar
void mp3_q_estado();
void mp3_q_volumen();
void mp3_q_equializa();
void mp3_q_play_mode();
void mp3_q_software();
void mp3_q_sd_filas();
void mp3_q_usb_filas();
void mp3_q_flahs_filas();
void mp3_q_on(); //averiguar
void mp3_q_sd_actual();
void mp3_q_usb_actual();
void mp3_q_flahs_actual();



