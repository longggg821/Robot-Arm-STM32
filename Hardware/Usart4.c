#include "stm32f10x.h"                  // Device header
#include "Usart4.h"

#pragma import(__use_no_semihosting)//关闭ARM的半主机模式

#include <stdio.h>

struct __FILE
{
	int handle; /* Add whatever you need here */ 
};

FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) 
{
	Uart4_SendByte(ch);
	return ch;
}

void _sys_exit(int x) 
{
	x = x; /* endless loop */
}

char Rx_Data;
uint8_t Rx_Flag;

uint8_t RxState;
uint8_t pRxPacket;
char Rx_Packet[100];

uint8_t Rx_Receive[201];
int reLength=0;
uint8_t reState=0;
uint8_t pReceive=0;

void Uart4_Init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	//tx
	GPIO_InitTypeDef	GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	//rx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);	
	
	USART_InitTypeDef	USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;//1000000;//
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(UART4,&USART_InitStructure);
	
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
	USART_ClearFlag(UART4,USART_FLAG_TC);
	
	NVIC_InitTypeDef	NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(UART4,ENABLE);
}

void Uart4_SendByte(uint8_t Byte)
{
	USART_SendData(UART4,Byte);
	while(USART_GetFlagStatus(UART4,USART_FLAG_TXE) == RESET);
}

void Uart4_SendArray(uint8_t *Array,uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i++)
	{
		Uart4_SendByte(Array[i]);
	}
}

void Uart4_SendPacket(uint8_t *Array,uint16_t Length)
{
	Uart4_SendByte(0xFF);
	Uart4_SendArray(Array,Length);
	Uart4_SendByte(0xFE);
}

void Uart4_SendString(char *String)
{
	uint8_t i;
	for(i = 0; String[i] != '\0';i++)
	{
		Uart4_SendByte(String[i]);
	}
}

//进值转换
uint32_t Uart4_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

void Uart4_SendNum(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		Uart4_SendByte(Number / Uart4_Pow(10, Length - i - 1) % 10 + '0');
	}
}

uint8_t Uart4_GetRxFlag()
{
	if (Rx_Flag == 1)
	{
		Rx_Flag = 0;
		return 1;
	}
	return 0;
}

uint8_t Uart4_GetRxData()
{
	return Rx_Data;
}
char* Uart4_GetRxPacketData()
{
	return Rx_Packet;
}
uint8_t Uart4_GetRxDataLength(){
	return pRxPacket;
}

void Uart4_BeginReceive(int nLen){
	reLength=nLen;
	pReceive=0;
	reState=1;  //receiving
}

uint8_t Uart4_IsReceiveFinish(){
	if(reState==2){
		reState=0;
		return 1;
	}
	return 0;
}
uint8_t* Uart4_GetReceiveData(){
	return Rx_Receive;
}

void UART4_IRQHandler()
{
	if (USART_GetFlagStatus(UART4,USART_FLAG_RXNE) == SET)
	{
		Rx_Data = USART_ReceiveData(UART4);
		if(reState==1){
			Rx_Receive[pReceive]=Rx_Data;
			pReceive++;
			if(pReceive>=reLength){
				reState=2; //receive finish
			}
		}
		
		if (RxState == 0)
		{
			if (Rx_Data == '@')
			{
				RxState = 1;
				pRxPacket = 0;
			}
		}
		else if (RxState == 1)
		{
//			if (Rx_Data == '\r')
//			{
//				RxState = 2;
//			}
			if (Rx_Data!= '\n')
			{
				Rx_Packet[pRxPacket] = Rx_Data;
				pRxPacket++;	
			}
			else
			{		
				RxState = 0;
				Rx_Packet[pRxPacket] = '\0';
				Rx_Flag = 1;
			}
		}
//		else if (RxState == 2)
//		{
//			if (Rx_Data== '\n')
//			{
//				RxState = 0;
//				Rx_Packet[pRxPacket] = '\0';
//				Rx_Flag = 1;
//			}
//		}
		
		USART_ClearITPendingBit(UART4,USART_FLAG_RXNE);
	}
}
