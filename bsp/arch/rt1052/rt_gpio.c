#include <sysport.h>
#ifdef CPU_MIMXRT1052DVL6B
#include <MIMXRT1052.h>

#include <stdint.h>

#include "rt1052.h"

/**
 * @brief RT1052的 GPIO配置函数
 * @note 对于RT的这个GPIO驱动函数，不能使用第0个IO，就是kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_00，不然会报错
 * @param Parameters 配置结构体
 */
void RT_GPIO_Config(void* const Parameters) {
    GPIO_Init_Parameters* GPIO_Parameter = (GPIO_Init_Parameters*)Parameters;
    uint32_t __gpio;
    if (GPIO_Parameter->GPIOx == GPIO1) {
        __gpio = 1;
    } else if (GPIO_Parameter->GPIOx == GPIO2) {
        __gpio = 2;
    } else if (GPIO_Parameter->GPIOx == GPIO3) {
        __gpio = 3;
    } else if (GPIO_Parameter->GPIOx == GPIO4) {
        __gpio = 4;
    } else {
        printf(FG_RED "Error: Invalid GPIOx\n" RESET_ALL);
    }
    int i         = 0;
    uint8_t __pin = 0;
    if (GPIO_Parameter->GPIO_Mode_x == 2) {  // 如果被设置成复用功能
        for (; i < 32; i++) {
            if (GPIO_Parameter->GPIO_Pin_Source[i]) {
                // 引脚映射
                __pin = (__gpio - 1) * 32 + i;
                IOMUXC->SW_MUX_CTL_PAD[__pin] |=
                    IOMUXC_SW_MUX_CTL_PAD_MUX_MODE(GPIO_Parameter->GPIO_AF_x);
            }
        }
    } else {
        for (; i < 32; i++) {
            if (GPIO_Parameter->GPIO_Pin_Source[i]) {
                GPIO_Parameter->GPIOx->GDIR &= ~(1 << i);  // 清除 GDIR 的相应位
                GPIO_Parameter->GPIOx->GDIR |= GPIO_Parameter->GPIO_Mode_x
                                               << i;  // 设置 GDIR 的相应位
                // 计算引脚号
                __pin = (__gpio - 1) * 32 + i;
                // 设置引脚复用功能,GPIO的配置只有在GPIO模式下才有效
                IOMUXC->SW_MUX_CTL_PAD[GPIO_Parameter->GPIO_Pin_Source[i]] |= IOMUXC_SW_MUX_CTL_PAD_MUX_MODE(5);
                IOMUXC->SW_PAD_CTL_PAD[GPIO_Parameter->GPIO_Pin_Source[i]] |= 0x10B0u;
                if (GPIO_Parameter->GPIO_OType_xx) {
                    // 清除 OTYPER 的相应位
                    IOMUXC->SW_PAD_CTL_PAD[GPIO_Parameter->GPIO_Pin_Source[i]] &= ~(1 << __pin);
                    // 设置引脚的输出类型
                    IOMUXC->SW_PAD_CTL_PAD[GPIO_Parameter->GPIO_Pin_Source[i]] |=
                        IOMUXC_SW_PAD_CTL_PAD_ODE(GPIO_Parameter->GPIO_OType_xx);
                }
                if (GPIO_Parameter->GPIO_PuPd_x) {
                    // 清除 PUPDR 的相应位
                    IOMUXC->SW_PAD_CTL_PAD[GPIO_Parameter->GPIO_Pin_Source[i]] &= ~(3 << __pin);
                    // 设置引脚的上拉/下拉电阻
                    IOMUXC->SW_PAD_CTL_PAD[GPIO_Parameter->GPIO_Pin_Source[i]] |=
                        IOMUXC_SW_PAD_CTL_PAD_PUS(GPIO_Parameter->GPIO_PuPd_x);
                }
                if (GPIO_Parameter->GPIO_Speed_x) {
                    // 清除 OSPEEDR 的相应位
                    IOMUXC->SW_PAD_CTL_PAD[GPIO_Parameter->GPIO_Pin_Source[i]] &= ~(3 << __pin);
                    // 设置引脚的速度
                    IOMUXC->SW_PAD_CTL_PAD[GPIO_Parameter->GPIO_Pin_Source[i]] |=
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(GPIO_Parameter->GPIO_Speed_x);
                }
            }
        }
    }
}

void RT_GPIO_AF_Config(void* const Parameters) {
    GPIO_Init_Parameters* GPIO_Parameter = (GPIO_Init_Parameters*)Parameters;
}

void RT_GPIO_Pin_Set(void* const Parameters, uint8_t Pin, uint8_t status) {
    GPIO_Init_Parameters* GPIO_Parameter = (GPIO_Init_Parameters*)Parameters;
    if (status) {
        GPIO_Parameter->GPIOx->DR |= 1U << Pin;  // 设置引脚为高电平
    } else {
        GPIO_Parameter->GPIOx->DR &= ~(1U << Pin);  // 设置引脚为低电平
    }
}

/**
 * @brief  GPIO引脚读取函数
 * @param  Parameters: GPIO引脚参数结构体指针
 * @retval 引脚状态 (0: 低电平, 1: 高电平)
 */
uint8_t RT_GPIO_Pin_Get(void* const Parameters, uint8_t Pin) {
    GPIO_Init_Parameters* GPIO_Parameter = (GPIO_Init_Parameters*)Parameters;
    if (GPIO_Parameter->GPIOx->DR & (1 << Pin)) {
        return 1;  // 高电平
    } else {
        return 0;  // 低电平
    }
}

uint32_t RT_GetGPIOPeriphClock(void* const Parameters) {}

#endif