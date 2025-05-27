#include <SteperMotor.h>
#include <stdint.h>

/*
    L298N电机驱动:42步进电机
*/

extern SYS_Port* port;

GPIO_Init_Parameters* L298N_Port_Init(SYS_Port* port) {
    // RCC->AHB4ENR |= RCC_AHB4ENR_GPIOGEN;  // 使能GPIOG时钟
    static GPIO_Init_Parameters L298N;
    // L298N.GPIOx                      = GPIOG;
    L298N.GPIO_Pin_Source[L208N_ENA] = ENABLE;  // ENA
    L298N.GPIO_Pin_Source[L208N_ENB] = ENABLE;  // ENB
    L298N.GPIO_Pin_Source[L298N_IN1] = ENABLE;  // IN1
    L298N.GPIO_Pin_Source[L298N_IN2] = ENABLE;  // IN2
    L298N.GPIO_Pin_Source[L298N_IN3] = ENABLE;  // IN3
    L298N.GPIO_Pin_Source[L298N_IN4] = ENABLE;  // IN4
    L298N.GPIO_Mode_x                = 1;
    L298N.GPIO_Speed_x               = 2;
    gpio.bsp_gpio_init(&L298N);
    return &L298N;
}

void Go_Step(GPIO_Init_Parameters* L298N, uint8_t Dir, uint8_t step, uint32_t speed) {
    if (Dir == Forward) {
        while (step--) {
            L298N_BForward(L298N);
            syscall.bsp_systick_delay_us(speed);
            L298N_BStop(L298N);
            syscall.bsp_systick_delay_us(speed);
            L298N_AForward(L298N);
            syscall.bsp_systick_delay_us(speed);
            L298N_AStop(L298N);
            syscall.bsp_systick_delay_us(speed);
            L298N_BBackward(L298N);
            syscall.bsp_systick_delay_us(speed);
            L298N_BStop(L298N);
            syscall.bsp_systick_delay_us(speed);
            L298N_ABackward(L298N);
            syscall.bsp_systick_delay_us(speed);
            L298N_AStop(L298N);
            syscall.bsp_systick_delay_us(speed);
        }
    } else if (Dir == Backward) {
        while (step--) {
            L298N_AForward(L298N);
            syscall.bsp_systick_delay_us(speed);
            L298N_AStop(L298N);
            syscall.bsp_systick_delay_us(speed);
            L298N_BForward(L298N);
            syscall.bsp_systick_delay_us(speed);
            L298N_BStop(L298N);
            syscall.bsp_systick_delay_us(speed);
            L298N_ABackward(L298N);
            syscall.bsp_systick_delay_us(speed);
            L298N_AStop(L298N);
            syscall.bsp_systick_delay_us(speed);
            L298N_BBackward(L298N);
            syscall.bsp_systick_delay_us(speed);
            L298N_BStop(L298N);
            syscall.bsp_systick_delay_us(speed);
        }
    }
}
