#include "stm32f10x.h"                  // Device header
#include "encoder.h"

/*
	 ENCODER4   --------- ENCODER3

     ENCODER1   --------- ENCODER2
*/

static struct ENCODER Encoder1={0,0,0,0,0,{0}};
static struct ENCODER Encoder2={0,0,0,0,0,{0}};
static struct ENCODER Encoder3={0,0,0,0,0,{0}};
static struct ENCODER Encoder4={0,0,0,0,0,{0}};
void Encoder_Init()
{
	Encoder_Gpio_Init();
	Encoder_Count_FIFO_Init();
}

int32_t Encoder1_Count_Get()
{
	return Encoder1.Avg_PulseCount;
}

int32_t Encoder2_Count_Get()
{
	return  Encoder2.Avg_PulseCount;
}

int32_t Encoder3_Count_Get()
{
	return Encoder3.Avg_PulseCount;
}

int32_t Encoder4_Count_Get()
{
	return  Encoder4.Avg_PulseCount;
}

void Encoder_Updata()
{
	Encoder1_Updata();
	Encoder2_Updata();
	Encoder3_Updata();
	Encoder4_Updata();
}
/**************************************************/

void Encoder1_Updata()
{
	Encoder1_Count_FIFO_Updata(Encoder1.PulseCount);
	Encoder1_AvgPulseCount_Updata();
	Encoder1.PulseCount = 0;
}

void Encoder2_Updata()
{
	Encoder2_Count_FIFO_Updata(Encoder2.PulseCount);	
	Encoder2_AvgPulseCount_Updata();	
	Encoder2.PulseCount = 0;
}

void Encoder3_Updata()
{
	Encoder3_Count_FIFO_Updata(Encoder3.PulseCount);	
	Encoder3_AvgPulseCount_Updata();	
	Encoder3.PulseCount = 0;
}

void Encoder4_Updata()
{
	Encoder4_Count_FIFO_Updata(Encoder4.PulseCount);	
	Encoder4_AvgPulseCount_Updata();	
	Encoder4.PulseCount = 0;
}

void Encoder_Gpio_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// ʹ��GPIO���ⲿ�ж�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

	// �����ⲿ�ж�����GPIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
								  GPIO_Pin_6 | GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// �����ⲿ�ж���·
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource3);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource6);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource7);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource8);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource9);
	
	
	// �����ⲿ�ж�0
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // �����غ��½��ؾ������ж�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	// �����ⲿ�ж�1
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // �����غ��½��ؾ������ж�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	// �����ⲿ�ж�1
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // �����غ��½��ؾ������ж�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
		// �����ⲿ�ж�1
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // �����غ��½��ؾ������ж�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
		EXTI_InitStructure.EXTI_Line = EXTI_Line6;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // �����غ��½��ؾ������ж�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	// �����ⲿ�ж�1
	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // �����غ��½��ؾ������ж�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	// �����ⲿ�ж�1
	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // �����غ��½��ؾ������ж�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	// �����ⲿ�ж�1
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // �����غ��½��ؾ������ж�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	// �����ⲿ�ж�0���ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// �����ⲿ�ж�1���ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}

/************* ENCODER 1 ***********************/
void EXTI0_IRQHandler() 
{
	if (EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		// �ⲿ�ж�0��������������˶�
		Encoder1.A = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0);
		EXTI_ClearITPendingBit(EXTI_Line0);
		
		if (Encoder1.A == Encoder1.B) 
		{
			Encoder1.PulseCount--;   
		} 
		else
		{
			Encoder1.PulseCount++;    
		}
	}
}

void EXTI1_IRQHandler() 
{
	if (EXTI_GetITStatus(EXTI_Line1) != RESET) 
	{
		// �ⲿ�ж�1��������������˶�
		Encoder1.B = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1);
		EXTI_ClearITPendingBit(EXTI_Line1);
		if (Encoder1.A == Encoder1.B) 
		{
			Encoder1.PulseCount++;    
		} 
		else
		{
			Encoder1.PulseCount--;    
		}
	}
}

/********************************************************/

/************* ENCODER 2 ***********************/
void EXTI2_IRQHandler() 
{
	if (EXTI_GetITStatus(EXTI_Line2) != RESET)
	{
		// �ⲿ�ж�0��������������˶�
		Encoder2.A = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2);
		EXTI_ClearITPendingBit(EXTI_Line2);
		if (Encoder2.A == Encoder2.B) 
		{
			Encoder2.PulseCount--;  
		} 
		else
		{
			Encoder2.PulseCount++;  
		}
	}
}

void EXTI3_IRQHandler() 
{
	if (EXTI_GetITStatus(EXTI_Line3) != RESET) 
	{
		// �ⲿ�ж�1��������������˶�
		Encoder2.B = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3);
		EXTI_ClearITPendingBit(EXTI_Line3);
		//Encoder2_Detect_Direction();
		if (Encoder2.A == Encoder2.B) 
		{
			Encoder2.PulseCount++;
		} 
		else
		{
			Encoder2.PulseCount--;  
		}
		
	}
}

void EXTI9_5_IRQHandler()
{
	if (EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
		// �ⲿ�ж�0��������������˶�
		Encoder3.A = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6);
		EXTI_ClearITPendingBit(EXTI_Line6);
		if (Encoder3.A == Encoder3.B) 
		{
			Encoder3.PulseCount--; 
		} 
		else
		{
			Encoder3.PulseCount++; 
		}
	}	
	else if (EXTI_GetITStatus(EXTI_Line7) != RESET) 
	{
		// �ⲿ�ж�1��������������˶�
		Encoder3.B = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7);
		EXTI_ClearITPendingBit(EXTI_Line7);
		//Encoder2_Detect_Direction();
		if (Encoder3.A == Encoder3.B) 
		{
			Encoder3.PulseCount++;
		} 
		else
		{
			Encoder3.PulseCount--;  // ��ת
		}
	}	
	else if (EXTI_GetITStatus(EXTI_Line8) != RESET) 
	{
		// �ⲿ�ж�1��������������˶�
		Encoder4.A = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8);
		EXTI_ClearITPendingBit(EXTI_Line8);
		//Encoder2_Detect_Direction();
		if (Encoder4.A == Encoder4.B) 
		{
			Encoder4.PulseCount--;
		} 
		else
		{
			Encoder4.PulseCount++;  
		}
	}
	else if (EXTI_GetITStatus(EXTI_Line9) != RESET) 
	{
		// �ⲿ�ж�1��������������˶�
		Encoder4.B = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9);
		EXTI_ClearITPendingBit(EXTI_Line9);
		//Encoder2_Detect_Direction();
		if (Encoder4.A == Encoder4.B) 
		{
			Encoder4.PulseCount++;
		} 
		else
		{
			Encoder4.PulseCount--; 
		}	
	}
}

/********************************************************/
void Encoder_Count_FIFO_Init()
{
	for (uint8_t i = 0; i < COUNT_SIZE; i++)
	{
		Encoder1.Count_FIFO[i] = 0;
		Encoder2.Count_FIFO[i] = 0;
		Encoder3.Count_FIFO[i] = 0;
		Encoder4.Count_FIFO[i] = 0;
	}
}

void Encoder1_Count_FIFO_Updata(int32_t value)
{
	if (Encoder1.pFIFO == COUNT_SIZE)
	{
		for (uint8_t i = 0; i < COUNT_SIZE - 1; i++)
		{
			Encoder1.Count_FIFO[i] = Encoder1.Count_FIFO[i+1];
		}
		Encoder1.pFIFO--;
	}

	Encoder1.Count_FIFO[Encoder1.pFIFO] = value;
	Encoder1.pFIFO++;
}

void Encoder2_Count_FIFO_Updata(int32_t value)
{
	if (Encoder2.pFIFO == COUNT_SIZE)
	{
		for (uint8_t i = 0; i < COUNT_SIZE - 1; i++)
		{
			Encoder2.Count_FIFO[i] = Encoder2.Count_FIFO[i+1];
		}
		Encoder2.pFIFO--;
	}

	Encoder2.Count_FIFO[Encoder2.pFIFO] = value;
	Encoder2.pFIFO++;
}

void Encoder3_Count_FIFO_Updata(int32_t value)
{
	if (Encoder3.pFIFO == COUNT_SIZE)
	{
		for (uint8_t i = 0; i < COUNT_SIZE - 1; i++)
		{
			Encoder3.Count_FIFO[i] = Encoder3.Count_FIFO[i+1];
		}
		Encoder3.pFIFO--;
	}

	Encoder3.Count_FIFO[Encoder3.pFIFO] = value;
	Encoder3.pFIFO++;
}

void Encoder4_Count_FIFO_Updata(int32_t value)
{
	if (Encoder4.pFIFO == COUNT_SIZE)
	{
		for (uint8_t i = 0; i < COUNT_SIZE - 1; i++)
		{
			Encoder4.Count_FIFO[i] = Encoder4.Count_FIFO[i+1];
		}
		Encoder4.pFIFO--;
	}

	Encoder4.Count_FIFO[Encoder4.pFIFO] = value;
	Encoder4.pFIFO++;
}

void Encoder1_AvgPulseCount_Updata()
{
	uint8_t i = 0; 
	int32_t sum = 0;
	
	for(i = 0; i < COUNT_SIZE; i++)
	{
		sum += Encoder1.Count_FIFO[i];
	}
	 
	Encoder1.Avg_PulseCount = sum >> 2;
	
}

void Encoder2_AvgPulseCount_Updata()
{
	uint8_t i = 0; 
	int32_t sum = 0;
	
	for(i = 0; i < COUNT_SIZE; i++)
	{
		sum += Encoder2.Count_FIFO[i];
	}
	 
	Encoder2.Avg_PulseCount = sum >> 2;
}

void Encoder3_AvgPulseCount_Updata()
{
	uint8_t i = 0; 
	int32_t sum = 0;
	
	for(i = 0; i < COUNT_SIZE; i++)
	{
		sum += Encoder3.Count_FIFO[i];
	}
	 
	Encoder3.Avg_PulseCount = sum >> 2;
}

void Encoder4_AvgPulseCount_Updata()
{
	uint8_t i = 0; 
	int32_t sum = 0;
	
	for(i = 0; i < COUNT_SIZE; i++)
	{
		sum += Encoder4.Count_FIFO[i];
	}
	 
	Encoder4.Avg_PulseCount = sum >> 2;
}
