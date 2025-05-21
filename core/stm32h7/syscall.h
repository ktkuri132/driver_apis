#ifndef __SYSCALL_H
#define __SYSCALL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stm32h7xx.h>

#define STDLIB

void Initialize_RAM_D1(void);
void SysTick_Init(void);
void SystemClock_Config();
void delay_us(uint32_t nus);
void delay_ms(uint32_t nms);
void NVIC_Configuration(void);

#ifdef __cplusplus
}
#endif

#endif