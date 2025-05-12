#include "stm32f10x.h"

uint32_t irq_tim = 0;
uint8_t timer_flag = 0;
/**
  * @brief  毫秒级中断
  * @param  xms 中断，范围：0 - 232
  * @retval 无
  */
void SysTick_Configuration(uint32_t xms) 
{
	irq_tim = xms;
    if (xms * 72000 > SysTick_LOAD_RELOAD_Msk) return;  // 检查参数的有效性

    SysTick->LOAD = xms * 72000;                    // 设置定时器重新加载值
	SysTick->VAL = 0x00;
    NVIC_SetPriority(SysTick_IRQn, 0);             // 设置 SysTick 中断的优先级为最高
    SysTick->CTRL = SysTick_CTRL_TICKINT_Msk      // 使能 SysTick 中断
                    | SysTick_CTRL_CLKSOURCE_Msk  // 设置时钟源为内部时钟
                    | SysTick_CTRL_ENABLE_Msk; 				//设置时钟源为HCLK，启动定时器
}

uint8_t SysTick_Timer_Inquire()
{
	if (timer_flag)
	{
		timer_flag = 0;
		return 1;
	}
	return 0;
}

/**
  * @brief  微秒级延时
  * @param  xus 延时时长，范围：0~233015
  * @retval 无
  */
void Delay_us(uint32_t xus)
{
	SysTick->LOAD = 72 * xus;				//设置定时器重装值
	SysTick->VAL = 0x00;					//清空当前计数值
	SysTick->CTRL = 0x00000005;				//设置时钟源为HCLK，启动定时器
	while(!(SysTick->CTRL & 0x00010000));	//等待计数到0
	SysTick->CTRL = 0x00000004;				//关闭定时器
}

/**
  * @brief  毫秒级延时
  * @param  xms 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_ms(uint32_t xms)
{
	while(xms--)
	{
		Delay_us(1000);
	}
}
 
/**
  * @brief  秒级延时
  * @param  xs 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_s(uint32_t xs)
{
	while(xs--)
	{
		Delay_ms(1000);
	}
} 

/********************************************/
void SysTick_Handler()
{
	SysTick_Configuration(irq_tim);
	timer_flag = 1;
	
}

