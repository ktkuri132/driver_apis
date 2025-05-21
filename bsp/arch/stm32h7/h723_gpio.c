#include "h723.h"
#ifdef STM32H723xx
#include <stdint.h>
#include <stm32h7xx.h>
#include <stm32h723xx.h>

void GPIO_Set(GPIO_TypeDef* GPIOx,u32 BITx,u32 MODE,u32 OTYPE,u32 OSPEED,u32 PUPD)
{  
	u32 pinpos=0,pos=0,curpin=0;
	for(pinpos=0;pinpos<16;pinpos++)
	{
		pos=1<<pinpos;	
		curpin=BITx&pos;
		if(curpin==pos)	
		{
			GPIOx->MODER&=~(3<<(pinpos*2));	
			GPIOx->MODER|=MODE<<(pinpos*2);	 
			if((MODE==0X01)||(MODE==0X02))	
			{  
				GPIOx->OSPEEDR&=~(3<<(pinpos*2));	
				GPIOx->OSPEEDR|=(OSPEED<<(pinpos*2));  
				GPIOx->OTYPER&=~(1<<pinpos) ;		
				GPIOx->OTYPER|=OTYPE<<pinpos;		
			}  
			GPIOx->PUPDR&=~(3<<(pinpos*2));	
			GPIOx->PUPDR|=PUPD<<(pinpos*2);	
		}
	}
}

void GPIO_AF_Set(GPIO_TypeDef* GPIOx,u8 BITx,u8 AFx)
{  
	GPIOx->AFR[BITx>>3]&=~(0X0F<<((BITx&0X07)*4));
	GPIOx->AFR[BITx>>3]|=(u32)AFx<<((BITx&0X07)*4);
} 

void __H723_GPIO_Config(void* const Parameters)
{
    GPIO_Init_Parameters *GPIO_Parameter = (GPIO_Init_Parameters*)Parameters;   
    // 清除并设置 MODER 寄存器的相应位
    GPIO_Parameter->GPIOx->MODER &= ~(3U << (GPIO_Parameter->GPIO_Pin_x * 2)); // 清除模式位
    GPIO_Parameter->GPIOx->MODER |= (GPIO_Parameter->GPIO_Mode_x << (GPIO_Parameter->GPIO_Pin_x * 2)); // 设置模式位

    // 清除并设置 OSPEEDR 寄存器的相应位
    GPIO_Parameter->GPIOx->OSPEEDR &= ~(3U << (GPIO_Parameter->GPIO_Pin_x * 2)); // 清除速度位
    GPIO_Parameter->GPIOx->OSPEEDR |= (GPIO_Parameter->GPIO_Speed_x << (GPIO_Parameter->GPIO_Pin_x * 2)); // 设置速度位

    // 清除并设置 PUPDR 寄存器的相应位
    GPIO_Parameter->GPIOx->PUPDR &= ~(3U << (GPIO_Parameter->GPIO_Pin_x * 2)); // 清除上拉/下拉位
    GPIO_Parameter->GPIOx->PUPDR |= (GPIO_Parameter->GPIO_PuPd_x << (GPIO_Parameter->GPIO_Pin_x * 2)); // 设置上拉/下拉位

    // 清除并设置 OTYPER 寄存器的相应位
    GPIO_Parameter->GPIOx->OTYPER &= ~(1U << GPIO_Parameter->GPIO_Pin_x); // 清除输出类型位
    GPIO_Parameter->GPIOx->OTYPER |= (GPIO_Parameter->GPIO_OType_xx << GPIO_Parameter->GPIO_Pin_x); // 设置输出类型位
    // GPIO_Set(GPIO_Parameter->GPIOx, GPIO_Parameter->GPIO_Pin_x, GPIO_Parameter->GPIO_Mode_x, GPIO_Parameter->GPIO_OType_xx, GPIO_Parameter->GPIO_Speed_x, GPIO_Parameter->GPIO_PuPd_x);

}


void H723_GPIO_Config(void* const Parameters)
{
    GPIO_Init_Parameters *GPIO_Parameter = (GPIO_Init_Parameters*)Parameters;   
    for (int i = 0; i < 16; i++) {
        if (GPIO_Parameter->GPIO_Pin_Source[i]) {
            GPIO_Parameter->GPIOx->MODER &= ~(3 << (2 * i));
            GPIO_Parameter->GPIOx->MODER |= GPIO_Parameter->GPIO_Mode_x << (2 * i);
            GPIO_Parameter->GPIOx->OSPEEDR &= ~(3 << (2 * i));
            GPIO_Parameter->GPIOx->OSPEEDR |= 3 << (2 * i);
            GPIO_Parameter->GPIOx->PUPDR &= ~(3 << (2 * i));
            GPIO_Parameter->GPIOx->PUPDR |= GPIO_Parameter->GPIO_PuPd_x << (2 * i);
            GPIO_Parameter->GPIOx->AFR[i / 8] &=
                ~(0x0F << (4 * (i % 8)));
                GPIO_Parameter->GPIOx->AFR[i / 8] |=
                GPIO_Parameter->GPIO_AF_x << (4 * (i % 8));
                GPIO_Parameter->GPIOx->OTYPER &= ~(1 << i);
                GPIO_Parameter->GPIOx->OTYPER |= GPIO_Parameter->GPIO_OType_xx << i;
        }
    }
}

void H723_GPIO_AF_Config(void* const Parameters){
    GPIO_Init_Parameters *GPIO_Parameter = (GPIO_Init_Parameters*)Parameters;
    if (GPIO_Parameter->GPIO_Pin_x < 8) {
        GPIO_Parameter->GPIOx->AFR[0] &= ~(0xF << (GPIO_Parameter->GPIO_Pin_x * 4)); // 清除 AFR[0] 的相应位
        GPIO_Parameter->GPIOx->AFR[0] |= (GPIO_Parameter->GPIO_AF_x << (GPIO_Parameter->GPIO_Pin_x * 4)); // 设置 AFR[0] 的相应位
    } else {
        GPIO_Parameter->GPIOx->AFR[1] &= ~(0xF << ((GPIO_Parameter->GPIO_Pin_x - 8) * 4)); // 清除 AFR[1] 的相应位
        GPIO_Parameter->GPIOx->AFR[1] |= (GPIO_Parameter->GPIO_AF_x << ((GPIO_Parameter->GPIO_Pin_x - 8) * 4)); // 设置 AFR[1] 的相应位
    }
    // GPIO_AF_Set(GPIO_Parameter->GPIOx, GPIO_Parameter->GPIO_Pin_Source_x, GPIO_Parameter->GPIO_AF_x);
}


void H723_GPIO_Pin_Set(void* const Parameters,uint8_t Pin,uint8_t status){
    GPIO_Init_Parameters *GPIO_Parameter = (GPIO_Init_Parameters*)Parameters;
    if (status) {
        GPIO_Parameter->GPIOx->ODR |= (1U << Pin); // 设置 ODR 的相应位
    } else {
        GPIO_Parameter->GPIOx->ODR &= ~(1U << Pin); // 清除 ODR 的相应位
    }
}

/**
    * @brief  GPIO引脚读取函数
    * @param  Parameters: GPIO引脚参数结构体指针
    * @retval 引脚状态 (0: 低电平, 1: 高电平)
*/
uint8_t H723_GPIO_Pin_Get(void* const Parameters,uint8_t Pin){
    GPIO_Init_Parameters *GPIO_Parameter = (GPIO_Init_Parameters*)Parameters;
    return (GPIO_Parameter->GPIOx->IDR & (1U << Pin)) ? 1 : 0; // 读取 IDR 的相应位
}

uint32_t H723_GetGPIOPeriphClock(void* const Parameters){
    

}

#endif