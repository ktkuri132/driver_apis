#ifndef __SYSCALL_H
#define __SYSCALL_H

#define __MSPM0G3507__
#include <arch/mspm0g3507/mspm0.h>
#include <ti/driverlib/m0p/dl_core.h>
#include <ti/driverlib/dl_common.h>
#include "include/ti_msp_dl_config.h"

__STATIC_INLINE void MSP_NVIC_Configuration(void) {
    NVIC_SetPriorityGrouping(4);
    NVIC_SetPriority(PendSV_IRQn, 0xFF);
    NVIC_SetPriority(SysTick_IRQn, 0xFF);
    NVIC_SetPriority(UART0_INT_IRQn, 0xFF);
    NVIC_EnableIRQ(PendSV_IRQn);
    NVIC_EnableIRQ(SysTick_IRQn);
    NVIC_EnableIRQ(UART0_INT_IRQn);

}

__STATIC_INLINE void delay_ms(uint32_t cycles) {
    DL_Common_delayCycles(cycles * 16000); // 1ms = 16000 cycles
}

#endif