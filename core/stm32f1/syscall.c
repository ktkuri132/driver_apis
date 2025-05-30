#include <stdint.h>
#include <syscall.h>
#include <sysport.h>
#include <middlewave/sh/shell.h>
EnvVar MyEnv[];
uart_st *usart_1;

void __delay_us(uint32_t nus){
    uint32_t temp;	    	 
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


