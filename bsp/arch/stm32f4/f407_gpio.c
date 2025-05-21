
#include "f407.h"
#ifdef STM32F40_41xxx

#include "f407_gpio.h"
#include <stdint.h>
#include <stm32f4/stm32f4xx.h>


/**
 * @brief GPIO驱动函数
 * 
 * @param Parameters 参数结构体指针
 */
void F407_GPIO_Config(void* const Parameters)
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
                GPIO_Parameter->GPIO_AF << (4 * (i % 8));
                GPIO_Parameter->GPIOx->OTYPER &= ~(1 << i);
                GPIO_Parameter->GPIOx->OTYPER |= GPIO_Parameter->GPIO_OType_xx << i;
        }
    }
}

void F407_GPIO_AF_Config(void* const Parameters){
    
}


void F407_GPIO_Pin_Set(void* const Parameters,uint8_t Pin,uint8_t status){
    GPIO_Init_Parameters *GPIO_Parameter = (GPIO_Init_Parameters*)Parameters;
    if (status) {
        GPIO_Parameter->GPIOx->ODR |= (1U << Pin); // 设置 ODR 的相应位
    } else {
        GPIO_Parameter->GPIOx->ODR &= ~(1U << Pin); // 清除 ODR 的相应位
    }
}

uint8_t F407_GPIO_Pin_Get(void* const Parameters,uint8_t Pin){
    GPIO_Init_Parameters *GPIO_Parameter = (GPIO_Init_Parameters*)Parameters;
    return (GPIO_Parameter->GPIOx->IDR & (1U << Pin)) ? 1 : 0; // 读取 IDR 的相应位
}

uint32_t F407_GetGPIOPeriphClock(void* const Parameters){
    return 0;
}


#endif