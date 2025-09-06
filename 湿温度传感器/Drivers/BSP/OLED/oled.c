#include "stdlib.h"
#include "./BSP/OLED/oled.h"
#include "./BSP/OLED/oledfont.h"
#include "./SYSTEM/delay/delay.h"



static uint8_t g_oled_gram[128][8];         			/* OLED��ʾ���� */
static void oled_wr_byte(uint8_t data, uint8_t cmd);    /* дһ���ֽڵ�OLED��0��ʾ�������1��ʾ��������*/
static uint32_t oled_pow(uint8_t m, uint8_t n);         /* OLED����ֵ��� */


void oled_wr_byte(uint8_t data, uint8_t cmd)
{
	uint8_t i;
	OLED_DC(cmd);
	OLED_CS(0);
	
	for(i = 0; i < 8; i++)
	{
		OLED_SCL(0);
		if(data & 0x80)
		{
			OLED_SDA(1);
		}else
		{
			OLED_SDA(0);
		}
		OLED_SCL(1);
		data <<= 1;
	}
	OLED_DC(1);
	OLED_CS(1);
}

void oled_refresh_gram(void)
{
	uint8_t i, n;
	for(i = 0; i < 8; i++)
	{
		oled_wr_byte(0xb0 + i, OLED_CMD);
		oled_wr_byte(0x00, OLED_CMD);
		oled_wr_byte(0x10, OLED_CMD);
		for(n = 0; n < 128; n++)
		{
			oled_wr_byte(g_oled_gram[n][i], OLED_DATA);
		}
	}
}

void oled_display_on(void)
{
	oled_wr_byte(0x8D, OLED_CMD);
	oled_wr_byte(0x14, OLED_CMD);
	oled_wr_byte(0xAF, OLED_CMD);
}

void oled_display_off(void)
{
	oled_wr_byte(0x8D, OLED_CMD);
	oled_wr_byte(0x10, OLED_CMD);
	oled_wr_byte(0xAE, OLED_CMD);
}

void oled_clear(void)
{
	uint8_t i, n;
	for(i = 0; i < 8; i++)
	{
		for(n = 0; n < 128; n++)
		{
			g_oled_gram[n][i] = 0x00;
		}
	}
	oled_refresh_gram();
}

void oled_draw_point(uint8_t x, uint8_t y, uint8_t dot)
{
	uint8_t pos, bx, temp = 0;
	
	if(x > 127 || y > 63) return;
	
	pos = y / 8;
	bx = y % 8;
	temp = 1 << bx;
	
	if(dot)
	{
		g_oled_gram[x][pos] |= temp;
	}else
	{
		g_oled_gram[x][pos] &= ~temp;
	}
}

void oled_fill(uint8_t x1, uint8_t y1,uint8_t x2, uint8_t y2, uint8_t dot)
{
	uint8_t x, y;
	
	for(x = x1; x <= x2; x++)
	{
		for(y = y1; y <= y2; y++)
		{
			oled_draw_point(x, y, dot);
		}
	}
	oled_refresh_gram();
}

void oled_show_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode)
{
	uint8_t temp, t, t1;
	uint8_t y0 = y;
	uint8_t* pfont = 0;
	uint8_t csize = ((size / 8) + ((size % 8) ? 1 : 0)) * (size / 2);
	chr = chr - ' ';
	
	if(size == 12)
	{
		pfont = (uint8_t* )oled_asc2_1206[chr];
	}else if(size == 16)
	{
		pfont = (uint8_t* )oled_asc2_1608[chr];
	}else if(size == 24)
	{
		pfont = (uint8_t* )oled_asc2_2412[chr];
	}else
	{
		return;
	}
	
	for(t = 0; t < csize; t++)
	{
		temp = pfont[t];
		for(t1 = 0; t1 < 8; t1++)
		{
			if(temp & 0x80)
			{
				oled_draw_point(x, y, mode);
			}else
			{
				oled_draw_point(x, y, !mode);
			}
			
			temp <<= 1;
			y++;
			
			if((y - y0) == size)
			{
				y = y0;
				x++;
				break;
			}
		}
	}
}

static uint32_t oled_pow(uint8_t m, uint8_t n)
{
	uint32_t result = 1;
	while(n--)
	{
		result *= m;
	}
	return result;
}

void oled_show_num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size)
{
	uint8_t temp, t;
	uint8_t enshow = 0;
	
	for(t = 0; t < len; t++)
	{
		temp = (num / oled_pow(10, len - t - 1)) % 10;
		if(enshow == 0 && t < len - 1)
		{
			if(temp == 0)
			{
				oled_show_char(x + (size / 2) * t, y, ' ', size, 1);
				continue;
			}else
			{
				enshow = 1;
			}
		}
		oled_show_char(x + (size / 2) * t, y, temp + '0', size, 1);
	}
}

void oled_show_string(uint8_t x, uint8_t y, const char* p, uint8_t size)
{
	while((*p >= ' ') && (*p <= '~'))
	{
		if(x > (128 - (size / 2)))
		{
			x = 0;
			y += size;
		}
		if(y > (64 - size))
		{
			x = y = 0;
			oled_clear();
		}
		oled_show_char(x, y, *p, size, 1);
		x += size / 2;
		p++;
	}
}

void oled_init()
{
	GPIO_InitTypeDef g_gpio_struct;
	
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	
	g_gpio_struct.Mode = GPIO_MODE_OUTPUT_PP;
	g_gpio_struct.Pin = OLED_SPI_SCL_PIN;
	g_gpio_struct.Pull = GPIO_PULLUP;
	g_gpio_struct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(OLED_SPI_SCL_PORT, &g_gpio_struct);
	
	g_gpio_struct.Mode = GPIO_MODE_OUTPUT_PP;
	g_gpio_struct.Pin = OLED_SPI_SDA_PIN;
	g_gpio_struct.Pull = GPIO_PULLUP;
	g_gpio_struct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(OLED_SPI_SDA_PORT, &g_gpio_struct);
	
	g_gpio_struct.Mode = GPIO_MODE_OUTPUT_PP;
	g_gpio_struct.Pin = OLED_SPI_DC_PIN;
	g_gpio_struct.Pull = GPIO_PULLUP;
	g_gpio_struct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(OLED_SPI_DC_PORT, &g_gpio_struct);
	
	g_gpio_struct.Mode = GPIO_MODE_OUTPUT_PP;
	g_gpio_struct.Pin = OLED_SPI_CS_PIN;
	g_gpio_struct.Pull = GPIO_PULLUP;
	g_gpio_struct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(OLED_SPI_CS_PORT, &g_gpio_struct);
	
	g_gpio_struct.Mode = GPIO_MODE_OUTPUT_PP;
	g_gpio_struct.Pin = OLED_SPI_RST_PIN;
	g_gpio_struct.Pull = GPIO_PULLUP;
	g_gpio_struct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(OLED_SPI_RST_PORT, &g_gpio_struct);
	
	OLED_CS(1);
	OLED_DC(1);
	
	OLED_SCL(1);
	OLED_SDA(1);
	
	OLED_RST(0);
	delay_ms(100);
	OLED_RST(1);
	
	oled_wr_byte(0xAE, OLED_CMD);   /* �ر���ʾ */
    oled_wr_byte(0xD5, OLED_CMD);   /* ����ʱ�ӷ�Ƶ����,��Ƶ�� */
    oled_wr_byte(80, OLED_CMD);     /* [3:0],��Ƶ����;[7:4],��Ƶ�� */
    oled_wr_byte(0xA8, OLED_CMD);   /* ��������·�� */
    oled_wr_byte(0X3F, OLED_CMD);   /* Ĭ��0X3F(1/64) */
    oled_wr_byte(0xD3, OLED_CMD);   /* ������ʾƫ�� */
    oled_wr_byte(0X00, OLED_CMD);   /* Ĭ��Ϊ0 */

    oled_wr_byte(0x40, OLED_CMD);   /* ������ʾ��ʼ�� [5:0],����. */

    oled_wr_byte(0x8D, OLED_CMD);   /* ��ɱ����� */
    oled_wr_byte(0x14, OLED_CMD);   /* bit2������/�ر� */
    oled_wr_byte(0x20, OLED_CMD);   /* �����ڴ��ַģʽ */
    oled_wr_byte(0x02, OLED_CMD);   /* [1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10; */
    oled_wr_byte(0xA1, OLED_CMD);   /* ���ض�������,bit0:0,0->0;1,0->127; */
    oled_wr_byte(0xC8, OLED_CMD);   /* ����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·�� */
    oled_wr_byte(0xDA, OLED_CMD);   /* ����COMӲ���������� */
    oled_wr_byte(0x12, OLED_CMD);   /* [5:4]���� */

    oled_wr_byte(0x81, OLED_CMD);   /* �Աȶ����� */
    oled_wr_byte(0xEF, OLED_CMD);   /* 1~255;Ĭ��0X7F (��������,Խ��Խ��) */
    oled_wr_byte(0xD9, OLED_CMD);   /* ����Ԥ������� */
    oled_wr_byte(0xf1, OLED_CMD);   /* [3:0],PHASE 1;[7:4],PHASE 2; */
    oled_wr_byte(0xDB, OLED_CMD);   /* ����VCOMH ��ѹ���� */
    oled_wr_byte(0x30, OLED_CMD);   /* [6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc; */

    oled_wr_byte(0xA4, OLED_CMD);   /* ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����) */
    oled_wr_byte(0xA6, OLED_CMD);   /* ������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ */
    oled_wr_byte(0xAF, OLED_CMD);   /* ������ʾ */
    oled_clear();
}

















