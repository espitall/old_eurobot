#include <ch.h>
#include <hal.h>
#include "ili9341.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_ltdc.h"

#define ILI9341_WRX_CLEAR() palClearPad(GPIOD, GPIOD_LCD_WRX)
#define ILI9341_WRX_SET() palSetPad(GPIOD, GPIOD_LCD_WRX)
#define ILI9341_CS_CLEAR() palClearPad(GPIOC, GPIOC_SPI5_LCD_CS)
#define ILI9341_CS_SET() palSetPad(GPIOC, GPIOC_SPI5_LCD_CS)


void ili9341SendCmd(uint8_t data) {
  ILI9341_WRX_CLEAR();
  ILI9341_CS_CLEAR();
  spiSend(&SPID5, 1, &data);
  ILI9341_CS_SET();
}

void ili9341SendData(uint8_t data) {
  ILI9341_WRX_SET();
  ILI9341_CS_CLEAR();
  spiSend(&SPID5, 1, &data);
  ILI9341_CS_SET();
}

void ili9341Init(uint8_t * frame_buffer) 
{
  //init spi
  ILI9341_CS_SET();

  //init ili9341 chip
  ili9341SendCmd(0xCA);
  ili9341SendData(0xC3);
  ili9341SendData(0x08);
  ili9341SendData(0x50);
  ili9341SendCmd(ILI9341_POWERB);
  ili9341SendData(0x00);
  ili9341SendData(0xC1);
  ili9341SendData(0x30);
  ili9341SendCmd(ILI9341_POWER_SEQ);
  ili9341SendData(0x64);
  ili9341SendData(0x03);
  ili9341SendData(0x12);
  ili9341SendData(0x81);
  ili9341SendCmd(ILI9341_DTCA);
  ili9341SendData(0x85);
  ili9341SendData(0x00);
  ili9341SendData(0x78);
  ili9341SendCmd(ILI9341_POWERA);
  ili9341SendData(0x39);
  ili9341SendData(0x2C);
  ili9341SendData(0x00);
  ili9341SendData(0x34);
  ili9341SendData(0x02);
  ili9341SendCmd(ILI9341_PRC);
  ili9341SendData(0x20);
  ili9341SendCmd(ILI9341_DTCB);
  ili9341SendData(0x00);
  ili9341SendData(0x00);
  ili9341SendCmd(ILI9341_FRC);
  ili9341SendData(0x00);
  ili9341SendData(0x1B);
  ili9341SendCmd(ILI9341_DFC);
  ili9341SendData(0x0A);
  ili9341SendData(0xA2);
  ili9341SendCmd(ILI9341_POWER1);
  ili9341SendData(0x10);
  ili9341SendCmd(ILI9341_POWER2);
  ili9341SendData(0x10);
  ili9341SendCmd(ILI9341_VCOM1);
  ili9341SendData(0x45);
  ili9341SendData(0x15);
  ili9341SendCmd(ILI9341_VCOM2);
  ili9341SendData(0x90);
  ili9341SendCmd(ILI9341_MAC);
  ili9341SendData(0xC8);
  ili9341SendCmd(ILI9341_3GAMMA_EN);
  ili9341SendData(0x00);
  ili9341SendCmd(ILI9341_RGB_INTERFACE);
  ili9341SendData(0xC2);
  ili9341SendCmd(ILI9341_DFC);
  ili9341SendData(0x0A);
  ili9341SendData(0xA7);
  ili9341SendData(0x27);
  ili9341SendData(0x04);

  ili9341SendCmd(ILI9341_COLUMN_ADDR);
  ili9341SendData(0x00);
  ili9341SendData(0x00);
  ili9341SendData(0x00);
  ili9341SendData(0xEF);

  ili9341SendCmd(ILI9341_PAGE_ADDR);
  ili9341SendData(0x00);
  ili9341SendData(0x00);
  ili9341SendData(0x01);
  ili9341SendData(0x3F);
  ili9341SendCmd(ILI9341_INTERFACE);
  ili9341SendData(0x01);
  ili9341SendData(0x00);
  ili9341SendData(0x06);

  ili9341SendCmd(ILI9341_GRAM);
  chThdSleepMilliseconds(500);

  ili9341SendCmd(ILI9341_GAMMA);
  ili9341SendData(0x01);

  ili9341SendCmd(ILI9341_PGAMMA);
  ili9341SendData(0x0F);
  ili9341SendData(0x29);
  ili9341SendData(0x24);
  ili9341SendData(0x0C);
  ili9341SendData(0x0E);
  ili9341SendData(0x09);
  ili9341SendData(0x4E);
  ili9341SendData(0x78);
  ili9341SendData(0x3C);
  ili9341SendData(0x09);
  ili9341SendData(0x13);
  ili9341SendData(0x05);
  ili9341SendData(0x17);
  ili9341SendData(0x11);
  ili9341SendData(0x00);
  ili9341SendCmd(ILI9341_NGAMMA);
  ili9341SendData(0x00);
  ili9341SendData(0x16);
  ili9341SendData(0x1B);
  ili9341SendData(0x04);
  ili9341SendData(0x11);
  ili9341SendData(0x07);
  ili9341SendData(0x31);
  ili9341SendData(0x33);
  ili9341SendData(0x42);
  ili9341SendData(0x05);
  ili9341SendData(0x0C);
  ili9341SendData(0x0A);
  ili9341SendData(0x28);
  ili9341SendData(0x2F);
  ili9341SendData(0x0F);

  ili9341SendCmd(ILI9341_SLEEP_OUT);
  chThdSleepMilliseconds(500);
  ili9341SendCmd(ILI9341_DISPLAY_ON);

  ili9341SendCmd(ILI9341_GRAM);

  //init ltdc peripheral
  LTDC_InitTypeDef LTDC_InitStruct;
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_LTDC, ENABLE);
  RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_DMA2D, ENABLE); 
  RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_DMA2D, DISABLE); 
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_LTDC, DISABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE); 
  LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;     
  LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;     
  LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;     
  LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IPC;
  LTDC_InitStruct.LTDC_BackgroundRedValue = 0;            
  LTDC_InitStruct.LTDC_BackgroundGreenValue = 0;          
  LTDC_InitStruct.LTDC_BackgroundBlueValue = 0;  
  RCC_PLLSAIConfig(192, 7, 4);
  RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div4);
  RCC_PLLSAICmd(ENABLE);
  while (RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET);
  LTDC_InitStruct.LTDC_HorizontalSync = 9;
  LTDC_InitStruct.LTDC_VerticalSync = 1;
  LTDC_InitStruct.LTDC_AccumulatedHBP = 29; 
  LTDC_InitStruct.LTDC_AccumulatedVBP = 3;  
  LTDC_InitStruct.LTDC_AccumulatedActiveW = 269;
  LTDC_InitStruct.LTDC_AccumulatedActiveH = 323;
  LTDC_InitStruct.LTDC_TotalWidth = 279; 
  LTDC_InitStruct.LTDC_TotalHeigh = 327;
  LTDC_Init(&LTDC_InitStruct);

  //init static and dynamic layers
  LTDC_Layer_InitTypeDef LTDC_Layer_InitStruct;
  LTDC_Layer_InitStruct.LTDC_HorizontalStart = 30;
  LTDC_Layer_InitStruct.LTDC_HorizontalStop = 269;
  LTDC_Layer_InitStruct.LTDC_VerticalStart = 4;
  LTDC_Layer_InitStruct.LTDC_VerticalStop = 323;
  LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_ARGB8888;
  LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 255; 
  LTDC_Layer_InitStruct.LTDC_DefaultColorBlue = 0;        
  LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0;       
  LTDC_Layer_InitStruct.LTDC_DefaultColorRed = 0;         
  LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0;
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;    
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA;
  LTDC_Layer_InitStruct.LTDC_CFBLineLength = 240 * 4 + 3;
  LTDC_Layer_InitStruct.LTDC_CFBPitch = 240 * 4;
  LTDC_Layer_InitStruct.LTDC_CFBLineNumber = 320;
  LTDC_Layer_InitStruct.LTDC_CFBStartAdress = (uint32_t)(void *)frame_buffer;
  LTDC_LayerInit(LTDC_Layer1, &LTDC_Layer_InitStruct);

  LTDC_Layer_InitStruct.LTDC_CFBStartAdress = ((uint32_t)(void *)frame_buffer) + 4 * ILI9341_PIXEL;
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_PAxCA;
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_PAxCA;
  LTDC_LayerInit(LTDC_Layer2, &LTDC_Layer_InitStruct);
  LTDC_ReloadConfig(LTDC_IMReload);

  LTDC_LayerCmd(LTDC_Layer1, ENABLE);
  LTDC_LayerCmd(LTDC_Layer2, ENABLE);
  LTDC_ReloadConfig(LTDC_IMReload);

  LTDC_DitherCmd(ENABLE);
  LTDC_Cmd(ENABLE);
  LTDC_LayerAlpha(LTDC_Layer1, 255);
  LTDC_LayerAlpha(LTDC_Layer2, 125);
  LTDC_ReloadConfig(LTDC_IMReload);
}

void ili9341SetLayer(uint8_t layer)
{
  switch(layer)
  {
    case 0:
      LTDC_LayerAlpha(LTDC_Layer1, 255);
      LTDC_LayerAlpha(LTDC_Layer2, 0);
      LTDC_ReloadConfig(LTDC_IMReload);
      break;

    case 1:
      LTDC_LayerAlpha(LTDC_Layer1, 0);
      LTDC_LayerAlpha(LTDC_Layer2, 255);
      LTDC_ReloadConfig(LTDC_IMReload);
      break;
  }
}
