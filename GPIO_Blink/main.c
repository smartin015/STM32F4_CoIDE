#include <stdio.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>

#define I2C_TIMEOUT_MAX 0xff
#define MEM_DEVICE_READ_ADDR 0xA0
#define MEM_DEVICE_WRITE_ADDR 0xA1
#define M2402 1
#define M24512 2

void Delay(long i) {
	while (i--) {  asm ( "nop" ); };
}

void init_GPIO(void) {
	GPIO_InitTypeDef GPIO_Output;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	 /* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
	GPIO_Output.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Output.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Output.GPIO_OType = GPIO_OType_PP;
	GPIO_Output.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Output.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_Output);
}

void blink(GPIO_TypeDef * port, int pin) {
	GPIO_ToggleBits(port, pin);
	Delay(0x0000ffff);
	GPIO_ToggleBits(port, pin);
	Delay(0x0000ffff);
}

int main(void) {
  	init_GPIO();

  	while (1) {
		blink(GPIOD, GPIO_Pin_12);
		blink(GPIOD, GPIO_Pin_13);
		blink(GPIOD, GPIO_Pin_14);
		blink(GPIOD, GPIO_Pin_15);
  	}
}
