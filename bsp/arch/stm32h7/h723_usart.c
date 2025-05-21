
#include "h723.h"
#ifdef STM32H723xx
#include <stdint.h>
#include <stm32h7xx.h>

void H723_USART_Init(void* const Parameters) {

    uint16_t CLK = 270;  

    USART_Parameters* USART_Parameter = (USART_Parameters*)Parameters;

    for (int i = 0; (i < 16)&&(USART_Parameter->GPIO_Pin_Source[i]); i++) {
        USART_Parameter->GPIOx->MODER &= ~(3 << (2 * USART_Parameter->GPIO_Pin_Source[i]));
        USART_Parameter->GPIOx->MODER |= 2 << (2 * USART_Parameter->GPIO_Pin_Source[i]);
        USART_Parameter->GPIOx->OSPEEDR &= ~(3 << (2 * USART_Parameter->GPIO_Pin_Source[i]));
        USART_Parameter->GPIOx->OSPEEDR |= 3 << (2 * USART_Parameter->GPIO_Pin_Source[i]);
        USART_Parameter->GPIOx->PUPDR &= ~(3 << (2 * USART_Parameter->GPIO_Pin_Source[i]));
        USART_Parameter->GPIOx->PUPDR |= 1 << (2 * USART_Parameter->GPIO_Pin_Source[i]);
        USART_Parameter->GPIOx->AFR[USART_Parameter->GPIO_Pin_Source[i] / 8] &=
            ~(0x0F << (4 * (USART_Parameter->GPIO_Pin_Source[i] % 8)));
        USART_Parameter->GPIOx->AFR[USART_Parameter->GPIO_Pin_Source[i] / 8] |=
            USART_Parameter->GPIO_AF << (4 * (USART_Parameter->GPIO_Pin_Source[i] % 8));
        USART_Parameter->GPIOx->OTYPER &= ~(1 << USART_Parameter->GPIO_Pin_Source[i]);
        USART_Parameter->GPIOx->OTYPER |= 0 << USART_Parameter->GPIO_Pin_Source[i];
    }
    uint32_t temp;
    temp = (CLK * 1000000 + (USART_Parameter->baudrate) / 2) / (USART_Parameter->baudrate);

    USART_Parameter->USARTx->BRR = temp;
    // 使能串口
    USART_Parameter->USARTx->CR1 |= USART_CR1_UE;
    // 数据位8位
    USART_Parameter->USARTx->CR1 &= ~USART_CR1_M;
    // 禁止奇偶校验
    USART_Parameter->USARTx->CR1 &= ~USART_CR1_PCE;
    // 使能接收中断
    USART_Parameter->USARTx->CR1 |= USART_CR1_RXNEIE;
    USART_Parameter->USARTx->CR3 |= USART_CR3_OVRDIS;
    
    // 设置一个停止位
    USART_Parameter->USARTx->CR2 &= ~USART_CR2_STOP;
    // 使能发送和接收
    USART_Parameter->USARTx->CR1 |= USART_CR1_TE | USART_CR1_RE;
    
    
}

void H723_USART_SendData(USART_TypeDef* USARTx, uint8_t data) {
    while (!(USARTx->ISR & USART_ISR_TXE));  // 等待发送缓冲区空
    USARTx->TDR = data;                      // 发送数据
}

void H723_USART_SendString(USART_TypeDef* USARTx, const char* str) {
    while (*str) {
        H723_USART_SendData(USARTx, *str++);
    }
}

#endif