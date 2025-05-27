#ifndef __STEPER_MOTOR_H
#define __STEPER_MOTOR_H

#include <sysport.h>

typedef enum {
    Forward,
    Backward,
} GPIO_Pin_L298N;

#define L208N_ENA Pin2
#define L208N_ENB Pin3
#define L298N_IN1 Pin4
#define L298N_IN2 Pin5
#define L298N_IN3 Pin6
#define L298N_IN4 Pin7

#define ENA(L298N,y) gpio.bsp_gpio_pin_set(L298N, L208N_ENA, y)
#define ENB(L298N,y) gpio.bsp_gpio_pin_set(L298N, L208N_ENB, y)
#define IN1(L298N,y) gpio.bsp_gpio_pin_set(L298N, L298N_IN1, y)
#define IN2(L298N,y) gpio.bsp_gpio_pin_set(L298N, L298N_IN2, y)
#define IN3(L298N,y) gpio.bsp_gpio_pin_set(L298N, L298N_IN3, y)
#define IN4(L298N,y) gpio.bsp_gpio_pin_set(L298N, L298N_IN4, y)

#define L298N_AStop(L298N) do { \
                                ENA(L298N, 0); \
                                IN1(L298N, 0); \
                                IN2(L298N, 0); \
                            } while (0)
#define L298N_AForward(L298N) do { \
                                ENA(L298N, 1); \
                                IN1(L298N, 1); \
                                IN2(L298N, 0); \
                            } while (0)
#define L298N_ABackward(L298N) do { \
                                ENA(L298N, 1); \
                                IN1(L298N, 0); \
                                IN2(L298N, 1); \
                            } while (0)
#define L298N_BStop(L298N) do { \
                                ENB(L298N, 0); \
                                IN3(L298N, 0); \
                                IN4(L298N, 0); \
                            } while (0)
#define L298N_BForward(L298N) do { \
                                ENB(L298N, 1); \
                                IN3(L298N, 1); \
                                IN4(L298N, 0); \
                            } while (0)
#define L298N_BBackward(L298N) do { \
                                ENB(L298N, 1); \
                                IN3(L298N, 0); \
                                IN4(L298N, 1); \
                            } while (0)

GPIO_Init_Parameters* L298N_Port_Init(SYS_Port* port);
void Go_Step(GPIO_Init_Parameters* L298N, uint8_t Dir, uint8_t step, uint32_t speed);

#endif