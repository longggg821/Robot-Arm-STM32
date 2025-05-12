#include "stm32f10x.h"                  // Device header
#include "Vehicle_chassis.h"
#include "Encoder.h"
#include "compute_pid.h"
#include "Motor.h"
#include "Usart.h"

#define  EC_MAX		160    //电机最快转速检测到的编码器脉冲数（周期为25ms）
//Kp, Ki, Kd,err, err_last, integral, output_val;
PID Motor1_pid = {-3.2f, -0.82f, 0, 0, 0, 0, 0, 0};
PID Motor2_pid = {-3.2f, -0.82f, 0, 0, 0, 0, 0, 0};
PID Motor3_pid = {-3.2f, -0.82f, 0, 0, 0, 0, 0, 0};
PID Motor4_pid = {-3.2f, -0.82f, 0, 0, 0, 0, 0, 0};


//4个编码器目标值变量
int target_EC1 = 0, target_EC2 = 0,target_EC3 = 0,target_EC4 = 0;

void Vehicle_Chassis_Init()
{
//	Usart1_Init();
	Motor_Init();
	Encoder_Init();
//	Vehicle_Chassis_Tim_Init();
	Vehicle_Motor1_TargetEC_Set(0);
	Vehicle_Motor2_TargetEC_Set(0);
	Vehicle_Motor3_TargetEC_Set(0);
	Vehicle_Motor4_TargetEC_Set(0);
}

void Vehicle_Motor1_TargetEC_Set(int target_val)
{
	//编码器目标值限幅
	target_val = target_val > EC_MAX?  EC_MAX : target_val;
	target_val = target_val < -EC_MAX?  -EC_MAX : target_val;
	
	target_EC1 = target_val;
}

void Vehicle_Motor2_TargetEC_Set(int target_val)
{
	//编码器目标值限幅
	target_val = target_val > EC_MAX?  EC_MAX : target_val;
	target_val = target_val < -EC_MAX?  -EC_MAX : target_val;
	
	target_EC2 = target_val;
}

void Vehicle_Motor3_TargetEC_Set(int target_val)
{
	//编码器目标值限幅
	target_val = target_val > EC_MAX?  EC_MAX : target_val;
	target_val = target_val < -EC_MAX?  -EC_MAX : target_val;
	
	target_EC3 = target_val;
}

void Vehicle_Motor4_TargetEC_Set(int target_val)
{
	//编码器目标值限3幅
	target_val = target_val > EC_MAX?  EC_MAX : target_val;
	target_val = target_val < -EC_MAX?  -EC_MAX : target_val;
	
	target_EC4 = target_val;
}

/************************************************************/
void Vehicle_Chassis_Run()
{
	int EC1 = 0, EC2 = 0,EC3 = 0, EC4 = 0;
	int M1_Pwm = 0, M2_Pwm = 0, M3_Pwm = 0, M4_Pwm = 0;
	
	Encoder_Updata();
	EC1 = Encoder1_Count_Get();
	EC2 = Encoder2_Count_Get();
	EC3 = Encoder3_Count_Get();
	EC4 = Encoder4_Count_Get();	
	M1_Pwm = Increment_PID_CloseLoop(&Motor1_pid,EC1,target_EC1);
	M2_Pwm = Increment_PID_CloseLoop(&Motor2_pid,EC2,target_EC2);
	M3_Pwm = Increment_PID_CloseLoop(&Motor3_pid,EC3,target_EC3);
	M4_Pwm = Increment_PID_CloseLoop(&Motor4_pid,EC4,target_EC4);
//    printf("%d,%d\n", target_EC1,EC1);
/******************左边**********************/	
//Motor1_SetSpeed(0,1000);
	M1_Pwm >= 0? Motor1_SetSpeed(0,M1_Pwm):Motor1_SetSpeed(1,0-M1_Pwm);
	M4_Pwm >= 0? Motor4_SetSpeed(0,M4_Pwm):Motor4_SetSpeed(1,0-M4_Pwm);
	
/******************右边**********************/	
	M2_Pwm >= 0? Motor2_SetSpeed(1,M2_Pwm):Motor2_SetSpeed(0,0-M2_Pwm);
	M3_Pwm >= 0? Motor3_SetSpeed(1,M3_Pwm):Motor3_SetSpeed(0,0-M3_Pwm);	
}

void Vehicle_Chassis_Tim_Init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_InternalClockConfig(TIM3);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 400 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3, ENABLE);
}

void TIM3_IRQHandler()
{
	if (TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);		
//		
//		Vehicle_Chassis_Run();
	}
}

