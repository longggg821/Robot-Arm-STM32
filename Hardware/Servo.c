#include "stm32f10x.h"                  // Device header
#include "Servo.h"

struct SERVO_PARAM robotarm_left = {150, 130, 150, 250};
struct SERVO_PARAM robotarm_up = {150, 80, 150, 185};
struct SERVO_PARAM robotarm_right = {150, 80, 150, 220};
struct SERVO_PARAM robotarm_ptz = {0, 50, 145, 250};	//145
void Servo_Init()
{
	Robot_Arm_Gpio_Init();
	Servo_Time_Init();	
	Robot_Arm_Reset();
}

void Robot_Arm_Reset()
{
	robotarm_left.pwm  = robotarm_left.middle;
	robotarm_up.pwm    = robotarm_up.middle;
	robotarm_right.pwm = robotarm_right.middle;
	robotarm_ptz.pwm   = robotarm_ptz.middle;
	
	Robot_LeftPwm_Set(robotarm_left.middle);
	Robot_RightPwm_Set(robotarm_right.middle);
	Robot_UpPwm_Set(robotarm_up.middle);
	Robot_PtzPwm_Set(robotarm_ptz.middle);	
}

/**
  * @brief  ��е��̧��
  * @param 	unit_pwm�������λת���Ƕ�
  * @retval ��
  */
void RobotArm_RaiseHand(uint8_t unit_pwm)
{
	unit_pwm = robotarm_left.pwm + unit_pwm;	
	Robot_LeftPwm_Set(unit_pwm);
}

/**
  * @brief  ��е�۷�����
  * @param 	unit_pwm�������λת���Ƕ�
  * @retval ��
  */
void RobotArm_DropHand(uint8_t unit_pwm)
{
	unit_pwm = robotarm_left.pwm - unit_pwm;
	Robot_LeftPwm_Set(unit_pwm);
}

/**
  * @brief  ��е������
  * @param 	unit_pwm�������λת���Ƕ�
  * @retval ��
  */
void RobotArm_StretchHand(uint8_t unit_pwm)
{
	unit_pwm = robotarm_right.pwm + unit_pwm;
	Robot_RightPwm_Set(unit_pwm);
}

/**
  * @brief  ��е������
  * @param 	unit_pwm�������λת���Ƕ�
  * @retval ��
  */
void RobotArm_ShrinkHand(uint8_t unit_pwm)
{
	unit_pwm = robotarm_right.pwm - unit_pwm;
	Robot_RightPwm_Set(unit_pwm);
}

/**
  * @brief  ��е������
  * @param 	unit_pwm�������λת���Ƕ�
  * @retval ��
  */
void RobotArm_ShakeHand(uint8_t unit_pwm)
{
	unit_pwm = robotarm_up.pwm + unit_pwm;
	Robot_UpPwm_Set(unit_pwm);
}

/**
  * @brief  ��е������
  * @param 	unit_pwm�������λת���Ƕ�
  * @retval ��
  */
void RobotArm_LetHand(uint8_t unit_pwm)
{
	unit_pwm = robotarm_up.pwm - unit_pwm;
	Robot_UpPwm_Set(unit_pwm);
}

/**
  * @brief  ��е����̨��ת
  * @param 	unit_pwm�������λת���Ƕ�
  * @retval ��
  */
void RobotArm_PtzLeft(uint8_t unit_pwm)
{
	unit_pwm = robotarm_ptz.pwm + unit_pwm;
	Robot_PtzPwm_Set(unit_pwm);
}

/**
  * @brief  ��е����̨��ת
  * @param 	unit_pwm�������λת���Ƕ�
  * @retval ��
  */
void RobotArm_PtzRight(uint8_t unit_pwm)
{
	unit_pwm = robotarm_ptz.pwm - unit_pwm;
	Robot_PtzPwm_Set(unit_pwm);
}
/*************************************/
void Robot_Arm_Gpio_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;  //���� ǰ��� �Ҷ��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	
	//��̨��ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_4);	
}

void Servo_Time_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	TIM_InternalClockConfig(TIM1);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 10-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM1, ENABLE);
}

void Robot_LeftPwm_Set(uint16_t value)
{
	//�޷�
	if (value < robotarm_left.left_limit)
	{
		value = robotarm_left.left_limit;
	}
	else if (value > robotarm_left.right_limit)
	{
		value = robotarm_left.right_limit;
	}
	
	robotarm_left.pwm = value;
}

void Robot_UpPwm_Set(uint16_t value)
{
	//�޷�
	if (value < robotarm_up.left_limit)
	{
		value = robotarm_up.left_limit;
	}
	else if (value > robotarm_up.right_limit)
	{
		value = robotarm_up.right_limit;
	}
	
	robotarm_up.pwm = value;
}

void Robot_RightPwm_Set(uint16_t value)
{
	//�޷�
	if (value < robotarm_right.left_limit)
	{
		value = robotarm_right.left_limit;
	}
	else if (value > robotarm_right.right_limit)
	{
		value = robotarm_right.right_limit;
	}
	
	robotarm_right.pwm = value;
}


void Robot_PtzPwm_Set(uint16_t value)
{
	//�޷�
	if (value < robotarm_ptz.left_limit)
	{
		value = robotarm_ptz.left_limit;
	}
	else if (value > robotarm_ptz.right_limit)
	{
		value = robotarm_ptz.right_limit;
	}
	
	robotarm_ptz.pwm = value;
}

uint16_t counter;
void TIM1_UP_IRQHandler()
{
	if (TIM_GetITStatus(TIM1,TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
		
		counter++;
		if (counter > 2000)
		{
			counter = 0;
		}
		
		//��е������
		if (counter < robotarm_left.pwm)
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_13);
		}
		else
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_13);
		}
		
		//��е��ǰ���
		if (counter < robotarm_up.pwm)
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_14);
		}
		else
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_14);
		}	
			
		//��е���Ҷ��
		if (counter < robotarm_right.pwm)
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_15);
		}
		else
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_15);
		}	
	
		//��е����̨
		if (counter < robotarm_ptz.pwm)
		{
			GPIO_SetBits(GPIOC,GPIO_Pin_4);
		}
		else
		{
			GPIO_ResetBits(GPIOC,GPIO_Pin_4);
		}
	}
}
