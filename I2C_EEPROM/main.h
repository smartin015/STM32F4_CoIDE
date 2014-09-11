#ifndef __MAIN_H
#define __MAIN_H


#include <stm32f4xx.h>
#include <misc.h>			 // I recommend you have a look at these in the ST firmware folder
#include <stm32f4xx_i2c.h> // under Libraries/STM32F4xx_StdPeriph_Driver/inc and src
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>


#define I2C_TIMEOUT_MAX 10000
#define MEM_DEVICE_WRITE_ADDR 0xA0
#define MEM_DEVICE_READ_ADDR 0xA1


enum  eepromtype  {M2401,M2402,M2404,M2408,M2416,M2432,M2464,M24128,M24256,M24512};

void Delay(__IO uint32_t); // __IO => Volatile

void init_I2C1(void);

uint8_t Write_24Cxx(uint16_t, uint8_t, uint8_t);

uint8_t Read_24Cxx(uint16_t, uint8_t);

#endif
