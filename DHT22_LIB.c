#include "DHT22_LIB.h"

extern float DHT22_TEMP; // Declarar en main como float
extern float DHT22_HUM;  // Declarar en main como float

uint8_t DHT22_CHKSM;

void DHT22_start(void)
{
    Output_Pin(DHT22_GPIO_Port, DHT22_Pin);
    HAL_GPIO_WritePin(DHT22_GPIO_Port, DHT22_Pin, 1);
    HAL_Delay(50);

    HAL_GPIO_WritePin(DHT22_GPIO_Port, DHT22_Pin, 0);
    HAL_Delay(1); // Para DHT22 basta con 1ms
    Input_Pin(DHT22_GPIO_Port, DHT22_Pin);
    delay_us(30);
}

uint8_t DHT22_ok()
{
    DHT22_start();
    if (!waitforhigh(DHT22_GPIO_Port, DHT22_Pin, 100)) return 0;
    if (!waitforlow(DHT22_GPIO_Port, DHT22_Pin, 100)) return 0;
    if (!waitforhigh(DHT22_GPIO_Port, DHT22_Pin, 100)) return 0;
    return 1;
}

void DHT22_Read()
{
    uint8_t i = 0;
    uint8_t data[5] = {0};

    Input_Pin(DHT22_GPIO_Port, DHT22_Pin);

    for (i = 0; i < 40; i++) {
        while (!HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin));
        delay_us(30);
        if (HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin))
            data[i / 8] |= (1 << (7 - (i % 8)));

        while (HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin));
    }

    DHT22_CHKSM = data[0] + data[1] + data[2] + data[3];

    if (DHT22_CHKSM == data[4]) {
        DHT22_HUM = ((data[0] << 8) | data[1]) * 0.1f;
        int16_t temp_raw = (data[2] << 8) | data[3];
        if (temp_raw & 0x8000) {
            temp_raw &= 0x7FFF;
            DHT22_TEMP = -temp_raw * 0.1f;
        } else {
            DHT22_TEMP = temp_raw * 0.1f;
        }
    }

    HAL_Delay(1);
}
