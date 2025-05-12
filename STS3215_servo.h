#ifndef __STS3215_SERVO_H
#define __STS3215_SERVO_H

#include <stdio.h>
#include "Usart.h"

//#define STS3215_CMD_WRITE_DATA 0x03

//#define STS3215_ID_DEFAULT 0x01

uint8_t calculateCheck(uint8_t*,uint8_t);
void sts3215_sendCmd(uint8_t, uint8_t, uint8_t, uint8_t*);
void sts3215_cmd_write(uint8_t, uint8_t, uint8_t*, uint8_t);
void sts3215_move_by_angle(uint8_t, uint16_t);
void sts3215_init(void);
#endif
