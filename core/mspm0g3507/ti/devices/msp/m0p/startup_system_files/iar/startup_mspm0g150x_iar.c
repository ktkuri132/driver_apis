/*****************************************************************************

  Copyright (C) 2021 Texas Instruments Incorporated - http://www.ti.com/

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

   Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the
   distribution.

   Neither the name of Texas Instruments Incorporated nor the names of
   its contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*****************************************************************************/

#include <stdint.h>

/*****************************************************************************

 Enable the IAR extensions for this source file.

*****************************************************************************/
#pragma language = extended
#pragma segment  = "CSTACK"

/*****************************************************************************

 Forward declaration of the default fault handlers.

*****************************************************************************/
extern void Reset_Handler(void);
static void Default_Handler(void);

extern void NMI_Handler(void);
extern void HardFault_Handler(void);
extern void SVC_Handler(void);
extern void PendSV_Handler(void);
extern void SysTick_Handler(void);

/* Device specific interrupt handler */
extern void GROUP0_IRQHandler(void);
extern void GROUP1_IRQHandler(void);
extern void TIMG8_IRQHandler(void);
extern void UART3_IRQHandler(void);
extern void ADC0_IRQHandler(void);
extern void ADC1_IRQHandler(void);
extern void DAC0_IRQHandler(void);
extern void SPI0_IRQHandler(void);
extern void SPI1_IRQHandler(void);
extern void UART1_IRQHandler(void);
extern void UART2_IRQHandler(void);
extern void UART0_IRQHandler(void);
extern void TIMG0_IRQHandler(void);
extern void TIMG6_IRQHandler(void);
extern void TIMA0_IRQHandler(void);
extern void TIMA1_IRQHandler(void);
extern void TIMG7_IRQHandler(void);
extern void TIMG12_IRQHandler(void);
extern void I2C0_IRQHandler(void);
extern void I2C1_IRQHandler(void);
extern void AES_IRQHandler(void);
extern void RTC_IRQHandler(void);
extern void DMA_IRQHandler(void);

/* Cortex-M0+ Processor Exceptions */
#pragma weak NMI_Handler       = Default_Handler
#pragma weak HardFault_Handler = Default_Handler
#pragma weak SVC_Handler       = Default_Handler
#pragma weak PendSV_Handler    = Default_Handler
#pragma weak SysTick_Handler   = Default_Handler

/* Device specific interrupt handler */
#pragma weak GROUP0_IRQHandler = Default_Handler
#pragma weak GROUP1_IRQHandler = Default_Handler
#pragma weak TIMG8_IRQHandler  = Default_Handler
#pragma weak UART3_IRQHandler  = Default_Handler
#pragma weak ADC0_IRQHandler   = Default_Handler
#pragma weak ADC1_IRQHandler   = Default_Handler
#pragma weak DAC0_IRQHandler   = Default_Handler
#pragma weak SPI0_IRQHandler   = Default_Handler
#pragma weak SPI1_IRQHandler   = Default_Handler
#pragma weak UART1_IRQHandler  = Default_Handler
#pragma weak UART2_IRQHandler  = Default_Handler
#pragma weak UART0_IRQHandler  = Default_Handler
#pragma weak TIMG0_IRQHandler  = Default_Handler
#pragma weak TIMG6_IRQHandler  = Default_Handler
#pragma weak TIMA0_IRQHandler  = Default_Handler
#pragma weak TIMA1_IRQHandler  = Default_Handler
#pragma weak TIMG7_IRQHandler  = Default_Handler
#pragma weak TIMG12_IRQHandler = Default_Handler
#pragma weak I2C0_IRQHandler   = Default_Handler
#pragma weak I2C1_IRQHandler   = Default_Handler
#pragma weak AES_IRQHandler    = Default_Handler
#pragma weak RTC_IRQHandler    = Default_Handler
#pragma weak DMA_IRQHandler    = Default_Handler

//*****************************************************************************
//
// The reference for the device initialization.
//
//*****************************************************************************
extern void SystemInit(void);

/*****************************************************************************

 The entry point for the application startup code.

*****************************************************************************/
extern void __iar_program_start(void);

/*****************************************************************************

 A union that describes the entries of the vector table.  The union is needed
 since the first entry is the stack pointer and the remainder are function
 pointers.

*****************************************************************************/
typedef union {
    void (*handler)(void);
    uint32_t ptr;
} uVectorEntry;

/*****************************************************************************

 The vector table.  Note that the proper constructs must be placed on this to
 ensure that it ends up at physical address 0x0000.0000.

*****************************************************************************/
__root const uVectorEntry __vector_table[] @ ".intvec" = {
    {.ptr = (uint32_t) __sfe("CSTACK")}, /* The initial stack pointer */
    Reset_Handler,                       /* The reset handler         */
    NMI_Handler,                         /* The NMI handler           */
    HardFault_Handler,                   /* The hard fault handler    */
    0,                                   /* Reserved                  */
    0,                                   /* Reserved                  */
    0,                                   /* Reserved                  */
    0,                                   /* Reserved                  */
    0,                                   /* Reserved                  */
    0,                                   /* Reserved                  */
    0,                                   /* Reserved                  */
    SVC_Handler,                         /* SVCall handler            */
    0,                                   /* Reserved                  */
    0,                                   /* Reserved                  */
    PendSV_Handler,                      /* The PendSV handler        */
    SysTick_Handler,                     /* The SysTick handler       */
    GROUP0_IRQHandler,                   /* GROUP0 interrupt handler  */
    GROUP1_IRQHandler,                   /* GROUP1 interrupt handler  */
    TIMG8_IRQHandler,                    /* TIMG8 interrupt handler   */
    UART3_IRQHandler,                    /* UART3 interrupt handler   */
    ADC0_IRQHandler,                     /* ADC0 interrupt handler    */
    ADC1_IRQHandler,                     /* ADC1 interrupt handler    */
    0,                                   /* Reserved                  */
    DAC0_IRQHandler,                     /* DAC0 interrupt handler    */
    0,                                   /* Reserved                  */
    SPI0_IRQHandler,                     /* SPI0 interrupt handler    */
    SPI1_IRQHandler,                     /* SPI1 interrupt handler    */
    0,                                   /* Reserved                  */
    0,                                   /* Reserved                  */
    UART1_IRQHandler,                    /* UART1 interrupt handler   */
    UART2_IRQHandler,                    /* UART2 interrupt handler   */
    UART0_IRQHandler,                    /* UART0 interrupt handler   */
    TIMG0_IRQHandler,                    /* TIMG0 interrupt handler   */
    TIMG6_IRQHandler,                    /* TIMG6 interrupt handler   */
    TIMA0_IRQHandler,                    /* TIMA0 interrupt handler   */
    TIMA1_IRQHandler,                    /* TIMA1 interrupt handler   */
    TIMG7_IRQHandler,                    /* TIMG7 interrupt handler   */
    TIMG12_IRQHandler,                   /* TIMG12 interrupt handler  */
    0,                                   /* Reserved                  */
    0,                                   /* Reserved                  */
    I2C0_IRQHandler,                     /* I2C0 interrupt handler    */
    I2C1_IRQHandler,                     /* I2C1 interrupt handler    */
    0,                                   /* Reserved                  */
    0,                                   /* Reserved                  */
    AES_IRQHandler,                      /* AES interrupt handler     */
    0,                                   /* Reserved                  */
    RTC_IRQHandler,                      /* RTC interrupt handler     */
    DMA_IRQHandler                       /* DMA interrupt handler     */
};

/*****************************************************************************

 This is the code that gets called when the processor first starts execution
 following a reset event.  Only the absolutely necessary set is performed,
 after which the application supplied entry() routine is called.  Any fancy
 actions (such as making decisions based on the reset cause register, and
 resetting the bits in that register) are left solely in the hands of the
 application.

*****************************************************************************/
__weak void Reset_Handler(void)
{
    /* Initialize the device */
    // SystemInit();

    /* Call the application's entry point. */
    __iar_program_start();
}

/*****************************************************************************

 This is the code that gets called when the processor receives an unexpected
 interrupt.  This simply enters an infinite loop, preserving the system state
 for examination by a debugger.

*****************************************************************************/
static void Default_Handler(void)
{
    /* Go into an infinite loop. */
    while (1) {
    }
}
