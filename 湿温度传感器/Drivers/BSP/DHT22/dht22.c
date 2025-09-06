#include "./BSP/DHT22/dht22.h"
#include "./SYSTEM/delay/delay.h"
#include <stdio.h>
//�������
static void DHT22_SetOutput(void) { 
    GPIO_InitTypeDef g_gpio_struct = {0};
    g_gpio_struct.Pin = DHT22_GPIO_PIN;
    g_gpio_struct.Mode = GPIO_MODE_OUTPUT_PP;
    g_gpio_struct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(DHT22_GPIO_PORT, &g_gpio_struct);
}
//��������
static void DHT22_SetInput(void) {
    GPIO_InitTypeDef g_gpio_struct = {0};
    g_gpio_struct.Pin = DHT22_GPIO_PIN;
    g_gpio_struct.Mode = GPIO_MODE_INPUT;
    g_gpio_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(DHT22_GPIO_PORT, &g_gpio_struct);
}
//�ȴ��ӳ�
static uint8_t DHT22_WaitFor(uint8_t level, uint16_t timeout) {
    uint32_t start = HAL_GetTick();
    while (HAL_GPIO_ReadPin(DHT22_GPIO_PORT, DHT22_GPIO_PIN) != level) {
        if ((HAL_GetTick() - start) > timeout) return 0;
        delay_us(10);
    }
    return 1;
}
//��ȡһ��λ
static uint8_t DHT22_ReadBit(void) {
    if (!DHT22_WaitFor(0, 100)) return 0;  // �ȴ��͵�ƽ
    if (!DHT22_WaitFor(1, 100)) return 0;  // �ȴ��ߵ�ƽ
    delay_us(30);
    return HAL_GPIO_ReadPin(DHT22_GPIO_PORT, DHT22_GPIO_PIN);
}
//��ȡһ���ֽ�
static uint8_t DHT22_ReadByte(void) {
    uint8_t byte = 0;
    for (uint8_t i = 0; i < 8; i++) {
        byte <<= 1;
        byte |= DHT22_ReadBit();
    }
    return byte;
}
//��ʼ��������
uint8_t DHT22_Init(void) {
    DHT22_GPIO_CLK_ENABLE();
    DHT22_SetInput();
    delay_ms(1000);
    return 0;
}
//��ȡ����������
uint8_t DHT22_ReadData(DHT22_Data *data) {
    uint8_t buf[5] = {0};
    
    // ���Ϳ�ʼ�ź�
    DHT22_SetOutput();
    HAL_GPIO_WritePin(DHT22_GPIO_PORT, DHT22_GPIO_PIN, GPIO_PIN_RESET);
    delay_ms(1);
    HAL_GPIO_WritePin(DHT22_GPIO_PORT, DHT22_GPIO_PIN, GPIO_PIN_SET);
    delay_us(30);
    DHT22_SetInput();
    
    // �ȴ���������Ӧ
    if (!DHT22_WaitFor(0, 10)) return 1;
    if (!DHT22_WaitFor(1, 10)) return 1;
    if (!DHT22_WaitFor(0, 10)) return 1;
    
    // ��ȡ����
    for (uint8_t i = 0; i < 5; i++) {
        buf[i] = DHT22_ReadByte();
    }
    
    // ���������Ч��
    if (buf[0] == 0 && buf[1] == 0 && buf[2] == 0 && buf[3] == 0) {
        return 2; // �����쳣
    }
    
    // ��������
    data->humidity = ((buf[0] << 8) | buf[1]) / 10.0f;
    int16_t temp = (buf[2] << 8) | buf[3];
    data->temperature = (temp & 0x8000) ? -(temp & 0x7FFF) / 10.0f : temp / 10.0f;
    
    return 0;
}

