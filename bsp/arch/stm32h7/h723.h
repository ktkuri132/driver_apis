#ifndef __H723_H
#define __H723_H

#include <sysport.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <Middlewave/sh/Serial.h>

#ifdef STM32H723xx
#include <stm32h7xx.h>
#include <stm32h723xx.h>


typedef  uint32_t u32;
typedef  uint16_t u16;
typedef  uint8_t u8;
typedef volatile uint32_t  vu32;

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

//
#define GPIO_MODE_IN    	0		
#define GPIO_MODE_OUT		1		
#define GPIO_MODE_AF		2		
#define GPIO_MODE_AIN		3		

#define GPIO_SPEED_LOW		0		
#define GPIO_SPEED_MID		1		
#define GPIO_SPEED_FAST		2		
#define GPIO_SPEED_HIGH		3		  

#define GPIO_PUPD_NONE		0		
#define GPIO_PUPD_PU		1		
#define GPIO_PUPD_PD		2		
#define GPIO_PUPD_RES		3		

#define GPIO_OTYPE_PP		0		
#define GPIO_OTYPE_OD		1		 

#define PIN0				1<<0
#define PIN1				1<<1
#define PIN2				1<<2
#define PIN3				1<<3
#define PIN4				1<<4
#define PIN5				1<<5
#define PIN6				1<<6
#define PIN7				1<<7
#define PIN8				1<<8
#define PIN9				1<<9
#define PIN10				1<<10
#define PIN11				1<<11
#define PIN12				1<<12
#define PIN13				1<<13
#define PIN14				1<<14
#define PIN15				1<<15 

typedef struct {
    GPIO_TypeDef *GPIOx;
    uint32_t GPIO_Pin_x;
    uint8_t GPIO_Pin_Source[16];
    uint32_t GPIO_Mode_x;
    uint32_t GPIO_PuPd_x;
    uint32_t GPIO_Speed_x;
    uint32_t GPIO_OType_xx;
    uint32_t GPIO_AF_x;
    uint8_t value;
}GPIO_Init_Parameters;

typedef struct{
    GPIO_TypeDef *GPIOx;
    uint32_t GPIO_Pin_x;
    uint8_t value;
}GPIO_Pin_Parameters ;



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

typedef struct {
    SPI_TypeDef *SPIx;
    GPIO_TypeDef *GPIOx;
    uint32_t SPI_BaudRatePrescaler;
    uint32_t SPI_Mode;
    uint32_t SPI_Direction;
    uint32_t SPI_DataSize;
    uint32_t SPI_CPOL;
    uint32_t SPI_CPHA;
    uint32_t SPI_NSS;
    uint32_t SPI_FirstBit;
    uint16_t GPIO_Pin_Source[16];
    uint32_t GPIO_AF;
}SPI_Parameters;

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
    RCC_TypeDef *RCCx;
    uint32_t RCC_BusPeriph;
    FunctionalState NewState;
}RCC_PeriphClock_Parameters;

// void NVIC_Init(void);

void GPIO_Set(GPIO_TypeDef* GPIOx,u32 BITx,u32 MODE,u32 OTYPE,u32 OSPEED,u32 PUPD);
void GPIO_AF_Set(GPIO_TypeDef* GPIOx,u8 BITx,u8 AFx);

void H723_PeriphClockCmd(void* const Parameters);
void H723_GPIO_Config(void* const Parameters);
void H723_GPIO_Pin_Set(void* const Parameters,uint8_t Pin,uint8_t status);
uint8_t H723_GPIO_Pin_Get(void* const Parameters,uint8_t Pin);
void H723_GPIO_AF_Config(void* const Parameters);
void H723_USART_Init(void* const Parameters);
void H723_TIM_Init(void* const Parameters);

/**
    * @brief  系统NVIC中断配置函数
    * @retval None
*/
static inline void NVIC_Init(void){
    NVIC_SetPriorityGrouping(4);	//设置中断优先级分组为4：4位抢占优先级，0位响应优先级
    
    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_EnableIRQ(USART1_IRQn);	//使能USART1中断通道
    NVIC_EnableIRQ(USART2_IRQn);	//使能USART2中断通道
    NVIC_EnableIRQ(PendSV_IRQn);

    NVIC_SetPriority(TIM2_IRQn, 2);	//设置TIM2中断优先级为0
    NVIC_SetPriority(USART1_IRQn, 1);	//设置USART1中断优先级为2
    NVIC_SetPriority(USART2_IRQn, 3);	//设置USART2中断优先级为2
    NVIC_SetPriority(PendSV_IRQn, 2);	//设置PendSV中断优先级为3
}


static inline void DELAY_ONCE(uint32_t ms){
    (void)ms;
    uint32_t  i = 0;
        for (i--; i > 1; i-=5);
}

/**
    * @brief  非阻塞延迟函数
    * @param  nus: 延迟的微秒数
    * @retval None
*/
static inline bool sleep_ms(uint32_t ms,uint64_t NowTime){
    static uint64_t start_time = 0;
    if(!start_time){
        start_time = NowTime;
    }
    if((NowTime - start_time) >= ms){
        start_time = 0;
        return true;
    }
    return false;
}


#endif

#endif

