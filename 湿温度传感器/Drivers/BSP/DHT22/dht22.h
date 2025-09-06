#ifndef __DHT22_H
#define __DHT22_H

#include "stdlib.h" 
#include "./SYSTEM/sys/sys.h"

// 定义DHT22连接的GPIO引脚
#define DHT22_GPIO_PORT				GPIOA
#define DHT22_GPIO_PIN				GPIO_PIN_0
#define DHT22_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()

// 温湿度数据结构体
typedef struct {
    float humidity;  // 湿度（单位：%）
    float temperature; // 温度（单位：C）
} DHT22_Data;

// 函数声明
uint8_t DHT22_Init(void);                  // 初始化DHT22
uint8_t DHT22_ReadData(DHT22_Data *data);  // 读取温湿度数据

#endif


