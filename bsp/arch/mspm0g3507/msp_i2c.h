#ifndef __MSP_IIC_H
#define __MSP_IIC_H

#include <syscall.h>
#include <sysport.h>
#include <ti/driverlib/dl_i2c.h>

// #define __SOFT_I2C__  // 定义为软件IIC
#define __HARD_I2C__  // 定义为硬件IIC
#ifdef __SOFT_I2C__

#endif

#ifdef __HARD_I2C__



#endif

#endif