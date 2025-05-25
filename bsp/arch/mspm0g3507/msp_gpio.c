#include "mspm0.h"
#include <ti/driverlib/dl_gpio.h>
#include <ti/devices/msp/m0p/mspm0g350x.h>
#include <syscall.h>
#include <sysport.h>

/**
 * @brief MSP的GPIO初始化函数
 * 
 * @param Parameters 参数结构体指针
 * @note  该函数用于初始化MSP的GPIO引脚
 */
void MSP_GPIO_init(void *const Parameters) {
    gpio_st *io = (gpio_st *)Parameters;
    if(io->INVERSION == NULL){  /*默认不反转*/
        io->INVERSION = DL_GPIO_INVERSION_DISABLE;
    }
    if(io->RESISTOR == NULL){   /*默认浮空*/
        io->RESISTOR = DL_GPIO_RESISTOR_NONE;
    }
    if(io->DRIVE_STRENGTH == NULL){ /*默认低驱动*/
        io->DRIVE_STRENGTH = DL_GPIO_DRIVE_STRENGTH_LOW;
    }
    if(io->HIZ == NULL){ /*默认不高阻*/
        io->HIZ = DL_GPIO_HIZ_DISABLE;
    }
    if(io->WAKEUP == NULL){ /*默认不唤醒*/
        io->WAKEUP = DL_GPIO_WAKEUP_DISABLE;
    }
    if(io->EVENT_ROUTE == NULL){ /*默认不事件路由*/
        io->EVENT_ROUTE = DL_GPIO_EVENT_ROUTE_1;
    }
    if(io->IIDX == NULL){ /*默认不使用中断*/
        io->IIDX = GPIO_CPU_INT_IIDX_STAT_NO_INTR;
    }
    if(io->IO_Dir == NULL){ /*默认输出*/
        io->IO_Dir = GPIO_DIR_OUTPUT;
    }
    if(io->IO_Dir == GPIO_DIR_INPUT){ /*输入*/
        DL_GPIO_initDigitalInput(io->PINCM);
        DL_GPIO_initDigitalInputFeatures(io->PINCM, io->INVERSION,io->RESISTOR,
                                        io->HYSTERESIS, io->WAKEUP);
    } else if(io->IO_Dir == GPIO_DIR_OUTPUT){ /*输出*/
        DL_GPIO_initDigitalOutput(io->PINCM);
        DL_GPIO_initDigitalOutputFeatures(io->PINCM, io->INVERSION,io->RESISTOR,
                                            io->DRIVE_STRENGTH, io->HIZ);
        DL_GPIO_enableOutput(io->GPIOx, io->GPIO_Pin); /*使能输出*/
    } else if(io->IO_Dir == GPIO_DIR_Analog){ /*模拟*/
        DL_GPIO_initDigitalInput(io->PINCM);
        DL_GPIO_initPeripheralAnalogFunction(io->PINCM);
    } else if(io->IO_Dir == GPIO_DIR_DIGITAL){ /*数字*/
        DL_GPIO_initDigitalInput(io->PINCM);
        DL_GPIO_initPeripheralFunction(io->PINCM, io->GPIO_AF);
    }
}

/**
 * @brief MSP的GPIO引脚设置函数
 * 
 * @param Parameters GPIO引脚参数结构体指针
 * @param pin 具体引脚
 * @param value 状态值 0:低电平 1:高电平 2:翻转
 */
void MSP_GPIO_setPin(void *const Parameters,uint32_t pin,uint8_t value) {
    GPIO_Pin_Parameters *io = (GPIO_Pin_Parameters *)Parameters;
    if(value == 0){ /*低电平*/
        DL_GPIO_clearPins(io->GPIOx, pin);
    } else if(value == 1){ /*高电平*/
        DL_GPIO_setPins(io->GPIOx, pin);
    } else if(value == 2){ /*翻转*/
        DL_GPIO_togglePins(io->GPIOx, pin);
    }
}

uint8_t MSP_GPIO_getPin(void *const Parameters,uint32_t pin) {
    GPIO_Pin_Parameters *io = (GPIO_Pin_Parameters *)Parameters;
    return DL_GPIO_readPins(io->GPIOx, pin);
}

/**
 * @brief MSP的GPIO复用功能设置函数
 * 
 * @param Parameters 参数结构体指针
 * @note  该函数用于设置MSP的GPIO引脚的复用功能
 */
void MSP_IOMUX(void *const Parameters) {
    gpio_st *io = (gpio_st *)Parameters;
    //DL_GPIO_initPeripheralFunction(io->PINCM, io->GPIO_AF);
    if(io->IO_Dir == GPIO_DIR_INPUT){ /*输入*/
        DL_GPIO_initPeripheralInputFunction(io->PINCM, io->GPIO_AF);
        // DL_GPIO_initPeripheralInputFunctionFeatures(io->PINCM, io->GPIO_AF,
        //                                             io->INVERSION, io->RESISTOR,
        //                                             io->HYSTERESIS, io->WAKEUP);
    } else if(io->IO_Dir == GPIO_DIR_OUTPUT){ /*输出*/
        DL_GPIO_initPeripheralOutputFunction(io->PINCM, io->GPIO_AF);
        // DL_GPIO_initPeripheralOutputFunctionFeatures(io->PINCM, io->GPIO_AF,
        //                                             io->INVERSION, io->RESISTOR,
        //                                             io->DRIVE_STRENGTH, io->HIZ);
    } else if(io->IO_Dir == GPIO_DIR_Analog){ /*模拟*/
        DL_GPIO_initPeripheralAnalogFunction(io->PINCM);
    }
}