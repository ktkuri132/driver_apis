#ifndef __RT1052_H
#define __RT1052_H

#include <Middlewave/sh/Serial.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sysport.h>


#ifdef CPU_MIMXRT1052DVL6B
#include <MIMXRT1052.h>

/*
    RT1052 默认状态下是开启了所有的外设时钟的，复位状态下全是11
*/

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef volatile uint32_t vu32;

typedef enum {
    AF_0,
    AF_1,
    AF_2,
    AF_3,
    AF_4,
    AF_5,
    AF_6,
    AF_7,
    AF_8,
    AF_9,
    AF_10,
    AF_11,
    AF_12,
    AF_13,
    AF_14,
    AF_15

} GPIO_AF;

typedef enum {
    Pin0,
    Pin1,
    Pin2,
    Pin3,
    Pin4,
    Pin5,
    Pin6,
    Pin7,
    Pin8,
    Pin9,
    Pin10,
    Pin11,
    Pin12,
    Pin13,
    Pin14,
    Pin15
} GPIO_Pin;

typedef enum { Channel_1, Channel_2, Channel_3, Channel_4 } Channel;

typedef enum { DISABLE, ENABLE } FunctionalState;

//
#define GPIO_MODE_IN 0
#define GPIO_MODE_OUT 1
#define GPIO_MODE_AF 2
#define GPIO_MODE_AIN 3

#define GPIO_SPEED_LOW 0
#define GPIO_SPEED_MID 1
#define GPIO_SPEED_FAST 2
#define GPIO_SPEED_HIGH 3

#define GPIO_PUPD_NONE 0
#define GPIO_PUPD_PU 1
#define GPIO_PUPD_PD 2
#define GPIO_PUPD_RES 3

#define GPIO_OTYPE_PP 0
#define GPIO_OTYPE_OD 1

#define PIN0 1 << 0
#define PIN1 1 << 1
#define PIN2 1 << 2
#define PIN3 1 << 3
#define PIN4 1 << 4
#define PIN5 1 << 5
#define PIN6 1 << 6
#define PIN7 1 << 7
#define PIN8 1 << 8
#define PIN9 1 << 9
#define PIN10 1 << 10
#define PIN11 1 << 11
#define PIN12 1 << 12
#define PIN13 1 << 13
#define PIN14 1 << 14
#define PIN15 1 << 15

typedef struct {
    GPIO_Type *GPIOx;
    uint32_t GPIO_Pin_x;
    uint8_t GPIO_Pin_Source[32];
    uint32_t GPIO_Mode_x;
    uint32_t GPIO_PuPd_x;
    uint32_t GPIO_Speed_x;
    uint32_t GPIO_OType_xx;
    uint32_t GPIO_AF_x;
    uint8_t value;
} GPIO_Init_Parameters;

typedef struct {
    GPIO_Type *GPIOx;
    uint32_t GPIO_Pin_x;
    uint8_t value;
} GPIO_Pin_Parameters;

void RT_GPIO_Config(void *const Parameters);
void RT_GPIO_Pin_Set(void *const Parameters, uint8_t Pin, uint8_t status);
uint8_t RT_GPIO_Pin_Get(void *const Parameters, uint8_t Pin);

typedef struct {
    LPUART_Type *USARTx;
    uint32_t baudrate;
    GPIO_Type *GPIOx;
    uint32_t GPIO_Mode_x;
    uint32_t GPIO_PuPd_x;
    uint32_t GPIO_Speed_x;
    uint16_t GPIO_Pin_Source[32];
    uint32_t TX_Pin;
    uint32_t RX_Pin;
    uint32_t GPIO_AF;
    uint8_t DR;
} USART_Parameters;

#define uart_st USART_Parameters

void RT_UART_Init(void *const Parameters);
void RT_USART_SendData(void *const Parameters, uint8_t data);
void RT_USART_SendString(void *const Parameters, uint8_t *str);
uint8_t RT_USART_ReceiveData(void *const Parameters, uint8_t *data);
uint8_t USART_GETRXS(void *const Parameters);

typedef struct {
    LPSPI_Type *SPIx;
    GPIO_Type *GPIOx;
    uint32_t SPI_BaudRatePrescaler;
    uint32_t SPI_Mode;
    uint32_t SPI_Direction;
    uint32_t SPI_DataSize;
    uint32_t SPI_CPOL;
    uint32_t SPI_CPHA;
    uint32_t SPI_NSS;
    uint32_t SPI_FirstBit;
    uint16_t GPIO_Pin_Source[32];
    uint32_t GPIO_AF;
} SPI_Parameters;

typedef struct {
    TMR_Type *TIMx;
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
    GPIO_Type *GPIOx;
    uint16_t GPIO_Pin_Source[32];
    uint32_t Output_Pin;
    uint32_t GPIO_AF;
} TIM_Parameters;

typedef struct {
    CCM_Type *RCCx;
    uint32_t RCC_BusPeriph;
    FunctionalState NewState;
} RCC_PeriphClock_Parameters;

void RT_TIM_Init(void *const Parameters);

static inline void DELAY_ONCE(uint32_t ms) {
    (void)ms;
    uint32_t i = 0;
    for (i--; i > 1; i -= 5);
}

/**
 * @brief  非阻塞延迟函数
 * @param  nus: 延迟的微秒数
 * @retval None
 */
static inline bool sleep_ms(uint32_t ms, uint64_t NowTime) {
    static uint64_t start_time = 0;
    if (!start_time) {
        start_time = NowTime;
    }
    if ((NowTime - start_time) >= ms) {
        start_time = 0;
        return true;
    }
    return false;
}

#endif

#endif