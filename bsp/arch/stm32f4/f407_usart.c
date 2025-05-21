#include "f407.h"

#ifdef STM32F40_41xxx
#include <stdint.h>


uint8_t USART_GETRXS(void *const Parameters) {
    uart_st* uart_p = (uart_st*)Parameters;
    if(uart_p->USARTx->SR & USART_SR_RXNE) {  // 判断接收中断
        return 1;  // 接收中断
    } else {
        return 0;  // 非接收中断
    }
}

void USART_CLEARRXS(void *const Parameters) {
    uart_st* uart_p = (uart_st*)Parameters;
    uart_p->USARTx->SR &= ~USART_SR_RXNE;  // 清除接收中断标志
}

uint8_t USART_GETTXS(){
    uint8_t TXE = 0;
    return TXE;
}

uint8_t USART_GETRE(){
    uint8_t RE = 0;
    return RE;
}

void F407_USART_Init(void* const Parameters) {
    uint16_t CLK = 0;  

    
    USART_Parameters* USART_Parameter = (USART_Parameters*)Parameters;

    if(USART_Parameter->USARTx == USART1){
        CLK = 168/2;
    } else if(USART_Parameter->USARTx == USART2){
        CLK = 168/4;
    } else if(USART_Parameter->USARTx == USART3){
        CLK = 168/4;
    } else if(USART_Parameter->USARTx == UART4){
        CLK = 168/4;
    } else if(USART_Parameter->USARTx == UART5){
        CLK = 168/4;
    } else if(USART_Parameter->USARTx == USART6){
        CLK = 168/2;
    } else if(USART_Parameter->USARTx == UART7){
        CLK = 168/4;
    } else if(USART_Parameter->USARTx == UART8){
        CLK = 168/4;
    }

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
    // 设置一个停止位
    USART_Parameter->USARTx->CR2 &= ~USART_CR2_STOP;
    // 使能发送和接收
    USART_Parameter->USARTx->CR1 |= USART_CR1_TE | USART_CR1_RE;
}

void F407_USART_SendData(void* const Parameters, uint8_t data){
    uart_st *uart_p = (uart_st *)Parameters;
   while (!(uart_p->USARTx->SR & USART_SR_TXE));  // 等待发送缓冲区空
    uart_p->USARTx->DR = data;  // 发送数据
}

void F407_USART_SendString(void* const Parameters, uint8_t *str) {
    uart_st *uart_p = (uart_st *)Parameters;
    while (*str) {
        F407_USART_SendData(uart_p, *str++);
    }
}

uint8_t F407_USART_ReceiveData(void* const Parameters,uint8_t *data) {
    uart_st *uart_p = (uart_st *)Parameters;
    uint8_t _data = 0;
    while (!(uart_p->USARTx->SR & USART_SR_RXNE));  // 等待接收缓冲区非空
    _data = uart_p->USARTx->DR;   
    if(data != NULL) {
        *data = _data;  // 将接收到的数据存储到data中
    }
    return *data;
}

#endif