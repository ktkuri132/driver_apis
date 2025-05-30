#ifndef __F103_H
#define __F103_H

#include <sysport.h>
#ifdef STM32F10X_MD
#include <stdint.h>
#include <stddef.h>
#include <stm32f10x.h>
#include <sysport.h>
#include "f1_gpio.h"
#include "f1_i2c.h"

typedef enum{
    AF_0,AF_1,AF_2,AF_3,AF_4,AF_5,AF_6,AF_7,
    AF_8,AF_9,AF_10,AF_11,AF_12,AF_13,AF_14,AF_15
}GPIO_AF;

typedef enum{
    Pin0,Pin1,Pin2,Pin3,Pin4,Pin5,Pin6,Pin7,
    Pin8,Pin9,Pin10,Pin11,Pin12,Pin13,Pin14,Pin15
}GPIO_Pin;

typedef enum{
    Channel_1,Channel_2,Channel_3,Channel_4
}Channel;

typedef struct {
    GPIO_TypeDef *GPIOx;
    uint32_t GPIO_Pin_x;
    uint8_t GPIO_Pin_Source[16];
    uint32_t GPIO_Mode_x;
    uint32_t GPIO_PuPd_x;
    uint32_t GPIO_Speed_x;
    uint32_t GPIO_OType_xx;
    uint32_t GPIO_AF;
    uint8_t value;
}GPIO_Init_Parameters;

typedef struct{
    GPIO_TypeDef *GPIOx;
    uint32_t GPIO_Pin_x;
    uint8_t value;
}GPIO_Pin_Parameters ;

void F407_GPIO_Config(void* const Parameters);
void F407_GPIO_Pin_Set(void* const Parameters,uint8_t Pin,uint8_t status);
uint8_t F407_GPIO_Pin_Get(void* const Parameters,uint8_t Pin);

typedef struct{
    USART_TypeDef *USARTx;
    uint32_t baudrate;
    GPIO_TypeDef *GPIOx;
    uint32_t GPIO_Mode_x;
    uint32_t GPIO_PuPd_x;
    uint32_t GPIO_Speed_x;
    uint16_t GPIO_Pin_Source[16];
    uint32_t TX_Pin;
    uint32_t RX_Pin;
    uint32_t GPIO_AF;
    uint8_t DR;
}USART_Parameters;

void F407_USART_Init(void* const Parameters);
void F407_USART_SendData(void* const Parameters, uint8_t data);
void F407_USART_SendString(void* const Parameters, uint8_t *str);
uint8_t F407_USART_ReceiveData(void* const Parameters,uint8_t *data);
uint8_t USART_GETRXS(void *const Parameters);
void USART_CLEARRXS(void *const Parameters);

typedef struct{
    TIM_TypeDef* TIMx;
    uint32_t ARR;
    uint32_t PSC;
    uint32_t ClockDivision;
    uint32_t CounterMode;
    uint8_t Channel[4];
    uint8_t OCMode;
    uint32_t OutputState;
    uint32_t Pulse;
    uint32_t OCPolarity;

    uint32_t RCC_TIMBusPeriph;
    uint32_t RCC_GPIOBusPeriph;
    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pin_Source[16];
    uint32_t Output_Pin;
    uint32_t GPIO_AF;
}TIM_Parameters;


typedef struct{
    uint32_t RCC_BusPeriph;
    FunctionalState NewState;
}RCC_PeriphClock_Parameters;


void F1_PeriphClockCmd(void* const Parameters);


void F1_TIM_Init(void* const Parameters);

void F1_RNG_Init(void);
uint32_t Get_Random_Number(void);



#endif

#endif