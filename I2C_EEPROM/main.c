
#include "main.h"

void Delay(__IO uint32_t nCount) {
  while(nCount--) {
  }
}

void init_LEDs(void) {
	GPIO_InitTypeDef GPIO_Output;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
	GPIO_Output.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14| GPIO_Pin_15;
	GPIO_Output.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Output.GPIO_OType = GPIO_OType_PP;
	GPIO_Output.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Output.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_Output);
}

void init_I2C1(void){
		GPIO_InitTypeDef GPIO_InitStruct;
		I2C_InitTypeDef I2C_InitStruct;
	
		/* enable APB1 peripheral clock for I2C1 */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	
		/* enable the peripheral clock for the pins used by 
		* I2C1, PB6 for I2C SCL and PB9 for I2C1_SDL
		*/
		/* GPIOB clock enable */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 
	
		/* This sequence sets up the I2C1SDA and I2C1SCL pins 
		* so they work correctly with the I2C1 peripheral
		*/
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_9; // Pins 6 (I2C1_SCL) and 9 (I2C1_SDA) are used
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; 			// the pins are configured as alternate function so the USART peripheral has access to them
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;		// this defines the IO speed and has nothing to do with the baudrate!
		GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;			// this defines the output type as open drain mode (as opposed to push pull)
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;			// this activates the pullup resistors on the IO pins
		GPIO_Init(GPIOB, &GPIO_InitStruct);					// now all the values are passed to the GPIO_Init() function which sets the GPIO registers

		/* The I2C1_SCL and I2C1_SDA pins are now connected to their AF
		* so that the USART1 can take over control of the 
		* pins
		*/
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1); //
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
	
		/* Configure I2C1 */
		/* I2C DeInit */
		I2C_DeInit(I2C1);
    
		/* Enable the I2C peripheral */
		I2C_Cmd(I2C1, ENABLE);
 
		/* Set the I2C structure parameters */
		I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
		I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
		I2C_InitStruct.I2C_OwnAddress1 = 0xEE;
		I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
		I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
		I2C_InitStruct.I2C_ClockSpeed = 30000;
  
		/* Initialize the I2C peripheral w/ selected parameters */
		I2C_Init(I2C1, &I2C_InitStruct);
}

uint8_t Write_24Cxx(uint16_t Addr, uint8_t Data, uint8_t Mem_Type)
{
		uint32_t timeout = I2C_TIMEOUT_MAX;
		uint8_t upper_addr,lower_addr;
		lower_addr = (uint8_t)((0x00FF)&Addr);
	
		if(Mem_Type==M24512)
		{
			Addr = Addr>>8;
			upper_addr = (uint8_t)((0x00FF)&Addr);
		}
		/* Generate the Start Condition */
		I2C_GenerateSTART(I2C1, ENABLE);

		/* Test on I2C1 EV5, Start trnsmitted successfully and clear it */
		timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
		{
			/* If the timeout delay is exeeded, exit with error code */
			if ((timeout--) == 0) return 0xFF;	
		}
   
		/* Send Memory device slave Address for write */
		I2C_Send7bitAddress(I2C1, MEM_DEVICE_WRITE_ADDR, I2C_Direction_Transmitter);
 
		/* Test on I2C1 EV6 and clear it */
		timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		{
			/* If the timeout delay is exeeded, exit with error code */
			if ((timeout--) == 0) return 0xFF;
		}
 
		if(Mem_Type==M24512)
		{
			/* Send I2C1 location address LSB */
			I2C_SendData(I2C1, upper_addr);
			/* Test on I2C1 EV8 and clear it */
			timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
			while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
			{
				/* If the timeout delay is exeeded, exit with error code */
				if ((timeout--) == 0) return 0xFF;
			}
		}	
		/* Send I2C1 location address LSB */
		I2C_SendData(I2C1, lower_addr);

		/* Test on I2C1 EV8 and clear it */
		timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			/* If the timeout delay is exeeded, exit with error code */
			if ((timeout--) == 0) return 0xFF;
		}
  
		/* Send Data */
		I2C_SendData(I2C1, Data);

		/* Test on I2C1 EV8 and clear it */
		timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			/* If the timeout delay is exeeded, exit with error code */
			if ((timeout--) == 0) return 0xFF;
		}  
 
		/* Send I2C1 STOP Condition */
		I2C_GenerateSTOP(I2C1, ENABLE);
  
		/* If operation is OK, return 0 */
		return 0;
}

uint8_t Read_24Cxx(uint16_t Addr, uint8_t Mem_Type)
{
		uint32_t timeout = I2C_TIMEOUT_MAX;
		uint8_t Data = 0;
		uint8_t upper_addr,lower_addr;
	
		lower_addr = (uint8_t)((0x00FF)&Addr);
	
		if(Mem_Type==M24512)
		{
			Addr = Addr>>8;
			upper_addr = (uint8_t)((0x00FF)&Addr);
	}
  /* Generate the Start Condition */
  I2C_GenerateSTART(I2C1, ENABLE);

  /* Test on I2C1 EV5 and clear it */
  timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {
    /* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return 0xFF;
  } 
  
  /* Send DCMI selcted device slave Address for write */
  I2C_Send7bitAddress(I2C1, MEM_DEVICE_WRITE_ADDR, I2C_Direction_Transmitter);
 
  /* Test on I2C1 EV6 and clear it */
  timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    /* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return 0xFF;
  } 

	if(Mem_Type==M24512)
	{
		/* Send I2C1 location address LSB */
		I2C_SendData(I2C1,upper_addr);

		/* Test on I2C1 EV8 and clear it */
		timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			/* If the timeout delay is exeeded, exit with error code */
			if ((timeout--) == 0) return 0xFF;
		}
	}		
	
	  /* Send I2C1 location address LSB */
  I2C_SendData(I2C1, lower_addr);

  /* Test on I2C1 EV8 and clear it */
  timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    /* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return 0xFF;
  } 
  
  /* Clear AF flag if arised */
  //I2C1->SR1 |= (uint16_t)0x0400;

  /* Generate the Start Condition */
  I2C_GenerateSTART(I2C1, ENABLE);
  
  /* Test on I2C1 EV6 and clear it */
  timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {
    /* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return 0xFF;
  } 
  
  /* Send DCMI selcted device slave Address for write */
  I2C_Send7bitAddress(I2C1, MEM_DEVICE_READ_ADDR, I2C_Direction_Receiver);
   
  /* Test on I2C1 EV6 and clear it */
  timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
  {
    /* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return 0xFF;
  }  
 
	/* Prepare an NACK for the next data received */
  I2C_AcknowledgeConfig(I2C1, DISABLE);  

  /* Test on I2C1 EV7 and clear it */
  timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
  {
    /* If the timeout delay is exeeded, exit with error code */
		//while(1);
    if ((timeout--) == 0) return 0xFF;
  }   


    
  /* Prepare Stop after receiving data */
  I2C_GenerateSTOP(I2C1, ENABLE);

  /* Receive the Data */
  Data = I2C_ReceiveData(I2C1);

  /* return the read data */
  return Data;
}

int main(void) {
	char i;
	init_LEDs();
    init_I2C1(); // initialize I2C1
	
    while (1) {
    	for (i = 0; i < 0xff; i++) {
    		if(Write_24Cxx(0x0000,i,M2404)) {
    			while(1) {}; // Write error
    		}
    		Delay(0x0000ffff); // Delay apparently needed for EEPROM to adjust

    		if((Read_24Cxx(0x0000,M2404))!=i)
    		{
    			while(1) {}; // Read error (or write failed)
    		}

    		GPIO_SetBits(GPIOD, GPIO_Pin_14);
    		Delay(0x0000ffff);
    		GPIO_ResetBits(GPIOD, GPIO_Pin_14);

    		Delay(0x000fffff);
    	}
    }
}

