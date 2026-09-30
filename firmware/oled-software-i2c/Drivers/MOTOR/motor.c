#include "motor.h"

int abs(int p)
{
	int q;
	q=p>0?p:(-p);
	return q;
}
float absF(float p)
{
	float q;
	q=p>0?p:(-p);
	return q;
}

/********************************************
函数功能：修改PWM波占空比
参数：  duty：占空比0~100
        channel：通道选择
返回值：无
********************************************/
void set_Duty(float duty,uint8_t channel)
{
    uint32_t CompareValue;
    CompareValue = abs((int)(40*duty));                  //占空比转换
    if(channel == 0)//右轮                                 
    {
        DL_TimerG_setCaptureCompareValue(PWM_Car_INST,CompareValue,DL_TIMER_CC_1_INDEX);
    }    
    else if(channel == 1)//左轮
    {
        DL_TimerG_setCaptureCompareValue(PWM_Car_INST,CompareValue,DL_TIMER_CC_0_INDEX);           
    }
}
/********************************************
函数功能：输入PWM波占空比驱动电机
参数：  motor_left：左轮目标占空比
        motor_right：右轮目标占空比
返回值：无
********************************************/
void Set_Pwm(float motor_left,float motor_right)
{
    PWM_Limit(motor_left,-80,80);
    PWM_Limit(motor_right,-80,80);  
	if(motor_left > 0)	//前进
	{
		DL_GPIO_setPins(GPIO_IN_PORT,GPIO_IN_PIN_AIN1_PIN);
	}
	else if(motor_left == 0)//只改占空比，不改方向
	{

	}
	else				//后退
	{
		DL_GPIO_clearPins(GPIO_IN_PORT,GPIO_IN_PIN_AIN1_PIN);
	}
    set_Duty(motor_left,1);//1是左轮
 
	if(motor_right > 0) //前进
	{
		DL_GPIO_setPins(GPIO_IN_PORT,GPIO_IN_PIN_BIN1_PIN);
	}
	else if(motor_right == 0)//只改占空比，不改方向
	{
		
	}	
	else				//后退
	{
        DL_GPIO_clearPins(GPIO_IN_PORT,GPIO_IN_PIN_BIN1_PIN);
	}
    set_Duty(motor_right,0);//0是右轮
}
// void motor_off(void)
// {
//     BIN1_0;
//     BIN2_0;
//     AIN1_0;
//     AIN2_0;
// }
// void motor_on(void)
// {
//     left_on();
//     right_on();
// }
float PWM_Limit(float IN,float max,float min)                   //pwm限幅
{
	float OUT = IN;
	if(OUT > max) OUT = max;
	if(OUT < min) OUT = min;
	return OUT;
}