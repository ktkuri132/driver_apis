#ifndef __SYSCALL_H
#define __SYSCALL_H

#define __MSPM0G3507__
#include <arch/mspm0g3507/mspm0.h>
#include <ti/driverlib/m0p/dl_core.h>
#include <ti/driverlib/dl_common.h>
#include "include/ti_msp_dl_config.h"

__STATIC_INLINE void MSP_NVIC_Configuration(void) {
    NVIC_SetPriorityGrouping(4);
    // NVIC_SetPriority(PendSV_IRQn, 3);
    // NVIC_SetPriority(SysTick_IRQn, 3);
    // NVIC_SetPriority(UART0_INT_IRQn, 2);
    // NVIC_SetPriority(UART1_INT_IRQn, 1);
    // NVIC_SetPriority(UART2_INT_IRQn, 2);
    // NVIC_SetPriority(UART3_INT_IRQn, 2);
    // NVIC_SetPriority(TIMA0_INT_IRQn, 1);
    NVIC_EnableIRQ(PendSV_IRQn);
    NVIC_EnableIRQ(SysTick_IRQn);
    NVIC_EnableIRQ(UART0_INT_IRQn);
    // NVIC_EnableIRQ(UART1_INT_IRQn);
    // NVIC_EnableIRQ(UART2_INT_IRQn);
    // NVIC_EnableIRQ(UART3_INT_IRQn);
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);

}

__STATIC_INLINE void delay_ms(uint32_t cycles) {
    DL_Common_delayCycles(cycles * 16000); // 1ms = 16000 cycles
}

void MSP_SysTick_Init(void);

#endif