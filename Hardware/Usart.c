#include "stm32f10x.h"                  // Device header
#include "Usart.h"

#pragma import(__use_no_semihosting)//关闭ARM的半主机模式

#include <stdio.h>


/***********************

Usart1's RX still has bug,it can't receive data.

************************/



struct __FILE
{
	int handle; /* Add whatever you need here */ 
};

FILE __Usart1_stdout;
FILE __Usart1_stdin;

int Usart1_fputc(int ch, FILE *f) 
{
	Usart1_SendByte(ch);
	return ch;
}

void _Usart1_sys_exit(int x) 
{
	x = x; /* endless loop */
}

void _ttywrch(int ch) { ch = ch; }

char Usart1_Rx_Data;
uint8_t Usart1_Rx_Flag;

uint8_t Usart1_RxState;
uint8_t Usart1_pRxPacket;
char Usart1_Rx_Packet[100];

void Usart1_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,  ENABLE);            //??APB2??????
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	
	//tx
	GPIO_InitTypeDef	GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//rx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	
	USART_InitTypeDef	USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 1000000;//9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	NVIC_InitTypeDef	NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
}

void Usart1_SendByte(uint8_t Byte)
{
	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}

void Usart1_SendArray(uint8_t *Array,uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i++)
	{
		Usart1_SendByte(Array[i]);
	}
}

void Usart1_SendPacket(uint8_t *Array,uint16_t Length)
{
	Usart1_SendByte(0xFF);
	Usart1_SendArray(Array,Length);
	Usart1_SendByte(0xFE);
}

void Usart1_SendString(char *String)
{
	uint8_t i;
	for(i = 0; String[i] != '\0';i++)
	{
		Usart1_SendByte(String[i]);
	}
}

//进值转换
uint32_t Usart1_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

void Usart1_SendNum(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		Usart1_SendByte(Number / Usart1_Pow(10, Length - i - 1) % 10 + '0');
	}
}

uint8_t Usart1_GetRxFlag()
{
	if (Usart1_Rx_Flag == 1)
	{
		Usart1_Rx_Flag = 0;
		return 1;
	}
	return 0;
}

uint8_t Usart1_GetRxData()
{
	return Usart1_Rx_Data;
}

char* Usart1_GetRxPacketData()
{
	return Usart1_Rx_Packet;
}

void USART1_IRQHandler()
{
	if (USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET)
	{
		Usart1_Rx_Data = USART_ReceiveData(USART1);
//		Usart1_SendByte(Usart1_Rx_Data);
		
		if (Usart1_RxState == 0)
		{
			if (Usart1_Rx_Data == '@')
			{
				Usart1_RxState = 1;
				Usart1_pRxPacket = 0;
			}
		}
		else if (Usart1_RxState == 1)
		{
			if (Usart1_Rx_Data == '\r')
			{
				Usart1_RxState = 2;
			}
			else
			{
				Rx_Packet[Usart1_pRxPacket] = Usart1_Rx_Data;
				Usart1_pRxPacket++;			
			}
		}
		else if (Usart1_RxState == 2)
		{
			if (Usart1_Rx_Data== '\n')
			{
				Usart1_RxState = 0;
				Rx_Packet[Usart1_pRxPacket] = '\0';
				Usart1_Rx_Flag = 1;
			}
		}
		
		USART_ClearITPendingBit(USART1,USART_FLAG_RXNE);
	}
}
