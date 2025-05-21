#include "f407.h"
#ifdef STM32F40_41xxx
#include "f407_spi.h"
#include "f407_gpio.h"
#ifndef Soft_SPI_W_CS

// void Soft_SPI_W_CS(uint8_t BitValue)
// {
// 	#ifdef __STM32F4xx_GPIO_H
// 	GPIO_WriteBit(SOFT_SPI_CS_PORT, SOFT_SPI_CS_PIN, (BitAction)BitValue);
// 	#else
// 	bsp_gpio_pin_set(SOFT_SPI_CS_PORT,SOFT_SPI_CS_PIN,BitValue);
// 	#endif
// }

// void Soft_SPI_W_SCK(uint8_t BitValue)
// {
// 	#ifdef __STM32F4xx_GPIO_H
// 	GPIO_WriteBit(SOFT_SPI_SCK_PORT, SOFT_SPI_SCK_PIN, (BitAction)BitValue);
// 	#else
// 	bsp_gpio_pin_set(SOFT_SPI_SCK_PORT,SOFT_SPI_SCK_PIN,BitValue);
// 	#endif
// }

// void Soft_SPI_W_MOSI(uint8_t BitValue)
// {
// 	#ifdef __STM32F4xx_GPIO_H
// 	GPIO_WriteBit(SOFT_SPI_MOSI_PORT, SOFT_SPI_MOSI_PIN, (BitAction)BitValue);
// 	#else
// 	bsp_gpio_pin_set(SOFT_SPI_MOSI_PORT,SOFT_SPI_MOSI_PIN,BitValue);
// 	#endif
// }

// uint8_t Soft_SPI_R_MISO(void)
// {
// 	#ifdef __STM32F4xx_GPIO_H
// 	return GPIO_ReadInputDataBit(SOFT_SPI_MISO_PORT, SOFT_SPI_MISO_PIN);
// 	#else
// 	return bsp_gpio_pin_get(SOFT_SPI_MISO_PORT,SOFT_SPI_MISO_PIN);
// 	#endif
// }

#endif



/*引脚初始化*/
struct GPIO_Init_Parameters* Soft_SPI_Init(void)
{
    static GPIO_Init_Parameters spi_gpio;
    spi_gpio.GPIOx = SOFT_SPI_SCK_PORT;
    spi_gpio.GPIO_Pin_Source[SOFT_SPI_SCK_PIN] = ENABLE;
    spi_gpio.GPIO_Pin_Source[SOFT_SPI_MOSI_PIN] = ENABLE;
    spi_gpio.GPIO_Pin_Source[SOFT_SPI_MISO_PIN] = ENABLE;
    spi_gpio.GPIO_Pin_Source[SOFT_SPI_CS_PIN] = ENABLE;
    spi_gpio.GPIO_Mode_x = SYS_GPIO_MODE_OUT;
    spi_gpio.GPIO_OType_xx = SYS_GPIO_OTYPE_PP;
    spi_gpio.GPIO_Speed_x = SYS_GPIO_SPEED_FAST;
    spi_gpio.GPIO_PuPd_x = SYS_GPIO_PUPD_PU;
    spi_gpio.GPIO_AF = 0; // 复用功能设置为0

    // 初始化通用接口
    gpio.bsp_gpio_init(&spi_gpio);
    
    Soft_SPI_W_CS(&spi_gpio,1);
	Soft_SPI_W_SCK(&spi_gpio,0);
    return &spi_gpio;
}

/**
  * @brief  SPI发送一个字节
  * @param  Byte 要发送的一个字节
  * @param  CS 片选
  * @retval 无
  */
void Soft_SPI_SendByte(void* const Parameters,uint8_t CS,uint8_t Byte)
{
    GPIO_Init_Parameters* _spi = (GPIO_Init_Parameters*)Parameters;
    Soft_SPI_W_CS(_spi,0);

	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		Soft_SPI_W_MOSI(_spi,!!Byte & (0x80 >> i));

		Soft_SPI_W_SCK(_spi,1);
		Soft_SPI_W_SCK(_spi,0);
	}

    Soft_SPI_W_CS(_spi,1);
}


void Soft_SPI_SendData(void* const Parameters,uint8_t CS,uint8_t *Data,uint8_t Count)
{
    GPIO_Init_Parameters* _spi = (GPIO_Init_Parameters*)Parameters;

    uint8_t i;
    for(i=0;i<Count;i++)
    {
        Soft_SPI_SendByte(_spi,Data[i],CS);
    }
}

/// @brief SPI接收一个字节
/// @param CS 片选
/// @return 若不启用缓冲数据，则正常返回接收数据
uint8_t Soft_SPI_ReceiveByte(void* const Parameters,uint8_t CS)
{
    GPIO_Init_Parameters* _spi = (GPIO_Init_Parameters*)Parameters;

    Soft_SPI_W_CS(_spi,0);

    uint8_t ByteReceive = 0x00;
    uint8_t i;
    for(i=0;i<8;i++)
    {
        /*
            兼容协议......
        */
        if(Soft_SPI_R_MISO(_spi)==1)
        {
            ByteReceive |= (0x80 >> i);
        }
        Soft_SPI_W_SCK(&_spi,1);
		Soft_SPI_W_SCK(&_spi,0);
    }

    Soft_SPI_W_CS(_spi,1);

    return ByteReceive;
}

void Soft_SPI_ReceiveData(void* const Parameters,uint8_t CS,uint8_t *Data,uint8_t Count)
{
    GPIO_Init_Parameters* _spi = (GPIO_Init_Parameters*)Parameters;

    uint8_t i;
    for(i=0;i<Count;i++)
    {
        *Data = Soft_SPI_ReceiveByte(_spi,CS);
    }
}


/**
  * @brief  SPI交换数据，同时收发
  * @param  SData 要写入的字节
  * @retval 无
  */
uint8_t Soft_SPI_SwapByte(void* const Parameters,uint8_t SData)
{
    GPIO_Init_Parameters* _spi = (GPIO_Init_Parameters*)Parameters;

    uint8_t i,ByteReceive = 0x00;
    for (i = 0; i < 8; i ++)						//循环8次，依次交换每一位数据
	{
		Soft_SPI_W_MOSI(_spi,SData & (0x80 >> i));		//使用掩码的方式取出ByteSend的指定一位数据并写入到MOSI线
		Soft_SPI_W_SCK(_spi,1);								//拉高SCK，上升沿移出数据
		if (Soft_SPI_R_MISO(_spi) == 1){ByteReceive |= (0x80 >> i);}	//读取MISO数据，并存储到Byte变量
																//当MISO为1时，置变量指定位为1，当MISO为0时，不做处理，指定位为默认的初值0
		Soft_SPI_W_SCK(_spi,0);								//拉低SCK，下降沿移入数据
	}
    return ByteReceive;

}
#endif
