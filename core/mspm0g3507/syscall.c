#include <sysport.h>
#include <middlewave/sh/Serial.h>
#include <middlewave/sh/shell.h>
#include <ti/devices/msp/m0p/mspm0g350x.h>
#include <include/env.h>


#define SystemCoreClock CPUCLK_FREQ

DeviceFamily MSP_Family = {
    .Architecture = "Cotex-M0+",
    .User = "ktkuri",
    .Password = NULL,
    .DeviceName = "LaunchPad",
    .OS = NULL,
    .Device = "MSPM0G3507",
    .Version = "1.0.0"
};

/**
 * @brief  SysTick定时器中断初始化函数
 * @retval None
 */
void MSP_SysTick_Init(void) {
    /* Configure the SysTick to have an interrupt in 1ms time basis */
    SysTick->LOAD  = (SystemCoreClock / 1000) - 1; /* 设置重装寄存器 */
    NVIC_SetPriority(SysTick_IRQn, 3);               /* 设置Systick定时器中断优先级 */
    NVIC_EnableIRQ(SysTick_IRQn);                  /* 设置SysTick定时器中断使能 */
    SysTick->VAL   = 0;                            /* 装载SysTick定时器的初始值 */
    /* 配置SysTick时钟源  使能SysTick中断   开始计时  */
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}
