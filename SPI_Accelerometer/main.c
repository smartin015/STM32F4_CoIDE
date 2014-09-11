/* ----- Read and write to MEMS LIS302 accelerometer -----
 *
 * 				http://www.LXTronic.com
 *
 * File: 		main.c
 *
 * Description: Configures CTRL_REG1 (20h) register with 11000000 (0xC0)
 * 				Refer to LIS302 datasheet
 * 				Reads X-Axis acceleration value and writes a variable -
 * 				watch it in debug mode or use it otherwise
 *
 *
 *
 */


#include "LIS3DSH.h"
int main(void)
{
    int whoami;
    LIS3DSH_Init();

    // Test our SPI connection to ensure we can read/write
    whoami = LIS3DSH_Read(0x0F);
    while (whoami != 0x3F) {};

    LIS3DSH_Set_Output(0x47); // 25Hz update rate, all axes enabled

    float xval = 0;
    float yval = 0;
    float zval = 0;
    while(1)
    {
        xval = LIS3DSH_Get_X_Out(1);
        yval = LIS3DSH_Get_Y_Out(1);
        zval = LIS3DSH_Get_Z_Out(1);
    }
}

/*
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_spi.h"


void mySPI_Init(void);
uint8_t mySPI_GetData(uint8_t);
void mySPI_SendData(uint8_t, uint8_t);


int main(void)
{
	int8_t data;

	mySPI_Init();

	mySPI_SendData(0x20, 0xC0); //LIS302D Config


    while(1)
    {
    	data = mySPI_GetData(0x29);
    }

}






uint8_t mySPI_GetData(uint8_t adress){

	GPIO_ResetBits(GPIOE, GPIO_Pin_3);

	adress = 0x80 | adress;

	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));  //transmit buffer empty?
	SPI_I2S_SendData(SPI1, adress);
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)); //data received?
	SPI_I2S_ReceiveData(SPI1);	//Clear RXNE bit

	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));  //transmit buffer empty?
	SPI_I2S_SendData(SPI1, 0x00);	//Dummy byte to generate clock
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)); //data received?
	GPIO_SetBits(GPIOE, GPIO_Pin_3);

	return SPI_I2S_ReceiveData(SPI1); //return reveiced data
}

void mySPI_SendData(uint8_t adress, uint8_t data){

	GPIO_ResetBits(GPIOE, GPIO_Pin_3);

	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));  //transmit buffer empty?
	SPI_I2S_SendData(SPI1, adress);
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)); //data received?
	SPI_I2S_ReceiveData(SPI1);

	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));  //transmit buffer empty?
	SPI_I2S_SendData(SPI1, data);
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)); //data received?
	SPI_I2S_ReceiveData(SPI1);

	GPIO_SetBits(GPIOE, GPIO_Pin_3);
}


void mySPI_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	SPI_InitTypeDef SPI_InitTypeDefStruct;

	SPI_InitTypeDefStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitTypeDefStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitTypeDefStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitTypeDefStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitTypeDefStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitTypeDefStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitTypeDefStruct.SPI_CPOL = SPI_CPOL_High;
	SPI_InitTypeDefStruct.SPI_CPHA = SPI_CPHA_2Edge;

	SPI_Init(SPI1, &SPI_InitTypeDefStruct);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitTypeDef GPIO_InitTypeDefStruct;

	// Init PA5 (CLK), PA6 (SDO), PA7 (SDI)
	GPIO_InitTypeDefStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_6;
	GPIO_InitTypeDefStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitTypeDefStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitTypeDefStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitTypeDefStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitTypeDefStruct);

	// Init PE3 (I2C_CS/SP). This must be HIGH to specify SPI mode instead of I2C
	GPIO_InitTypeDefStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitTypeDefStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitTypeDefStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitTypeDefStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitTypeDefStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOE, &GPIO_InitTypeDefStruct);

	// Allow alternate functions on PA5, PA6, PA7
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

	GPIO_SetBits(GPIOE, GPIO_Pin_3);

	SPI_Cmd(SPI1, ENABLE);
}
*/
