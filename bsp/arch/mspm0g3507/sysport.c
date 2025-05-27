#include <errno.h>
#include "sysport.h"
#include <stdint.h>
#include <Middlewave/sh/Serial.h>
#include <Middlewave/sh/shell.h>
#include <include/ti_msp_dl_config.h>
#include <syscall.h>
#define USART
// #define TIM
// #define I2C
// #define SPI

SYS_Port *sys_port;

/* 项目环境变量声明  */
extern EnvVar MyEnv[];


/**
    * @brief  系统接口初始化函数
    * @param  None
 */
SYS_Port* SysPort_Init() {
    static SYS_Port port = {
        .System_Init = SYSCFG_DL_init,
        .SysTick_Init = MSP_SysTick_Init,
        .syscall_port = {
          .bsp_systick_delay_us = DL_Common_delayCycles,
          .bsp_systick_delay_ms = delay_ms,
          .NVIC_Configuration = MSP_NVIC_Configuration,
        },
        .rcc_periph_port = {
            .bsp_rcc_periph_clock = NULL,
        },
        .gpio_port = {
            .bsp_gpio_init = NULL,
            .bsp_gpio_af_set = NULL,
            .bsp_gpio_pin_set = MSP_GPIO_setPin,
           .bsp_gpio_pin_get = MSP_GPIO_getPin,
            .GetGPIOPeriphClock = NULL,
        },
        #ifdef I2C
        .i2c_port = {
            .bsp_IIC_Init = NULL,
            .bsp_IIC_Start = NULL,
           .bsp_IIC_Stop = NULL,
           .bsp_IIC_Wait_Ack = NULL,
           .bsp_IIC_Ack = NULL,
        },
        #endif
        #ifdef SPI
        .spi_port = {
            .bsp_SPI_Init = NULL,
            .bsp_SPI_SendByte = NULL,
            .bsp_SPI_SendData = NULL,
            .bsp_SPI_ReceiveByte = NULL,
            .bsp_SPI_ReceiveData = NULL,
            .bsp_SPI_SwapByte = NULL,
        },

        #endif
        #ifdef USART    
        .usart_port = {
            .bsp_usart_x_inti = NULL,
            .bsp_usart_x_send_byte = NULL,
            .bsp_usart_x_send_string = MSP_SendString,
            .bsp_usart_x_receive = MSP_ReceiveData,
            .bsp_get_rxs = NULL,
        },
        #endif
        #ifdef TIM
        .tim_port = {
            .bsp_tim_x_inti = NULL,
            .bsp_tim_x_start = NULL,
        }
        #endif
    };
    sys_port = &port; // 将系统接口指针指向静态变量
    return &port;
}




/// 任务切换函数
/// @param userEnv: 用户环境变量数组
void Task_Switch(EnvVar *userEnv) {
    // 假如环境变量过长可采取其他的查找算法:如二分查找等
    // 这里采用线性查找
    int i;
    for (i = 0; userEnv[i].name != NULL; i++) {
        if(userEnv[i].RunStae){
            // 执行命令
            sfp.syspfunc = userEnv[i].callback;  // 设置系统函数指针
            sfp.argc = userEnv[i].argc;  // 设置参数个数
            sfp.Parameters = userEnv[i].arg;  // 设置参数
            userEnv[i].RunStae = 0;  // 重置运行状态
            return;  // 跳出循环，避免重复执行
        }
    }
    i = 0;  // 重置循环变量
}

void PendSV_Handler(){
    Task_Switch(MyEnv); // 执行任务切换
}

void SysTick_Handler(void) {
    srt.SysRunTime++;
    if (srt.SysRunTime % 1000 == 0) {
        srt.SysRunTimeSec++;
        if (srt.SysRunTimeSec % 60 == 0) {
            srt.SysRunTimeMin++;
        }
    }
}

/**
 *
 * 不使用标准库的重定向
 * @}
 */

extern struct USART_Parameters *usart_1;
#ifndef STDLIB

#if defined ( __CC_ARM )

#pragma import(__use_no_semihosting)                             
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
   
void _sys_exit(int x) 
{ 
	x = x; 
} 
 
int fputc(int ch, FILE *f)
{ 	
	(void)f;
    usart.bsp_usart_x_send_string(usart_1,(uint8_t*)&ch); // 发送数据
    return ch;
}

int fgetc(FILE *f) {
    (void)f;
    int ch = usart.bsp_usart_x_receive(usart_1,NULL); // 接收数据
    return ch;
}

/*
 * Arm Compiler 6 (armclang)
 */
#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)

__asm (".global __use_no_semihosting\n\t");

void _sys_exit(int x) 
{ 
	x = x; 
} 

int fputc(int ch, FILE *f)
{ 	
	(void)f;
    usart.bsp_usart_x_send_string(UART_0_INST,(uint8_t*)&ch); // 发送数据
    return ch;
}

int fgetc(FILE *f) {
    (void)f;
    int ch = usart.bsp_usart_x_receive(usart_1,NULL); // 接收数据
    return ch;
}

/*
 * GNU Compiler
 */
#elif defined ( __GNUC__ )

int _write(int file, char *ptr, int len) {
    (void)file;
    int i = 0;
    for (i = 0; i < len; i++) {
        usart.bsp_usart_x_send_byte(usart_1, (char)(*ptr++)); // 发送数据
    }
    return len;
}

int _read(int file, char *ptr, int len) {
    (void)file;
    int DataIdx;

    for (DataIdx = 0; DataIdx < len; DataIdx++) {
        *ptr++ = usart.bsp_usart_x_receive(usart_1,NULL); // 接收数据
    }
    return len;
}


int __attribute__((__weak__)) _kill(int pid, int sig) {
    (void)pid;
    (void)sig;
    return -1;
}

void __attribute__((__weak__)) _exit(int status) {
    _kill(status, -1);
    while (1) {
    } /* Make sure we hang here */
}

int __attribute__((__weak__)) _close(int file) {
    (void)file;
    return -1;
}

int __attribute__((__weak__)) _fstat(int file, struct stat *st) {
    (void)file;
    return 0;
}

int __attribute__((__weak__)) _isatty(int file) {
    (void)file;
    return 1;
}

int __attribute__((__weak__)) _lseek(int file, int ptr, int dir) {
    (void)file;
    (void)ptr;
    (void)dir;
    return 0;
}

int __attribute__((__weak__)) _open(char *path, int flags, ...) {
    (void)path;
    (void)flags;
    /* Pretend like we always fail */
    return -1;
}

int __attribute__((__weak__)) _wait(int *status) {
    (void)status;
    return -1;
}

int __attribute__((__weak__)) _unlink(char *name) {
    (void)name;
    return -1;
}

int __attribute__((__weak__)) _times(struct tms *buf) {
    (void)buf;
    return -1;
}

int __attribute__((__weak__)) _stat(char *file, struct stat *st) {
    (void)file;
    return 0;
}

int __attribute__((__weak__)) _link(char *old, char *new) {
    (void)old;
    (void)new;
    return -1;
}

int __attribute__((__weak__)) _fork(void) { return -1; }

int __attribute__((__weak__)) _execve(char *name, char **argv, char **env) {
    (void)name;
    (void)argv;
    (void)env;
    return -1;
}

static uint8_t *__sbrk_heap_end = NULL;

void* __attribute__((__weak__)) _sbrk(int incr) {
    
   extern uint8_t _end;             /* Symbol defined in the linker script */
   extern uint8_t _estack;          /* Symbol defined in the linker script */
   extern uint32_t _Min_Stack_Size; /* Symbol defined in the linker script */
   const uint32_t stack_limit =
       (uint32_t)&_estack - (uint32_t)&_Min_Stack_Size;
   const uint8_t *max_heap = (uint8_t *)stack_limit;
   uint8_t *prev_heap_end;

   /* Initialize heap end at first call */
   if (NULL == __sbrk_heap_end) {
       __sbrk_heap_end = &_end;
   }

   /* Protect heap from growing into the reserved MSP stack */
   if (__sbrk_heap_end + incr > max_heap) {
       errno = ENOMEM;
       return (void *)-1;
   }

   prev_heap_end = __sbrk_heap_end;
   __sbrk_heap_end += incr;

   return (void *)prev_heap_end;
    
}

int __attribute__((__weak__)) _getpid(void) {
    (void)1;
    return 1;
}

#else




#endif

#endif


