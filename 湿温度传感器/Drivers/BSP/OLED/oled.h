#ifndef __OLED_H
#define __OLED_H

#include "stdlib.h" 
#include "./SYSTEM/sys/sys.h"

//OLED引脚定义
#define OLED_SPI_SCL_PORT					GPIOC
#define OLED_SPI_SCL_PIN					GPIO_PIN_0
#define OLED_SPI_SCL_CLK_ENABLE()			do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)

#define OLED_SPI_SDA_PORT					GPIOC
#define OLED_SPI_SDA_PIN					GPIO_PIN_1
#define OLED_SPI_SDA_CLK_ENABLE()			do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)

#define OLED_SPI_DC_PORT					GPIOD
#define OLED_SPI_DC_PIN						GPIO_PIN_3
#define OLED_SPI_DC_CLK_ENABLE()			do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)

#define OLED_SPI_CS_PORT					GPIOD
#define OLED_SPI_CS_PIN						GPIO_PIN_6
#define OLED_SPI_CS_CLK_ENABLE()			do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)

#define OLED_SPI_RST_PORT					GPIOG
#define OLED_SPI_RST_PIN					GPIO_PIN_15
#define OLED_SPI_RST_CLK_ENABLE()			do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)

//OLED端口定义
#define OLED_SCL(x)							do{ x ? \
														HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET) : \
														HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET); \
												}while(0)

#define OLED_SDA(x)							do{ x ? \
														HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET) : \
														HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET); \
												}while(0)

#define OLED_DC(x)							do{ x ? \
														HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET) : \
														HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET); \
												}while(0)

#define OLED_CS(x)							do{ x ? \
														HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET) : \
														HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET); \
												}while(0)

#define OLED_RST(x)							do{ x ? \
														HAL_GPIO_WritePin(GPIOG, GPIO_PIN_15, GPIO_PIN_SET) : \
														HAL_GPIO_WritePin(GPIOG, GPIO_PIN_15, GPIO_PIN_RESET); \
												}while(0)
//数据-命令定义
#define OLED_DATA	1
#define OLED_CMD	0



void oled_init(void);//初始化OLED
void oled_refresh_gram(void);//更新并显存到OLED
void oled_display_on(void);//开启显示
void oled_display_off(void);//关闭显示
void oled_clear(void);//清屏
void oled_draw_point(uint8_t x, uint8_t y, uint8_t dot);//画点
void oled_fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot);//矩形区域填充
void oled_show_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode);//显示字符
void oled_show_num(uint8_t x, uint8_t y,uint32_t num,  uint8_t len, uint8_t size);//显示数字
void oled_show_string(uint8_t x, uint8_t y, const char* p, uint8_t size);//显示字符串



#endif








