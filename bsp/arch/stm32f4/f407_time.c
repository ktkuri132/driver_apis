
#include "f407.h"
#ifdef STM32F40_41xxx  // 选择对应的芯片

#include <stdint.h>

#define OC1_PWM_MODE1 TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2
#define OC2_PWM_MODE1 TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2
#define OC3_PWM_MODE1 TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2
#define OC4_PWM_MODE1 TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2

#define OC1_PWM_MODE2 TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0
#define OC2_PWM_MODE2 TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_0
#define OC3_PWM_MODE2 TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_0
#define OC4_PWM_MODE2 TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_0

#define PWM_Mode_1 1
#define PWM_Mode_2 2

void F407_TIM_Init(void* const Parameters) {
    TIM_Parameters* TIM_Parameter = (TIM_Parameters*)Parameters;
    for (int i = 0; i < 16; i++) {
        if (TIM_Parameter->GPIO_Pin_Source[i]) {
            TIM_Parameter->GPIOx->MODER &= ~(3 << (2 * i));
            TIM_Parameter->GPIOx->MODER |= 2 << (2 * i);
            TIM_Parameter->GPIOx->OSPEEDR &= ~(3 << (2 * i));
            TIM_Parameter->GPIOx->OSPEEDR |= 3 << (2 * i);
            TIM_Parameter->GPIOx->PUPDR &= ~(3 << (2 * i));
            TIM_Parameter->GPIOx->PUPDR |= 1 << (2 * i);
            TIM_Parameter->GPIOx->AFR[i / 8] &=
                ~(0x0F << (4 * (i % 8)));
            TIM_Parameter->GPIOx->AFR[i / 8] |=
                TIM_Parameter->GPIO_AF << (4 * (i % 8));
            TIM_Parameter->GPIOx->OTYPER &= ~(1 << i);
            TIM_Parameter->GPIOx->OTYPER |= 0 << i;
        }
    }
    TIM_Parameter->TIMx->ARR = TIM_Parameter->ARR;
    TIM_Parameter->TIMx->PSC = TIM_Parameter->PSC;
    for (int i = 0; i < 4; i++) {
        if (TIM_Parameter->Channel[i]) {
            switch (i) {
                case Channel_1: {
                    if (TIM_Parameter->OCMode == PWM_Mode_1) {
                        TIM_Parameter->TIMx->CCMR1 |= OC1_PWM_MODE1;
                    } else if (TIM_Parameter->OCMode == PWM_Mode_2) {
                        TIM_Parameter->TIMx->CCMR1 |= OC1_PWM_MODE2;
                    }
                    TIM_Parameter->TIMx->CCMR1 |= TIM_CCMR1_OC1PE;
                    TIM_Parameter->TIMx->CCER |= TIM_CCER_CC1E;
                } break;
                case Channel_2: {
                    if (TIM_Parameter->OCMode == PWM_Mode_1) {
                        TIM_Parameter->TIMx->CCMR1 |= OC2_PWM_MODE1;
                    } else if (TIM_Parameter->OCMode == PWM_Mode_2) {
                        TIM_Parameter->TIMx->CCMR1 |= OC2_PWM_MODE2;
                    }
                    TIM_Parameter->TIMx->CCMR1 |= TIM_CCMR1_OC2PE;
                    TIM_Parameter->TIMx->CCER |= TIM_CCER_CC2E;
                } break;
                case Channel_3: {
                    if (TIM_Parameter->OCMode == PWM_Mode_1) {
                        TIM_Parameter->TIMx->CCMR2 |= OC3_PWM_MODE1;
                    } else if (TIM_Parameter->OCMode == PWM_Mode_2) {
                        TIM_Parameter->TIMx->CCMR2 |= OC3_PWM_MODE2;
                    }
                    TIM_Parameter->TIMx->CCMR2 |= TIM_CCMR2_OC3PE;
                    TIM_Parameter->TIMx->CCER |= TIM_CCER_CC3E;
                } break;
                case Channel_4: {
                    if (TIM_Parameter->OCMode == PWM_Mode_1) {
                        TIM_Parameter->TIMx->CCMR2 |= OC4_PWM_MODE1;
                    } else if (TIM_Parameter->OCMode == PWM_Mode_2) {
                        TIM_Parameter->TIMx->CCMR2 |= OC4_PWM_MODE2;
                    }
                    TIM_Parameter->TIMx->CCMR2 |= TIM_CCMR2_OC4PE;
                    TIM_Parameter->TIMx->CCER |= TIM_CCER_CC4E;
                } break;
                default:
                    break;
            }
        }
    }

    TIM_Parameter->TIMx->CR1 |= TIM_CR1_ARPE;
    TIM_Parameter->TIMx->CR1 |= TIM_CR1_CEN;
}

#endif