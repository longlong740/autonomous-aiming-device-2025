#include "line.h"
#include "ti_msp_dl_config.h"
#include "motor.h"
#include "Encoder.h"
volatile int16_t Get_Encoder_countA,Get_Encoder_countB;//外部中断捕获脉冲数
uint32_t gpio_interrup1,gpio_interrup2;
/*******************************************************
  函数功能：外部中断模拟编码器信号
  入口函数：无
  返回值：无
 ************************************************************/
void GROUP1_IRQHandler(void)
{
	//获取中断信号
    gpio_interrup1 = DL_GPIO_getEnabledInterruptStatus(LEFT_ENCODER_PORT,LEFT_ENCODER_E1A_PIN|LEFT_ENCODER_E1B_PIN);
    gpio_interrup2 = DL_GPIO_getEnabledInterruptStatus(RIGHT_ENCODER_PORT,RIGHT_ENCODER_E2A_PIN|RIGHT_ENCODER_E2B_PIN);
    
    
	//Get_Encoder_countB 左轮
	if((gpio_interrup1 & LEFT_ENCODER_E1A_PIN)==LEFT_ENCODER_E1A_PIN)
	{
		if(!DL_GPIO_readPins(LEFT_ENCODER_PORT,LEFT_ENCODER_E1B_PIN))
		{
			Get_Encoder_countB--;
		}
		else
		{
			Get_Encoder_countB++;
		}
	}
	else if((gpio_interrup1 & LEFT_ENCODER_E1B_PIN)==LEFT_ENCODER_E1B_PIN)
	{
		if(!DL_GPIO_readPins(LEFT_ENCODER_PORT,LEFT_ENCODER_E1A_PIN))
		{
			Get_Encoder_countB++;
		}
		else
		{
			Get_Encoder_countB--;
		}
	}
	
	//encoderA
	if((gpio_interrup2 & RIGHT_ENCODER_E2A_PIN)==RIGHT_ENCODER_E2A_PIN)
	{
		if(!DL_GPIO_readPins(RIGHT_ENCODER_PORT,RIGHT_ENCODER_E2B_PIN))
		{
			Get_Encoder_countA--;
		}
		else
		{
			Get_Encoder_countA++;
		}
	}
	else if((gpio_interrup2 & RIGHT_ENCODER_E2B_PIN)==RIGHT_ENCODER_E2B_PIN)
	{
		if(!DL_GPIO_readPins(RIGHT_ENCODER_PORT,RIGHT_ENCODER_E2A_PIN))
		{
			Get_Encoder_countA++;
		}                 
		else              
		{                 
			Get_Encoder_countA--;
		}
	}
	DL_GPIO_clearInterruptStatus(LEFT_ENCODER_PORT,LEFT_ENCODER_E1A_PIN|LEFT_ENCODER_E1B_PIN);
	DL_GPIO_clearInterruptStatus(RIGHT_ENCODER_PORT,RIGHT_ENCODER_E2A_PIN|RIGHT_ENCODER_E2B_PIN);
}