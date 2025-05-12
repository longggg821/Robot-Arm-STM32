#include "stm32f10x.h"                  // Device header
#include "Delay.h"
//#include "Vehicle_chassis.h"
//#include "PS2.h "
#include "Servo.h"
#include "encoder.h"
//#include "LED.h"
#include "Buzzer.h"
#include "Usart4.h"
#include <string.h>
#include <cstdlib>

//#include "Usart.h"

//#include "SCServo.h"

#include "STS3215_Servo.h"

#define   UNIT_PWM		1           //每次按下按键机械臂舵机PWM高电平时间增加30us
#define   PS2_LSPEED    100		   //遥杆拨到底对应最大目标值（注：此值最大为80）
#define   PS2_RSPEED    50
#define   RECIPROCAL	0.0078f    //128的倒数，此处为方便摇杆值映射目标值的计算。不可更改

uint8_t KeyNum;
uint8_t PS2_Mode,Last_PS2_Mode;
int16_t M1_Target, M2_Target, M3_Target, M4_Target;
uint8_t Time;
char* rxData;
char str_angle[101];
char str_param2[101];
int angle;
int param2;
char* str_angle_end;//the function named strtol's second parameter need a point to receive
char* str_angle_end2;

void pickUp(){
	Robot_Arm_Reset();
	sts3215_move_by_angle(1,7);
//	Delay_ms(1000);
	Robot_UpPwm_Set(110);
	Robot_LeftPwm_Set(140);
	Robot_RightPwm_Set(220);
	Delay_ms(2000);
	Robot_UpPwm_Set(148);
	Delay_ms(1000);
	Robot_RightPwm_Set(140);
	Delay_ms(1000);
}

void putDown(){
	Robot_LeftPwm_Set(220);
	
	sts3215_move_by_angle(1,187);//Robot_PtzPwm_Set(150);//
	Delay_ms(1000);
	
	Robot_RightPwm_Set(170);
	Delay_ms(1000);
	Robot_UpPwm_Set(110);
	Delay_ms(1000);
	Robot_RightPwm_Set(140);
	Robot_UpPwm_Set(147);
	
	sts3215_move_by_angle(1,7);//Robot_PtzPwm_Set(150);//
	
	Delay_ms(1000);
	Robot_LeftPwm_Set(140);
	Delay_ms(1000);
}

uint8_t getNumLength(uint32_t num){
			uint8_t length=1;
			if(num/1000>0){
				length=4;
			}
			else if(num/100>0){
				length=3;
			}
			else if(num/10>0){
				length=2;
			}
			return length;
}



int main(void)
{
	Delay_ms(100);
	Buzzer_Init();
	Buzzer_ON();
	Delay_ms(200);
	Buzzer_OFF();
	Uart4_Init();
	Uart4_SendString("begin\n");
	
//	Delay_ms(1000);
	sts3215_init();
	sts3215_move_by_angle(1,180);
	Delay_ms(1000);
	sts3215_move_by_angle(1,0);
//	Usart1_Init();
//	Usart1_SendString("test\n");
	
	__disable_irq();  
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
//	LED_Init();
//	PS2_Init();
	Servo_Init();
//	Vehicle_Chassis_Init();
	SysTick_Configuration(5);  //最大可以设置中断周期为232；
	
	__enable_irq();
//	RobotArm_RaiseHand(3);
//	RobotArm_StretchHand(40);
//	RobotArm_DropHand(15);
//	RobotArm_ShakeHand(20);

//	setEnd(0);
//	WritePosEx(1, 4095, 2400, 50);
//  Delay_ms((4095-0)*1000/(60*50) + (60*50)*10/(50) + 50);
	

	while(1)
	{	
		if(Uart4_GetRxFlag()){
			rxData=Uart4_GetRxPacketData();
//			strcmp(rxData,"");
			if(rxData[1]!=' ' || (rxData[0]!='l' && rxData[0]!='r' && rxData[0]!='b' && rxData[0]!='f' && rxData[0]!='u' && rxData[0]!='d')){
				continue;
			}
			switch(rxData[0]){
				case 'u':pickUp();break;
				case 'd':putDown();break;
				default:break;
			}
			strncpy(str_angle,rxData+2,Uart4_GetRxDataLength()-2);
			str_angle[Uart4_GetRxDataLength()-2]=0;
			Uart4_SendString(str_angle);
			Uart4_SendByte('\n');
			angle=(int)strtol(str_angle, &str_angle_end, 10);
			
			Uart4_SendString("re:");
			
//			Uart4_SendByte('0'+al);
//			Uart4_SendByte(' ');
			Uart4_SendNum(angle,getNumLength(angle));
			Uart4_SendByte(' ');
			switch(rxData[0]){
				case 'l':Robot_LeftPwm_Set(angle);break;
				case 'r':Robot_RightPwm_Set(angle);break;
				case 'b':
//								strncpy(str_param2,str_angle_end+1,Uart4_GetRxDataLength()-3);
								param2=(int)strtol(str_angle_end+1, &str_angle_end2, 10);
								Uart4_SendNum(param2,getNumLength(param2));
								Uart4_SendByte(' ');
//								Robot_PtzPwm_Set(angle);
								sts3215_move_by_angle(1,angle);
								Delay_ms(param2);
//								Robot_PtzPwm_Set(150);
								break;
				case 'f':Robot_UpPwm_Set(angle);break;
				default:break;
			}
			Uart4_SendByte('\n');
//			Delay_ms(1000);//wait servo to move
//			else if(rxData[0]=='l'){
//			}
//			else if(rxData[0]=='r'){
//			}
//			else if(rxData[0]=='b'){
//			}
//			else if(rxData[0]=='f'){
//			}
		}
//		Delay_ms(1000);
//	Uart4_SendString("loop\n");
//		if (SysTick_Timer_Inquire())
//		{
//			Time++;
//			
//			if ((Time + 1)%2 == 0)                            //遥控控制   25ms
//			{
//				KeyNum = ps2_key_serch();
//				PS2_Mode = ps2_mode_get();				
//					
//				//模式切换时，电机、机械臂复位
//				if (PS2_Mode != Last_PS2_Mode)   
//				{
//					M1_Target = 0;
//					M4_Target = 0;
//					M2_Target = 0;
//					M3_Target = 0;
//					Robot_Arm_Reset();
//				}
//			/**************小车底盘控制*****************/				
//				if(PS2_Mode == PSB_REDLIGHT_MODE)
//				{
//					static unsigned char ps2_lx,ps2_ly,ps2_rx,ps2_ry;
//					ps2_lx = ps2_get_anolog_data(PSS_LX); 
//					ps2_ly = ps2_get_anolog_data(PSS_LY);
//					ps2_rx = ps2_get_anolog_data(PSS_RX);	
//					ps2_ry = ps2_get_anolog_data(PSS_RY);	
//					
//					if (ps2_get_key_state(PSB_L1))  //机械臂控制
//					{
//						if (ps2_lx == 0x00)	RobotArm_PtzLeft(UNIT_PWM);	
//						if (ps2_lx == 0xff) RobotArm_PtzRight(UNIT_PWM);	
//						if (ps2_ly == 0x00)	RobotArm_DropHand(UNIT_PWM);	
//						if (ps2_ly == 0xff) RobotArm_RaiseHand(UNIT_PWM);		
//						if (ps2_ry == 0x00)	RobotArm_StretchHand(UNIT_PWM);	
//						if (ps2_ry == 0xff) RobotArm_ShrinkHand(UNIT_PWM);							
//						if (ps2_rx == 0x00)	RobotArm_ShakeHand(UNIT_PWM);	
//						if (ps2_rx == 0xff) RobotArm_LetHand(UNIT_PWM);			
//				    
//						M1_Target = 0;
//						M4_Target = 0;
//						M2_Target = 0;
//						M3_Target = 0;
//						
//					}
//					else   //小车底盘控制控制
//					{
//						//遥感数据处理
//						int lx_adjust,rx_adjust,ly_adjust;
//						int lx_target = 0, ly_target = 0, rx_target = 0;
//						
//						lx_adjust = ps2_lx - 128;
//						rx_adjust = ps2_rx - 128;
//						ly_adjust = ps2_ly - 127;
//						
//						ly_target = PS2_LSPEED  * ly_adjust * RECIPROCAL;  //1 4同向，2 3反向
//						lx_target = PS2_LSPEED  * lx_adjust * RECIPROCAL;  //1 2同向，3 4反向
//						rx_target = PS2_RSPEED  * rx_adjust * RECIPROCAL;  //1 2同向，3 4反向

//						M1_Target = -ly_target - lx_target + rx_target;
//						M2_Target = +ly_target - lx_target + rx_target; 
//						M3_Target = +ly_target + lx_target + rx_target;
//						M4_Target = -ly_target + lx_target + rx_target;						
//					
//					}
//				}			
//				else if(PS2_Mode == PSB_GREENLIGHT_MODE)
//				{
//					if (ps2_get_key_state(PSB_L1))  //机械臂控制
//					{
//						if (ps2_get_key_state(PSB_PAD_LEFT))   RobotArm_PtzLeft(UNIT_PWM);	
//						if (ps2_get_key_state(PSB_PAD_RIGHT))  RobotArm_PtzRight(UNIT_PWM);	
//						if (ps2_get_key_state(PSB_PAD_UP))	   RobotArm_DropHand(UNIT_PWM);   
//						if (ps2_get_key_state(PSB_PAD_DOWN))   RobotArm_RaiseHand(UNIT_PWM);    
//						if (ps2_get_key_state(PSB_GREEN))	   RobotArm_StretchHand(UNIT_PWM);	
//						if (ps2_get_key_state(PSB_BLUE))       RobotArm_ShrinkHand(UNIT_PWM);								
//						if (ps2_get_key_state(PSB_PINK))	   RobotArm_ShakeHand(UNIT_PWM);	
//						if (ps2_get_key_state(PSB_RED))		   RobotArm_LetHand(UNIT_PWM);											
//					}
//					else   //小车底盘控制控制
//					{	
//						unsigned char up_state,down_state;
//						unsigned char left_state,right_state;
//						unsigned char pink_state,red_state;
//						int lx_target = 0, ly_target = 0, rx_target = 0;
//						
//						up_state = ps2_get_key_state(PSB_PAD_UP);
//						down_state = ps2_get_key_state(PSB_PAD_DOWN);
//						left_state = ps2_get_key_state(PSB_PAD_LEFT);
//						right_state = ps2_get_key_state(PSB_PAD_RIGHT);
//						pink_state = ps2_get_key_state(PSB_PINK);
//						red_state = ps2_get_key_state(PSB_RED);
//						
//						up_state == 1?   ly_target = PS2_LSPEED : (down_state == 1? (ly_target = 0 - PS2_LSPEED) : (ly_target = 0));
//						left_state == 1? lx_target = PS2_LSPEED : (right_state == 1? (lx_target = 0 - PS2_LSPEED) : (lx_target = 0));
//						pink_state == 1? rx_target = PS2_RSPEED : (red_state == 1? (rx_target = 0 - PS2_RSPEED) : (rx_target = 0));

//						M1_Target =  ly_target + lx_target - rx_target;
//						M2_Target = -ly_target + lx_target - rx_target; 
//						M3_Target = -ly_target - lx_target - rx_target;
//						M4_Target =  ly_target - lx_target - rx_target;	
//					}
//				}
//				else
//				{
//					M1_Target = 0;
//					M4_Target = 0;
//					M2_Target = 0;
//					M3_Target = 0;
//					Robot_Arm_Reset();
//				}
//				Last_PS2_Mode = PS2_Mode;		
//				
//			}					
//			if ((Time + 1)%5 == 0)       //电机闭环控制  
//			{
//				//编码器目标值设置
//				Vehicle_Motor1_TargetEC_Set(M1_Target);
//				Vehicle_Motor2_TargetEC_Set(M2_Target);
//				Vehicle_Motor3_TargetEC_Set(M3_Target);
//				Vehicle_Motor4_TargetEC_Set(M4_Target);	
//				Vehicle_Chassis_Run();
//			}
//			if((Time + 1)%200 == 0)
//			{
//				LED_Turn();
//				Time = 0;
//			}

//		}
	}	
}

