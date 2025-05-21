#ifndef On_Chip_SPI
#define On_Chip_SPI

#ifndef __SOFTSPI_
#define __SOFTSPI_

#include "f407.h"
#include "sysport.h"

#ifdef STM32F40_41xxx

// SPI协议通用接口
#define SOFT_SPI_SCK_PORT GPIOB
#define SOFT_SPI_MOSI_PORT GPIOB
#define SOFT_SPI_MISO_PORT GPIOB
#define SOFT_SPI_CS_PORT GPIOB

#define SOFT_SPI_SCK_PIN Pin3
#define SOFT_SPI_MOSI_PIN Pin5
#define SOFT_SPI_MISO_PIN Pin4
#define SOFT_SPI_CS_PIN Pin14

#define Soft_SPI_W_CS(spi_gpio, x) gpio.bsp_gpio_pin_set(spi_gpio, SOFT_SPI_CS_PIN, x)
#define Soft_SPI_W_SCK(spi_gpio, x) gpio.bsp_gpio_pin_set(spi_gpio, SOFT_SPI_SCK_PIN, x)
#define Soft_SPI_W_MOSI(spi_gpio, x) gpio.bsp_gpio_pin_set(spi_gpio, SOFT_SPI_MOSI_PIN, x)
#define Soft_SPI_R_MISO(spi_gpio) gpio.bsp_gpio_pin_get(spi_gpio, SOFT_SPI_MISO_PIN)

struct GPIO_Init_Parameters* Soft_SPI_Init(void);
void Soft_SPI_SendByte(void* const Parameters, uint8_t CS, uint8_t Byte);
void Soft_SPI_SendData(void* const Parameters, uint8_t CS, uint8_t* Data, uint8_t Count);
uint8_t Soft_SPI_ReceiveByte(void* const Parameters, uint8_t CS);
void Soft_SPI_ReceiveData(void* const Parameters, uint8_t CS, uint8_t* Data, uint8_t Count);
uint8_t Soft_SPI_SwapByte(void* const Parameters, uint8_t SData);

#endif
#endif
#endif