#ifndef _USART_H_
#define _USART_H_

#include <stdio.h>

extern char Rx_Packet[100];
void Uart4_Init(void);
void Uart4_SendByte(uint8_t Byte);
uint8_t Uart4_GetRxFlag(void);
uint8_t Uart4_GetRxData(void);
char* Uart4_GetRxPacketData(void);
uint8_t Uart4_GetRxDataLength(void);
void Uart4_SendArray(uint8_t *Array,uint16_t Length);
void Uart4_SendString(char *String);
void Uart4_SendNum(uint32_t Number, uint8_t Length);
void Uart4_SendPacket(uint8_t *Array,uint16_t Length);
void Uart4_BeginReceive(int nLen);
uint8_t Uart4_IsReceiveFinish(void);
uint8_t* Uart4_GetReceiveData(void);
#endif
