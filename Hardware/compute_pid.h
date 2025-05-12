#ifndef _COMPUTE_PID_H
#define _COMPUTE_PID_H

#include<stdio.h>
#include<stdlib.h>

//typedef struct {
//	float Kp;
//	float Ki;
//	float Kd;
//	float Ts;
//	int Error;
//	int Error_prev;
//	int Error_prevlast;
//	float Proportional;
//	float Integral;
//	float Integral_prev;
//	float Derivative;
//	int Output;
//}PID;


//float Increment_PID_CloseLoop(PID *pid,int Encoder,int Target);  //闭环增量式PID 
typedef struct
{
	float Kp,Ki,Kd;   //比例、积分、微分系数
	int err;          //偏差值
	int err_pre;	  //上一次误差
	int err_last;     //上上此误差
	int output;   	  //输出值
	int output_last;  //上一次输出
	
}PID;

void Set_Pid_Target(PID *pid,int temp_val);
int Increment_PID_CloseLoop(PID *pid, int actual_val, int target_val);
#endif

