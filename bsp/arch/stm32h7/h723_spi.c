#include "h723.h"

#ifdef STM32H723xx


void H723_SPI_Init(void* const Parameters) {
    SPI_Parameters* SPI_Parameter = (SPI_Parameters*)Parameters;
    for (int i = 0; i < 16; i++) {
        if (SPI_Parameter->GPIO_Pin_Source[i]) {
            SPI_Parameter->GPIOx->MODER &= ~(3 << (2 * i));
            SPI_Parameter->GPIOx->MODER |= 2 << (2 * i);
            SPI_Parameter->GPIOx->OSPEEDR &= ~(3 << (2 * i));
            SPI_Parameter->GPIOx->OSPEEDR |= 3 << (2 * i);
            SPI_Parameter->GPIOx->PUPDR &= ~(3 << (2 * i));
            SPI_Parameter->GPIOx->PUPDR |= 1 << (2 * i);
            SPI_Parameter->GPIOx->AFR[i / 8] &=
                ~(0x0F << (4 * (i % 8)));
            SPI_Parameter->GPIOx->AFR[i / 8] |=
                SPI_Parameter->GPIO_AF << (4 * (i % 8));
            SPI_Parameter->GPIOx->OTYPER &= ~(1 << i);
            SPI_Parameter->GPIOx->OTYPER |= 0 << i;
        }
    }
    SPI_Parameter->SPIx->CR1 &= ~SPI_CR1_SPE; // 禁用 SPI
    SPI_Parameter->SPIx->CR1 |= SPI_Parameter->SPI_Mode; // 设置 SPI 模式
    SPI_Parameter->SPIx->CR1 |= SPI_Parameter->SPI_BaudRatePrescaler; // 设置波特率分频器
    SPI_Parameter->SPIx->CR1 |= SPI_Parameter->SPI_Direction; // 设置为主模式
    SPI_Parameter->SPIx->CR1 |= SPI_Parameter->SPI_DataSize; // 设置数据大小
    SPI_Parameter->SPIx->CR1 |= SPI_Parameter->SPI_CPOL; // 设置时钟极性
    SPI_Parameter->SPIx->CR1 |= SPI_Parameter->SPI_CPHA; // 设置时钟相位
    SPI_Parameter->SPIx->CR1 |= SPI_Parameter->SPI_NSS; // 使能软件 NSS 管脚
    SPI_Parameter->SPIx->CR1 |= SPI_Parameter->SPI_FirstBit; // 设置数据传输顺序   
    SPI_Parameter->SPIx->CR1 |= SPI_CR1_SPE; // 使能 SPI
}

void H723_SPI_SendData(void* const Parameters) {
    // SPI_Parameters* SPI_Parameter = (SPI_Parameters*)Parameters;
    // while (!(SPI_Parameter->SPIx->SR & SPI_SR_TXE)); // 等待发送缓冲区空
    // SPI_Parameter->SPIx->DR = SPI_Parameter->DR; // 发送数据
    // while (!(SPI_Parameter->SPIx->SR & SPI_SR_RXNE)); // 等待接收缓冲区非空
    // SPI_Parameter->DR = SPI_Parameter->SPIx->DR; // 接收数据
}


#endif