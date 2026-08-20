#include "delays.h"


//define DHT22_PORT GPIOB
//#define DHT22_PIN GPIO_PIN_10

extern float DHT22_TEMP; // Declarar en main como float
extern float DHT22_HUM;  // Declarar en main como float


uint8_t DHT22_ok();
void  DHT22_Read();
