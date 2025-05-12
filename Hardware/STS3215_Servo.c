#include "stm32f10x.h"
#include <stdio.h>
#include "Usart.h"

#include "STS3215_Servo.h"

#define STS3215_CMD_WRITE_DATA 0x03

#define STS3215_ID_DEFAULT 0x01

uint8_t calculateCheck(uint8_t* data,uint8_t len){
	uint16_t count=0;
	for(uint8_t v=0;v<len;v++){
		count+=data[v];
	}
	return ~count & 0xff;
}

void sts3215_sendCmd(uint8_t id, uint8_t len, uint8_t instruction, uint8_t* param){
	uint8_t cmd[4+len];
	cmd[0]=0xff;
	cmd[1]=0xff;
	cmd[2]=id;
	cmd[3]=len;
	cmd[4]=instruction;
	for(int v=0;v<len-2;v++){
		cmd[5+v]=param[v];
	}
	cmd[3+len]=calculateCheck(cmd+2, len+1);
	Usart1_SendArray(cmd, 4+len);
}

void sts3215_cmd_write(uint8_t id, uint8_t addr, uint8_t* param, uint8_t paramLen){
	uint8_t newparam[paramLen+1];
	newparam[0]=addr;
	for(int v=0;v<paramLen;v++){
		newparam[1+v]=param[v];
	}
	sts3215_sendCmd(id, paramLen+3, STS3215_CMD_WRITE_DATA, newparam);
}

void sts3215_move_by_angle(uint8_t id, uint16_t angle){
	uint16_t data=4096*angle/360;//angle*11;//
	uint8_t param[2]={data&0xff,(data>>8)&0xff};
	sts3215_cmd_write(id, 0x2a,param ,2);
}

void sts3215_init(){
	Usart1_Init();
	sts3215_move_by_angle(STS3215_ID_DEFAULT,0);
}
