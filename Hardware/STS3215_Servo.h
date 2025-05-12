#ifndef _STS3215_SERVO_H_
#define _STS3215_SERVO_H_

#include <stdio.h>
#include "Usart.h"

uint8_t calculateCheck(uint8_t*,uint8_t);
void sts3215_sendCmd(uint8_t, uint8_t, uint8_t, uint8_t*);
void sts3215_cmd_write(uint8_t, uint8_t, uint8_t*, uint8_t);
void sts3215_move_by_angle(uint8_t, uint16_t);
void sts3215_init(void);
#endif
