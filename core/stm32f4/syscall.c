#include "stm32f4xx.h"
#include <stdio.h>
// #include <sysport.h>

/**
    * @brief  系统NVIC中断配置函数
    * @retval None
*/
void NVIC_Init(void){
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

/**
 * @brief  SysTick定时器中断初始化函数
 * @retval None
 */
 void SysTick_Init(void) {
    /* Configure the SysTick to have an interrupt in 1ms time basis */
    SysTick->LOAD  = (SystemCoreClock / 1000) - 1; /* 设置重装寄存器 */
    NVIC_SetPriority(SysTick_IRQn, 3);               /* 设置Systick定时器中断优先级 */
    NVIC_EnableIRQ(SysTick_IRQn);                  /* 设置SysTick定时器中断使能 */
    SysTick->VAL   = 0;                            /* 装载SysTick定时器的初始值 */
    /* 配置SysTick时钟源  使能SysTick中断   开始计时  */
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

void delay_us(uint32_t nus){
    u32 temp;	    	 
    SysTick->LOAD = (nus * (SystemCoreClock / 1000000)) - 1;
	SysTick->VAL=0x00;        				//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ; //开始倒数 	 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));	//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; //关闭计数器
	SysTick->VAL =0X00;       				//清空计数器 
}

void delay_ms(uint32_t nms){
    uint32_t i;
    for (i = 0; i < nms; i++) {
        delay_us(100);
    } 
}

