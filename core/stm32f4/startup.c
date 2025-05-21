/*
  ******************************************************************************
  * @file      startup_stm32f407xx.c
  * @author    MCD Application Team
  * @brief     STM32F407xx Devices vector table for GCC based toolchains. 
  *            This module performs:
  *                - Set the initial SP
  *                - Set the initial PC == Reset_Handler,
  *                - Set the vector table entries with the exceptions ISR address
  *                - Branches to main in the C library (which eventually
  *                  calls main()).
  *            After Reset the Cortex-M4 processor is in Thread mode,
  *            priority is Privileged, and the Stack is set to Main.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
*/

#include <stdint.h>
#include "syscall.h"
/* External symbols defined in linker script */
extern void *_estack;
extern unsigned int _sidata;  /* Start address for the initialization values of the .data section */
extern unsigned int _sdata;   /* Start address for the .data section */
extern unsigned int _edata;   /* End address for the .data section */
extern unsigned int _sbss;    /* Start address for the .bss section */
extern unsigned int _ebss;    /* End address for the .bss section */

/* Function prototypes */
void Reset_Handler(void);
void Default_Handler(void);
void SystemInit(void);
void __libc_init_array(void);

/* Weak interrupt handlers */
__attribute__ ((weak, alias ("Default_Handler"))) void NMI_Handler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void HardFault_Handler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void MemManage_Handler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void BusFault_Handler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void UsageFault_Handler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void SVC_Handler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DebugMon_Handler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void PendSV_Handler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void SysTick_Handler(void);

/* External Interrupts */
__attribute__ ((weak, alias ("Default_Handler"))) void WWDG_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void PVD_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void TAMP_STAMP_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void RTC_WKUP_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void FLASH_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void RCC_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void EXTI0_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void EXTI1_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void EXTI2_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void EXTI3_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void EXTI4_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA1_Stream0_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA1_Stream1_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA1_Stream2_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA1_Stream3_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA1_Stream4_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA1_Stream5_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA1_Stream6_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void ADC_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void CAN1_TX_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void CAN1_RX0_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void CAN1_RX1_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void CAN1_SCE_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void EXTI9_5_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void TIM1_BRK_TIM9_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void TIM1_UP_TIM10_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void TIM1_TRG_COM_TIM11_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void TIM1_CC_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void TIM2_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void TIM3_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void TIM4_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void I2C1_EV_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void I2C1_ER_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void I2C2_EV_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void I2C2_ER_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void SPI1_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void SPI2_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void USART1_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void USART2_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void USART3_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void EXTI15_10_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void RTC_Alarm_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void OTG_FS_WKUP_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void TIM8_BRK_TIM12_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void TIM8_UP_TIM13_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void TIM8_TRG_COM_TIM14_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void TIM8_CC_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA1_Stream7_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void FSMC_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void SDIO_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void TIM5_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void SPI3_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void UART4_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void UART5_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void TIM6_DAC_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void TIM7_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA2_Stream0_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA2_Stream1_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA2_Stream2_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA2_Stream3_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA2_Stream4_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void ETH_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void ETH_WKUP_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void CAN2_TX_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void CAN2_RX0_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void CAN2_RX1_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void CAN2_SCE_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void OTG_FS_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA2_Stream5_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA2_Stream6_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DMA2_Stream7_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void USART6_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void I2C3_EV_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void I2C3_ER_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void OTG_HS_EP1_OUT_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void OTG_HS_EP1_IN_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void OTG_HS_WKUP_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void OTG_HS_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void DCMI_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void HASH_RNG_IRQHandler(void);
__attribute__ ((weak, alias ("Default_Handler"))) void FPU_IRQHandler(void);

/* Vector table */
void (* const g_pfnVectors[])(void) __attribute__ ((section(".isr_vector"))) = {
    (void *)&_estack,            /* Initial stack pointer */
    Reset_Handler,               /* Reset Handler */
    NMI_Handler,                 /* NMI Handler */
    HardFault_Handler,           /* Hard Fault Handler */
    MemManage_Handler,           /* MPU Fault Handler */
    BusFault_Handler,            /* Bus Fault Handler */
    UsageFault_Handler,          /* Usage Fault Handler */
    0,                           /* Reserved */
    0,                           /* Reserved */
    0,                           /* Reserved */
    0,                           /* Reserved */
    SVC_Handler,                 /* SVCall Handler */
    DebugMon_Handler,            /* Debug Monitor Handler */
    0,                           /* Reserved */
    PendSV_Handler,              /* PendSV Handler */
    SysTick_Handler,             /* SysTick Handler */

    /* External Interrupts */
    WWDG_IRQHandler,             /* Window WatchDog */
    PVD_IRQHandler,              /* PVD through EXTI Line detection */
    TAMP_STAMP_IRQHandler,       /* Tamper and TimeStamps through the EXTI line */
    RTC_WKUP_IRQHandler,         /* RTC Wakeup through the EXTI line */
    FLASH_IRQHandler,            /* FLASH */
    RCC_IRQHandler,              /* RCC */
    EXTI0_IRQHandler,            /* EXTI Line0 */
    EXTI1_IRQHandler,            /* EXTI Line1 */
    EXTI2_IRQHandler,            /* EXTI Line2 */
    EXTI3_IRQHandler,            /* EXTI Line3 */
    EXTI4_IRQHandler,            /* EXTI Line4 */
    DMA1_Stream0_IRQHandler,     /* DMA1 Stream 0 */
    DMA1_Stream1_IRQHandler,     /* DMA1 Stream 1 */
    DMA1_Stream2_IRQHandler,     /* DMA1 Stream 2 */
    DMA1_Stream3_IRQHandler,     /* DMA1 Stream 3 */
    DMA1_Stream4_IRQHandler,     /* DMA1 Stream 4 */
    DMA1_Stream5_IRQHandler,     /* DMA1 Stream 5 */
    DMA1_Stream6_IRQHandler,     /* DMA1 Stream 6 */
    ADC_IRQHandler,              /* ADC1, ADC2 and ADC3s */
    CAN1_TX_IRQHandler,          /* CAN1 TX */
    CAN1_RX0_IRQHandler,         /* CAN1 RX0 */
    CAN1_RX1_IRQHandler,         /* CAN1 RX1 */
    CAN1_SCE_IRQHandler,         /* CAN1 SCE */
    EXTI9_5_IRQHandler,          /* External Line[9:5]s */
    TIM1_BRK_TIM9_IRQHandler,    /* TIM1 Break and TIM9 */
    TIM1_UP_TIM10_IRQHandler,    /* TIM1 Update and TIM10 */
    TIM1_TRG_COM_TIM11_IRQHandler, /* TIM1 Trigger and Commutation and TIM11 */
    TIM1_CC_IRQHandler,          /* TIM1 Capture Compare */
    TIM2_IRQHandler,             /* TIM2 */
    TIM3_IRQHandler,             /* TIM3 */
    TIM4_IRQHandler,             /* TIM4 */
    I2C1_EV_IRQHandler,          /* I2C1 Event */
    I2C1_ER_IRQHandler,          /* I2C1 Error */
    I2C2_EV_IRQHandler,          /* I2C2 Event */
    I2C2_ER_IRQHandler,          /* I2C2 Error */
    SPI1_IRQHandler,             /* SPI1 */
    SPI2_IRQHandler,             /* SPI2 */
    USART1_IRQHandler,           /* USART1 */
    USART2_IRQHandler,           /* USART2 */
    USART3_IRQHandler,           /* USART3 */
    EXTI15_10_IRQHandler,        /* External Line[15:10]s */
    RTC_Alarm_IRQHandler,        /* RTC Alarm (A and B) through EXTI Line */
    OTG_FS_WKUP_IRQHandler,      /* USB OTG FS Wakeup through EXTI line */
    TIM8_BRK_TIM12_IRQHandler,   /* TIM8 Break and TIM12 */
    TIM8_UP_TIM13_IRQHandler,    /* TIM8 Update and TIM13 */
    TIM8_TRG_COM_TIM14_IRQHandler, /* TIM8 Trigger and Commutation and TIM14 */
    TIM8_CC_IRQHandler,          /* TIM8 Capture Compare */
    DMA1_Stream7_IRQHandler,     /* DMA1 Stream7 */
    FSMC_IRQHandler,             /* FSMC */
    SDIO_IRQHandler,             /* SDIO */
    TIM5_IRQHandler,             /* TIM5 */
    SPI3_IRQHandler,             /* SPI3 */
    UART4_IRQHandler,            /* UART4 */
    UART5_IRQHandler,            /* UART5 */
    TIM6_DAC_IRQHandler,         /* TIM6 and DAC1&2 underrun errors */
    TIM7_IRQHandler,             /* TIM7 */
    DMA2_Stream0_IRQHandler,     /* DMA2 Stream 0 */
    DMA2_Stream1_IRQHandler,     /* DMA2 Stream 1 */
    DMA2_Stream2_IRQHandler,     /* DMA2 Stream 2 */
    DMA2_Stream3_IRQHandler,     /* DMA2 Stream 3 */
    DMA2_Stream4_IRQHandler,     /* DMA2 Stream 4 */
    ETH_IRQHandler,              /* Ethernet */
    ETH_WKUP_IRQHandler,         /* Ethernet Wakeup through EXTI line */
    CAN2_TX_IRQHandler,          /* CAN2 TX */
    CAN2_RX0_IRQHandler,         /* CAN2 RX0 */
    CAN2_RX1_IRQHandler,         /* CAN2 RX1 */
    CAN2_SCE_IRQHandler,         /* CAN2 SCE */
    OTG_FS_IRQHandler,           /* USB OTG FS */
    DMA2_Stream5_IRQHandler,     /* DMA2 Stream 5 */
    DMA2_Stream6_IRQHandler,     /* DMA2 Stream 6 */
    DMA2_Stream7_IRQHandler,     /* DMA2 Stream 7 */
    USART6_IRQHandler,           /* USART6 */
    I2C3_EV_IRQHandler,          /* I2C3 event */
    I2C3_ER_IRQHandler,          /* I2C3 error */
    OTG_HS_EP1_OUT_IRQHandler,   /* USB OTG HS End Point 1 Out */
    OTG_HS_EP1_IN_IRQHandler,    /* USB OTG HS End Point 1 In */
    OTG_HS_WKUP_IRQHandler,      /* USB OTG HS Wakeup through EXTI */
    OTG_HS_IRQHandler,           /* USB OTG HS */
    DCMI_IRQHandler,             /* DCMI */
    0,                           /* CRYP crypto */
    HASH_RNG_IRQHandler,         /* Hash and Rng */
    FPU_IRQHandler               /* FPU */
};

/* Reset handler */
void Reset_Handler(void) {
    /* Set stack pointer */
    __asm volatile ("ldr sp, =_estack");

    /* Call the clock system initialization function */
    SystemInit();

    /* Copy the data segment initializers from flash to SRAM */
    unsigned int *pSrc = &_sidata;
    unsigned int *pDest = &_sdata;
    while (pDest < &_edata) {
        *pDest++ = *pSrc++;
    }

    /* Zero fill the bss segment */
    pDest = &_sbss;
    while (pDest < &_ebss) {
        *pDest++ = 0;
    }

    /* Call static constructors */
    __libc_init_array();

    /* Call the application's entry point */
    main();

    /* Infinite loop if main() returns */
    while (1);
}

/* Default handler for unhandled interrupts */
void Default_Handler(void) {
    /* Infinite loop */
    while (1){
        
    }
}