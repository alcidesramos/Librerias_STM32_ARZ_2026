
#include "dfplayer.h"


const  uint8_t  inicio=0x7E;
 const  uint8_t  fin=0XEF;
 const  uint8_t version=0xFF;
 const  uint8_t  eco=0;   // si se coloca 1 responde con eco
 const  uint8_t largo=0x06;
uint16_t checksum;
uint8_t comando;
uint16_t parametro=0;
uint8_t play=1;

//comando
  #define  next 0x01
  #define  previus 0x02
  #define  archivo 0x03      //de 0 a 2999
  #define  volumen_sub 0x04
  #define  volumen_baj 0x05
  #define  volumen 0x06    //  de 0 a 30
  #define  equializa 0x07
     #define  normal  0
     #define  pop      1
     #define  rock     2
     #define  jazz     3
     #define  classic  4
     #define  base     5
  #define  repetir 0x08
     #define  todo       0
     #define  folder     1
     #define  actual     2
     #define  random     3
  #define  fuente 0x09
   #define  usb       0
   #define  sd        1
   #define  aux       2
   #define  sleep     3
   #define  flash     4
  #define  modo_on     0x0A
  #define  modo_normal 0x0B
  #define  modo_reset  0x0C
  #define  modo_play   0x0D
  #define  modo_pausa  0x0E
  #define  carpeta  0x0F   //suena carpeta seleccionada se DH=Folde  DL=cancion  folde 01~99
  #define  set_volumen 0x10    //parte alta=1 ajuta      parte baja= ganancia  0~31
  #define  repite_ya  0x011  //1 = repite cancion   0=para
  
  //consultas
  #define  es_usb       0x03C
  #define  es_sd        0x03D
  #define  es_flahs     0x03E
  #define  inicializa    0x03F
  #define  retransmitir     0x040
  #define  responder    0x041
  #define  p_estado    0x042
  #define  p_volumen    0x043
  #define  p_equaliza    0x044
  #define  p_modo_rep    0x045
  #define  p_sofware    0x046
  #define  sd_filas    0x047
  #define  usb_filas    0x048
  #define  flahs_filas    0x049
  #define  on_    0x04A
  #define  sd_actual    0x04B
  #define  usb_actual    0x04C
  #define  flahs_actual    0x04D
  
void calula_checksum()
{
checksum=0;

checksum=version+largo+comando+eco+hi(parametro)+lo(parametro);
checksum=-checksum;
}


void manda_mp3()
{
  uartx_write(&dfplayer_uart,inicio);
  uartx_write(&dfplayer_uart,version);
  uartx_write(&dfplayer_uart,largo);
  uartx_write(&dfplayer_uart,comando);
  uartx_write(&dfplayer_uart,eco);
  uartx_write(&dfplayer_uart,hi(parametro));
  uartx_write(&dfplayer_uart,lo(parametro));
  uartx_write(&dfplayer_uart,hi(checksum));
  uartx_write(&dfplayer_uart,lo(checksum));
  uartx_write(&dfplayer_uart,fin);
  parametro=0;
  HAL_Delay(100);
}

void mp3_next()
{
comando=next;
calula_checksum();
manda_mp3();
}

void mp3_previo()
{
comando=previus;
calula_checksum();
manda_mp3();
}

void mp3_play_x(uint16_t cancion)
{
comando=archivo;
parametro=cancion;
calula_checksum();
manda_mp3();
}

void mp3_volumen_sube()
{
comando=volumen_sub;
calula_checksum();
manda_mp3();
}

void mp3_volumen_baja()
{
comando=volumen_baj;
calula_checksum();
manda_mp3();
}

void mp3_volumen_x(uint8_t vol)
{
comando=volumen;
parametro=vol;
calula_checksum();
manda_mp3();
}

void mp3_equializa(uint8_t equal)
{
comando=equializa;
parametro=equal;
calula_checksum();
manda_mp3();
}

void mp3_play_mode(uint8_t como)
{
comando=repetir;
parametro=como;
calula_checksum();
manda_mp3();
}

void mp3_fuente(uint8_t como)
{
comando=fuente;
parametro=como;
calula_checksum();
manda_mp3();
}

void mp3_modo_off()
{
comando=modo_on;
calula_checksum();
manda_mp3();
}


void mp3_modo_normal()
{
comando=modo_normal;
calula_checksum();
manda_mp3();
}


void mp3_modo_reset()
{
comando=modo_reset;
calula_checksum();
manda_mp3();
}


void mp3_modo_play()
{
comando=modo_play;
calula_checksum();
manda_mp3();
}


void mp3_modo_pausa()
{
comando=modo_pausa;
calula_checksum();
manda_mp3();
}




void mp3_suena(uint8_t carp,uint8_t fila)
{
comando=carpeta;
parametro=(uint16_t) carp<<8 | fila;
calula_checksum();
manda_mp3();
}


void mp3_volumen_set(uint8_t prend,uint8_t nivel)
{
comando=set_volumen;
parametro=(uint16_t) prend<<8 | nivel;
calula_checksum();
manda_mp3();
}

void mp3_repite_ya(uint8_t como)    //1=repite  0=stop
{
comando=repite_ya;
parametro=como;
calula_checksum();
manda_mp3();
}

void mp3_q_estado_usb()
{
comando=es_usb;
calula_checksum();
manda_mp3();
}

void mp3_q_estado_sd()
{
comando=es_sd;
calula_checksum();
manda_mp3();

}

void mp3_q_estado_flahs()
{
comando=es_flahs;
calula_checksum();
manda_mp3();
}

void mp3_inicializa()
{
comando=inicializa;
calula_checksum();
manda_mp3();
}

void mp3_retransmite()
{
comando=retransmitir;
calula_checksum();
manda_mp3();
}

void mp3_responder()
{
comando=responder;
calula_checksum();
manda_mp3();
}


void mp3_q_estado()
{
comando=p_estado;
calula_checksum();
manda_mp3();
}


void mp3_q_volumen()
{
comando=p_volumen;
calula_checksum();
manda_mp3();
}



void mp3_q_equializa()
{
comando=p_equaliza;
calula_checksum();
manda_mp3();
}


void mp3_q_play_mode()
{
comando=p_modo_rep;
calula_checksum();
manda_mp3();
}

void mp3_q_software()
{
comando=p_sofware;
calula_checksum();
manda_mp3();
}


void mp3_q_sd_filas()
{
comando=sd_filas;
calula_checksum();
manda_mp3();
}



void mp3_q_usb_filas()
{
comando=usb_filas;
calula_checksum();
manda_mp3();
}



void mp3_q_flahs_filas()
{
comando=flahs_filas;
calula_checksum();
manda_mp3();
}



void mp3_q_on()
{
comando=on_;
calula_checksum();
manda_mp3();
}

void mp3_q_sd_actual()
{
comando=sd_actual;
calula_checksum();
manda_mp3();
}

void mp3_q_usb_actual()
{
comando=usb_actual;
calula_checksum();
manda_mp3();
}

void mp3_q_flahs_actual()
{
comando=flahs_actual;
calula_checksum();
manda_mp3();
}
