#include "stm32f4xx_hal.h"

extern SPI_HandleTypeDef hspi1;

#define RESET_ACTIVE() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET)
#define RESET_IDLE() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET)
#define CS_ACTIVE() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET)
#define CS_IDLE() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET)
#define DC_COMMAND() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET)
#define DC_DATA() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET)

#define DC_DATA() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET)

#define TFT9341_MADCTL_MY  0x80
#define TFT9341_MADCTL_MX  0x40
#define TFT9341_MADCTL_MV  0x20
#define TFT9341_MADCTL_ML  0x10
#define TFT9341_MADCTL_RGB 0x00
#define TFT9341_MADCTL_BGR 0x08
#define TFT9341_MADCTL_MH  0x04
#define TFT9341_ROTATION (TFT9341_MADCTL_MX | TFT9341_MADCTL_BGR)
#define	TFT9341_BLACK   0x0000
#define	TFT9341_BLUE    0x001F
#define	TFT9341_RED     0xF800
#define	TFT9341_GREEN   0x07E0
#define TFT9341_CYAN    0x07FF
#define TFT9341_MAGENTA 0xF81F
#define TFT9341_YELLOW  0xFFE0
#define TFT9341_WHITE   0xFFFF

#define swap(a,b) {int16_t t=a;a=b;b=t;}

void TFT9341_SendCommand(uint8_t cmd)
{
  DC_COMMAND();
  HAL_SPI_Transmit (&hspi1, &cmd, 1, 5000);
}

void TFT9341_SendData(uint8_t dt)
{
	DC_DATA();
	HAL_SPI_Transmit (&hspi1, &dt, 1, 5000);
}

static void TFT9341_WriteData(uint8_t* buff, size_t buff_size) {
	DC_DATA();
	while(buff_size > 0) {
		uint16_t chunk_size = buff_size > 32768 ? 32768 : buff_size;
		HAL_SPI_Transmit(&hspi1, buff, chunk_size, HAL_MAX_DELAY);
		buff += chunk_size;
		buff_size -= chunk_size;
	}
}

void TFT9341_reset(void)
{
	RESET_ACTIVE();
	HAL_Delay(5);
	RESET_IDLE();
}

static void TFT9341_SetAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  TFT9341_SendCommand(0x2A);
  {
    uint8_t data[] = { (x0 >> 8) & 0xFF, x0 & 0xFF, (x1 >> 8) & 0xFF, x1 & 0xFF };
    TFT9341_WriteData(data, sizeof(data));
  }
  TFT9341_SendCommand(0x2B);
  {
    uint8_t data[] = { (y0 >> 8) & 0xFF, y0 & 0xFF, (y1 >> 8) & 0xFF, y1 & 0xFF };
    TFT9341_WriteData(data, sizeof(data));
  }
  TFT9341_SendCommand(0x2C);
}

void TFT9341_FillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
  if((x1 >= TFT9341_WIDTH) || (y1 >= TFT9341_HEIGHT) || (x2 >= TFT9341_WIDTH) || (y2 >= TFT9341_HEIGHT)) return;
	if(x1>x2) swap(x1,x2);
	if(y1>y2) swap(y1,y2);
  TFT9341_SetAddrWindow(x1, y1, x2, y2);
  uint8_t data[] = { color >> 8, color & 0xFF };
  DC_DATA();
  for(uint32_t i = 0; i < (x2-x1+1)*(y2-y1+1); i++)
  {
      HAL_SPI_Transmit(&hspi1, data, 2, HAL_MAX_DELAY);
  }
}

void TFT9341_ini(uint16_t w_size, uint16_t h_size){
	uint8_t data[15];
	CS_ACTIVE();
	TFT9341_reset();
	TFT9341_SendCommand(0x01);
	HAL_Delay(1000);
	 HAL_Delay(1000);
	  data[0] = 0x39;
	  data[1] = 0x2C;
	  data[2] = 0x00;
	  data[3] = 0x34;
	  data[4] = 0x02;
	  TFT9341_SendCommand(0xCB);
	  TFT9341_WriteData(data, 5);
	  data[0] = 0x00;
	  data[1] = 0xC1;
	  data[2] = 0x30;
	  TFT9341_SendCommand(0xCF);
	  TFT9341_WriteData(data, 3);
	  data[0] = 0x85;
	  data[1] = 0x00;
	  data[2] = 0x78;
	  TFT9341_SendCommand(0xE8);
	  TFT9341_WriteData(data, 3);
	  data[0] = 0x00;
	  data[1] = 0x00;
	  TFT9341_SendCommand(0xEA);
	  TFT9341_WriteData(data, 2);
	  data[0] = 0x64;
	  data[1] = 0x03;
	  data[2] = 0x12;
	  data[3] = 0x81;
	  TFT9341_SendCommand(0xED);
	  TFT9341_WriteData(data, 4);
	  data[0] = 0x20;
	  TFT9341_SendCommand(0xF7);
	  TFT9341_WriteData(data, 1);
	  data[0] = 0x10;
	  TFT9341_SendCommand(0xC0);
	  TFT9341_WriteData(data, 1);
	  data[0] = 0x10;
	  TFT9341_SendCommand(0xC1);
	  TFT9341_WriteData(data, 1);
	  data[0] = 0x3E;
	  data[1] = 0x28;
	  TFT9341_SendCommand(0xC5);
	  TFT9341_WriteData(data, 2);
	  data[0] = 0x86;
	  TFT9341_SendCommand(0xC7);
	  TFT9341_WriteData(data, 1);
	  data[0] = 0x48;
	  TFT9341_SendCommand(0x36);
	  TFT9341_WriteData(data, 1);
	  data[0] = 0x55;
	  TFT9341_SendCommand(0x3A);
	  TFT9341_WriteData(data, 1);
	  data[0] = 0x00;
	  data[1] = 0x18;
	  TFT9341_SendCommand(0xB1);
	  TFT9341_WriteData(data, 2);
	  data[0] = 0x08;
	  data[1] = 0x82;
	  data[2] = 0x27;
	  TFT9341_SendCommand(0xB6);
	  TFT9341_WriteData(data, 3);
	  data[0] = 0x00;
	  TFT9341_SendCommand(0xF2);
	  TFT9341_WriteData(data, 1);
	  data[0] = 0x01;
	  TFT9341_SendCommand(0x26);
	  TFT9341_WriteData(data, 1);
	  data[0] = 0x0F;
	  data[1] = 0x31;
	  data[2] = 0x2B;
	  data[3] = 0x0C;
	  data[4] = 0x0E;
	  data[5] = 0x08;
	  data[6] = 0x4E;
	  data[7] = 0xF1;
	  data[8] = 0x37;
	  data[9] = 0x07;
	  data[10] = 0x10;
	  data[11] = 0x03;
	  data[12] = 0x0E;
	  data[13] = 0x09;
	  data[14] = 0x00;
	  TFT9341_SendCommand(0xE0);
	  TFT9341_WriteData(data, 15);
	  data[0] = 0x00;
	  data[1] = 0x0E;
	  data[2] = 0x14;
	  data[3] = 0x03;
	  data[4] = 0x11;
	  data[5] = 0x07;
	  data[6] = 0x31;
	  data[7] = 0xC1;
	  data[8] = 0x48;
	  data[9] = 0x08;
	  data[10] = 0x0F;
	  data[11] = 0x0C;
	  data[12] = 0x31;
	  data[13] = 0x36;
	  data[14] = 0x0F;
	  TFT9341_SendCommand(0xE1);
	  TFT9341_WriteData(data, 15);
	  TFT9341_SendCommand(0x11);
	  HAL_Delay(120);
	  data[0] = TFT9341_ROTATION;
	  TFT9341_SendCommand(0x29);
	  TFT9341_WriteData(data, 1);
	  TFT9341_WriteData(data, 1);
	  TFT9341_WIDTH = w_size;
	  TFT9341_HEIGHT = h_size;
}

void TFT9341_FillScreen(uint16_t color)
{
  TFT9341_FillRect(0, 0, TFT9341_WIDTH-1, TFT9341_HEIGHT-1, color);
}

void TFT9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color){
	TFT9341_FillRect(x, y, x, y, color);
}

void TFT9341_DrawMassive128x128(uint16_t mimage[16384]){
	uint16_t currcolnum = 0;
	uint16_t x = 0;
	for(uint16_t y = 0; y != TFT9341_HEIGHT;y++){
		for(; x != TFT9341_HEIGHT;x++){
			TFT9341_DrawPixel(y, x, mimage[currcolnum]);
			currcolnum++;
		}
		x = 0;
	}
}
