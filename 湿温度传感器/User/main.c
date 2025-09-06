#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/OLED/oled.h"
#include "./BSP/DHT22/dht22.h"
#include <stdio.h>

// 显示传感器数据
void display_sensor_data(float humi, float temp, uint8_t status) {
    // 清空数据区域
    oled_fill(40, 0, 127, 15, 0);
    oled_fill(40, 24, 127, 39, 0);
    
    // 显示湿度
    if (status == 0) {
        char humi_str[10];
        snprintf(humi_str, sizeof(humi_str), "%.1f%%", humi);
        oled_show_string(40, 0, humi_str, 16);
    } else {
        oled_show_string(40, 0, "Error", 16);
    }
    
    // 显示温度
	if (status == 0) {
        char temp_str[10];
		snprintf(temp_str, sizeof(temp_str), "%.1fC", temp);
		oled_show_string(40, 24, temp_str, 16);
    } else {
        oled_show_string(40, 24, "Error", 16);
    }
}

int main(void) {
    DHT22_Data sensor_data;

    // 系统初始化
    HAL_Init();
    sys_stm32_clock_init(RCC_PLL_MUL9);
    delay_init(72);
    led_init();
    usart_init(115200);
    oled_init();
    
    // 显示启动信息
    oled_clear();
    oled_show_string(20, 0, "DHT22 Monitor", 16);
    oled_show_string(10, 24, "Initializing...", 16);
    oled_refresh_gram();
    delay_ms(2000);
    
    // 传感器初始化
    if (DHT22_Init() != 0) {
        oled_clear();
        oled_show_string(10, 0, "Sensor Error!", 16);
        oled_show_string(0, 24, "Check Connection", 16);
        oled_refresh_gram();
        while(1); // 停止执行
    }
    
    // 主显示界面
    oled_clear();
    oled_show_string(0, 0, "Humi: ", 16);
    oled_show_string(0, 24, "Temp: ", 16);
    oled_refresh_gram();


    while(1) {
        // 读取传感器数据
        uint8_t result = DHT22_ReadData(&sensor_data);
        
        // 更新显示
        display_sensor_data(sensor_data.humidity, sensor_data.temperature, result);
        oled_refresh_gram();
        
        // 串口输出
        if (result == 0) {
            printf("Humidity: %.1f%%, Temperature: %.1fC\n", 
                   sensor_data.humidity, sensor_data.temperature);
        } else {
            printf("Sensor read error: %d\n", result);
        }
        
        // LED闪烁指示工作状态
        LED0_TOGGLE();
        delay_ms(2000); // 2秒更新一次
    }
}

