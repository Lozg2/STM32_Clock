#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include "RTC.h"

uint16_t Serial_SendTime[6];
char Serial_RxPacket[100];
uint8_t Serial_RxFlag;

void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	/*USART      TX PA10    RX PA9*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStruture;
	USART_InitStruture.USART_BaudRate = 9600;
	USART_InitStruture.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruture.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruture.USART_Parity = USART_Parity_No;
	USART_InitStruture.USART_StopBits = USART_StopBits_1;
	USART_InitStruture.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStruture);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	 
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
}	

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

}

	
void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)
	{
		Serial_SendByte(Number / Serial_Pow (10,Length - i - 1) % 10 + '0');
	}

}

/*Send Time*/
void Serial_SendPacket(void)
{
	Serial_SendString("\r\nTIME:");
	Serial_SendNumber(RTC_Time[0],4);
	Serial_SendString("-");
	Serial_SendNumber(RTC_Time[1],2);
	Serial_SendString("-");
	Serial_SendNumber(RTC_Time[2],2);
	Serial_SendString(" ");
	Serial_SendNumber(RTC_Time[3],2);
	Serial_SendString(":");
	Serial_SendNumber(RTC_Time[4],2);
	Serial_SendString(":");
	Serial_SendNumber(RTC_Time[5],2);
	Serial_SendString(".\r\n");
	Serial_SendString("Send Over");	
}	

void USART1_IRQHandler(void)
{
	static uint8_t RxState = 0;
	static uint8_t pRxPacket = 0;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		uint8_t RxData = USART_ReceiveData(USART1);
		
		if (RxState == 0)
		{
			/*TIME_E*/
			if (RxData == 'E' && Serial_RxFlag == 0)
			{
				RxState = 1;
				pRxPacket = 0;
			}
			/*ALARM_L*/
			else if (RxData == 'L' && Serial_RxFlag == 0)
			{
				RxState = 2;
				pRxPacket = 0;
			}
		}
			/*TIME_=*/
		else if (RxState == 1)
		{
			if (RxData == '=')
			{
				RxState = 3;
			}
			else if(RxData == '?')
			{
				RxState = 0;
				Serial_RxFlag = 3;
			}
		}
			/*ALARM_=*/
		else if (RxState == 2)
		{
			if (RxData == '=')
			{
				RxState = 4;
			}
			else if(RxData == '?')
			{
				RxState = 0;
				Serial_RxFlag = 2;
			}
		}
		/*SET_TIME*/
		else if (RxState == 3)
		{
			if (RxData == '.')
			{
				RxState = 5;         //end
			}	
			else
			{
				Serial_RxPacket[pRxPacket] = RxData;
				pRxPacket ++;
			}
		}
		else if (RxState == 5)
			{
				RxState = 0;
				Serial_RxPacket[pRxPacket] = '\0';
				Serial_RxFlag = 1;
			}
			/*SET_ALARM*/
		else if (RxState == 4)
		{
			if (RxData == '.')
			{
				RxState = 6;
			}
			else
			{
				Serial_RxPacket[pRxPacket] = RxData;
				pRxPacket ++;
			}
		}
		else if (RxState == 6)
			{
				RxState = 0;
				Serial_RxPacket[pRxPacket] = '\0';
				Serial_RxFlag = 2;
			}
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	 
	}
}

int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}
