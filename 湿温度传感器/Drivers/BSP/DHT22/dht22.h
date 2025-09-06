#ifndef __DHT22_H
#define __DHT22_H

#include "stdlib.h" 
#include "./SYSTEM/sys/sys.h"

// ����DHT22���ӵ�GPIO����
#define DHT22_GPIO_PORT				GPIOA
#define DHT22_GPIO_PIN				GPIO_PIN_0
#define DHT22_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()

// ��ʪ�����ݽṹ��
typedef struct {
    float humidity;  // ʪ�ȣ���λ��%��
    float temperature; // �¶ȣ���λ��C��
} DHT22_Data;

// ��������
uint8_t DHT22_Init(void);                  // ��ʼ��DHT22
uint8_t DHT22_ReadData(DHT22_Data *data);  // ��ȡ��ʪ������

#endif


