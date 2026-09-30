#include "line.h"
#include "ti_msp_dl_config.h"
#include "motor.h"
#include "motor.h"
#include "control.h"
#include "printf.h"
#include "Delay.h"
extern float Start_Yaw;
// extern atk_ms901m_attitude_data_t attitude_dat;
extern float encoderA_cnt;
extern float encoderB_cnt;
extern uint8_t Gray_Line[8];
extern uint8_t Translation;
extern uint8_t Gray_Flag_Cur;
extern uint16_t cx;

float     Kp1=0.5;  //0.65
float	  Ki1=0;
float	  Kd1=0;//0.15

float CurrentA, CurrentB;			//编码器测得当前速度
float targetA=0, targetB=0;			//当前目标速度
volatile float Speed_Middle=15;				//中值速度

uint8_t ledbegin=0,lednum=0,ledflag=0,ledflag1=0,ledflag2=0;
uint8_t begingray=0,grayflag=0;
uint8_t a=0; 

float PID_Gray(void)
{
	static float err, Last_err,Sum_err;
	float Pwm;
	Last_err = err;			//记录误差
	err = Line_Control();  	//计算误差
	Sum_err+=err;				//累加误差	
    Sum_err=PWM_Limit(Sum_err,50,-50);			                                                
	//积分限幅
	Pwm = Kp1 * err+ Ki1 * Sum_err + Kd1 * (err - Last_err);   	//位置式PID控制器
    PWM_Limit(Pwm,Limit, -Limit);
	return Pwm;           
}

volatile PID_t  InnerA, InnerB;
volatile float speedA;
volatile float speedB; //定时器中断中获取脉冲数

PID_t InnerA = {
    .Kp = 10.0f, .Ki = 0.0f, .Kd = 0.0f,
    .OutMax = 100, .OutMin = -100
};

//左轮
PID_t InnerB = {
    .Kp = 9.0f, .Ki = 0.0f, .Kd = 0.0f,
    .OutMax = 100, .OutMin = -100
};



void PID_Update(volatile PID_t *pid)
{
    pid->Error1 = pid->Error0;
    pid->Error0 = pid->Target - pid->Actual;

    if (pid->Ki != 0)
        pid->ErrorInt += pid->Error0;
    else
        pid->ErrorInt = 0;

    pid->Out = pid->Kp * pid->Error0 +
               pid->Ki * pid->ErrorInt +
               pid->Kd * (pid->Error0 - pid->Error1);

    if (pid->Out > pid->OutMax) pid->Out = pid->OutMax;
    if (pid->Out < pid->OutMin) pid->Out = pid->OutMin;
}




uint8_t straight_flag=0;
volatile uint8_t n=0,cnt_flag=1;
uint8_t run=4;
int8_t cycle_speedl=-5,cycle_speedr=12;//3 14+5
uint8_t timeslow=0;

uint8_t buzzer=1;
volatile float bias;                             //差速值

volatile uint8_t stop_flag = 0;
void title_1(void)                       //模式一
{
    float Motor_Left, Motor_Right;            //电机赋值
    Line_Control();   //获取灰度
    if(Gray_Line[0] == 1 && Gray_Line[1] == 1 && Gray_Line[6] == 0 && Gray_Line[7] == 0)
    //13  识别到转弯口                                              14                                  15                                                  24                                25                                          35
    {
        straight_flag=1;//开始直角转弯
    }
    if(straight_flag==1)//转弯
    {
        DL_GPIO_setPins(BUZZER_PORT,BUZZER_PIN_28_PIN);

        begingray=1;//转弯等待0.3s后开始监测是否进入巡线轨道（防止误触）
        if(grayflag==1)        //只有四或者五寻到黑线
        {
            //if(((Gray_Line[3]==1)&&(Gray_Line[2]==1)&&(Gray_Line[1]==1)&&(Gray_Line[0]==0))||((Gray_Line[3]==1)&&(Gray_Line[2]==1)&&(Gray_Line[1]==0)))
            if(((Gray_Line[3] == 1 || Gray_Line[4] == 1) && Gray_Line[2] == 0 && Gray_Line[5] == 0))
            {
                straight_flag=0;
                grayflag=0;
            }
            
        }
        InnerB.Target = cycle_speedl;//B是左轮
        InnerA.Target = cycle_speedr;
        InnerA.Actual = speedA;
        InnerB.Actual = speedB;
        PID_Update(&InnerA);
        PID_Update(&InnerB);

        Motor_Left=PWM_Limit(InnerB.Out,100,-100);
        Motor_Right=PWM_Limit(InnerA.Out,100,-100);  
        Set_Pwm(Motor_Left, Motor_Right);
        // Set_Pwm(cycle_speedl, cycle_speedr);

        cnt_flag=0;
    }
    else if(straight_flag==0) 
    {
        DL_GPIO_clearPins(BUZZER_PORT,BUZZER_PIN_28_PIN);
        if(n==run)//走到指定圈速
        {
            // DL_GPIO_clearPins(GPIO_STBY_PORT,GPIO_STBY_PIN_STBY_PIN);
            stop_flag = 1;
            Set_Pwm(0,0);
            return;
        }
        if(cnt_flag==0)
        {
            cnt_flag=1;
            n=n+1;
        }
        begingray=0;
        grayflag=0;
        //bias = Line_Control();         //差速为灰度返回值
        // bias=PID_Gray();
        // Motor_Left=Speed_Middle+bias;
        // Motor_Right=Speed_Middle-bias;
        // // Motor_Left=Speed_Middle;
        // // Motor_Right=Speed_Middle;
        // Motor_Left=PWM_Limit(Motor_Left,100,-100);
        // Motor_Right=PWM_Limit(Motor_Right,100,-100);  
        // Set_Pwm(Motor_Left, Motor_Right);
        bias=PID_Gray();
        InnerB.Target = Speed_Middle+bias;//B是左轮
        InnerA.Target = Speed_Middle-bias;
        InnerA.Actual = speedA;
        InnerB.Actual = speedB;
        PID_Update(&InnerA);
        PID_Update(&InnerB);

        Motor_Left=PWM_Limit(InnerB.Out,100,-100);
        Motor_Right=PWM_Limit(InnerA.Out,100,-100);  
        Set_Pwm(Motor_Left, Motor_Right);
    }
}

void Control_ABCDA(void)                    //模式二
{
	// line();
	// int Motor_Left, Motor_Right;            //电机赋值
    // float bias;                             //差速值
	
	// if(Gray_Flag_Cur==1)//全白 即走直线
	// {
	// 	if(n==3)DL_GPIO_clearPins(GPIO_STBY_PORT,GPIO_STBY_PIN_STBY_PIN);
	// 	//n为模式切换计数值，用于判断是否跑完全程

	// 	ledflag2=0;			//给黑线的声光提醒复位
	// 	if(ledflag1==0)		//开始白色做一次声光提醒
    //     {
    //         ledflag1=1;
    //         ledbegin=1;
    //     }
	// 	bias = attitude_dat.yaw-Start_Yaw;
	// 	if (n%2==0)
    //     {
    //         if(bias<0) bias = 180 - absF(bias);      //当走过白色区域为偶数时，须将+180和-180转化为0+和0-
    //         else bias = bias - 180;
    //     }
    //     else if(n%2==1)
    //     {
    //         bias = attitude_dat.yaw-Start_Yaw;    //当走过白色区域为奇数时，不需要转化
    //     }
    //     flag=0;
	// 	Speed_Middle=70;
	// }
	// else if(Gray_Flag_Cur==2)       //在黑线区域小车根据灰度返回值循迹
    // {
    //     ledflag1=0;
    //     if(ledflag2==0)
    //     {
    //         ledflag2=1;
    //         ledbegin=1;
    //     }
    //     if(flag==0)
    //     {
    //         flag=1;
    //         n=n+1;
    //     }
    //     bias = line_small();         //差速为灰度返回值
    //     Gray_Flag_Cur=2;
	// 	Speed_Middle=50;
    // }
	
	// targetA = Speed_Middle+bias;                //基于基准值给电机差速
	// targetB = Speed_Middle-bias;
    // CurrentA = encoderA_cnt/3;       //左轮当前转速
	// CurrentB = encoderB_cnt/3;      //右轮当前转速
	// Motor_Left  = PID_A(CurrentA,targetA);
	// Motor_Right = PID_B(CurrentB,targetB);		//PWM限幅
	// Motor_Left=PWM_Limit(Motor_Left,100,-100);
    // Motor_Right=PWM_Limit(Motor_Right,100,-100);  
	// Set_Pwm(Motor_Left, Motor_Right);

}


uint8_t start_flag=0;
void Control_ACBDA(void)
{
	// line();
	// int Motor_Left, Motor_Right;            //电机赋值
    // float bias;                             //差速值

	// if (ledflag==1) DL_GPIO_setPins(GPIO_LED_PORT,GPIO_LED_PIN_led_PIN);    //声光模块驱动判断
    // else DL_GPIO_clearPins(GPIO_LED_PORT,GPIO_LED_PIN_led_PIN);
	
	// if(Gray_Flag_Cur==1)//全白 即走直线
	// {
	// 	if(n==3)DL_GPIO_clearPins(GPIO_STBY_PORT,GPIO_STBY_PIN_STBY_PIN);
	// 	//n为模式切换计数值，用于判断是否跑完全程
	// 	ledflag2=0;			//给黑线的声光提醒复位
	// 	if(ledflag1==0)		//开始白色做一次声光提醒
    //     {
    //         ledflag1=1;
    //         ledbegin=1;
    //         start_flag=1;
    //     }
	// 	bias = attitude_dat.yaw-Start_Yaw;
    //     if (n%2==0)
    //     {
    //         bias=bias-105;
    //         //-105度角为目标值
    //         if(timeslow<=20)
	// 	    Speed_Middle=110;
    //         else Speed_Middle=60;
    //     }
    //     else if(n%2==1)//0度角为目标值
    //     {
    //         bias = attitude_dat.yaw-Start_Yaw;    
    //         //当走过白色区域为奇数时，不需要转化
    //         if(timeslow<=20)
	// 	    Speed_Middle=110;
    //         else Speed_Middle=60;
    //     }
    //     flag=0;
	// }
	// else if(Gray_Flag_Cur==2)       //在黑线区域小车根据灰度返回值循迹
    // {
    //     timeslow=0;
    //     start_flag=0;
    //     ledflag1=0;
    //     if(ledflag2==0)
    //     {
    //         ledflag2=1;
    //         ledbegin=1;
    //     }
    //     if(flag==0)
    //     {
    //         flag=1;
    //         n=n+1;
    //     }
    //     bias = line_small();         //差速为灰度返回值
    //     Gray_Flag_Cur=2;
	// 	Speed_Middle=50;
    // }
	
	// targetA = Speed_Middle+bias;                //基于基准值给电机差速
	// targetB = Speed_Middle-bias;
    // CurrentA = encoderA_cnt/3;       //左轮当前转速
	// CurrentB = encoderB_cnt/3;      //右轮当前转速
	// Motor_Left  = PID_A(CurrentA,targetA);
	// Motor_Right = PID_B(CurrentB,targetB);		//PWM限幅
	// Motor_Left=PWM_Limit(Motor_Left,100,-100);
    // Motor_Right=PWM_Limit(Motor_Right,100,-100);  
	// Set_Pwm(Motor_Left, Motor_Right);

}

void Control_ACBDAx4(void)
{
	// line();
	// int Motor_Left, Motor_Right;            //电机赋值
    // float bias;                             //差速值

	// if (ledflag==1) DL_GPIO_setPins(GPIO_LED_PORT,GPIO_LED_PIN_led_PIN);    //声光模块驱动判断
    // else DL_GPIO_clearPins(GPIO_LED_PORT,GPIO_LED_PIN_led_PIN);
	
	// if(Gray_Flag_Cur==1)//全白 即走直线
	// {
	// 	if(n==9)DL_GPIO_clearPins(GPIO_STBY_PORT,GPIO_STBY_PIN_STBY_PIN);
	// 	//n为模式切换计数值，用于判断是否跑完全程

	// 	ledflag2=0;			//给黑线的声光提醒复位
	// 	if(ledflag1==0)		//开始白色做一次声光提醒
    //     {
    //         ledflag1=1;
    //         ledbegin=1;
    //         start_flag=1;
    //     }
	// 	bias = attitude_dat.yaw-Start_Yaw;
    //     if (n%2==0)
    //     {
    //         bias=bias-105;
    //         //-105度角为目标值
    //         if(timeslow<=20)
	// 	    Speed_Middle=80;
    //         else Speed_Middle=60;
    //     }
    //     else if(n%2==1)//0度角为目标值
    //     {
    //         bias = attitude_dat.yaw-Start_Yaw;    
    //         //当走过白色区域为奇数时，不需要转化
    //         if(timeslow<=20)
	// 	    Speed_Middle=80;
    //         else Speed_Middle=60;
    //     }
    //     flag=0;
	// }
	// else if(Gray_Flag_Cur==2)       //在黑线区域小车根据灰度返回值循迹
    // {
    //     timeslow=0;
    //     start_flag=0;
    //     ledflag1=0;
    //     if(ledflag2==0)
    //     {
    //         ledflag2=1;
    //         ledbegin=1;
    //     }
    //     if(flag==0)
    //     {
    //         flag=1;
    //         n=n+1;
    //     }
    //     bias = line_small();         //差速为灰度返回值
    //     Gray_Flag_Cur=2;
	// 	Speed_Middle=50;
    // }	
	// targetA = Speed_Middle+bias;                //基于基准值给电机差速
	// targetB = Speed_Middle-bias;
    // CurrentA = encoderA_cnt/3;       //左轮当前转速
	// CurrentB = encoderB_cnt/3;      //右轮当前转速
	// Motor_Left  = PID_A(CurrentA,targetA);
	// Motor_Right = PID_B(CurrentB,targetB);		//PWM限幅
	// Motor_Left=PWM_Limit(Motor_Left,100,-100);
    // Motor_Right=PWM_Limit(Motor_Right,100,-100);  
	// Set_Pwm(Motor_Left, Motor_Right);

}
// void SendString(char *str)
// {
// 	while(*str!='\0')
// 	{
// 		DL_UART_Main_transmitDataBlocking(UART_1_INST,*str++);
// 	}


uint8_t i=0,j=0;

void openmv_xunxian(void)
{
    // uint8_t base_speed=10;
    // if(i==0)
    // {
    //     DL_UART_transmitDataBlocking(UART_OPENMV_INST,1);
    //     Delay_s(1);
    //     i=1;
    // }
    // float Motor_Left, Motor_Right;            //电机赋值
    // Motor_Left=base_speed-(cx-320)*0.1;
    // Motor_Right=base_speed+(cx-320)*0.1;
    // Motor_Left=PWM_Limit(Motor_Left,60,-60);
    // Motor_Right=PWM_Limit(Motor_Right,60,-60);  
    // Set_Pwm(Motor_Left, Motor_Right);
}




// float PID_A(float Encoder,float Target)
// {
// 	static float Bias, Last_bias,Sum_bias;
// 	float Pwm;
// 	Last_bias = Bias;			//记录误差
// 	Bias = Target - Encoder;  	//计算误差
// 	Sum_bias+=Bias;				//累加误差	
//     Sum_bias=PWM_Limit(Sum_bias,Limit,-Limit);			                                                
// 	//积分限幅
// 	Pwm = Kp1 * Bias+ Ki1 * Sum_bias + Kd1 * (Bias - Last_bias);   	//位置式PID控制器
// 	PWM_Limit(Pwm,Limit, -Limit);
// 	return Pwm;           
// }

// float PID_B(float Encoder,float Target)
// {
// 	static float Bias, Last_bias,Sum_bias;
// 	float Pwm;
// 	Last_bias = Bias;			//记录误差
// 	Bias = Target - Encoder;  	//计算误差
// 	Sum_bias+=Bias;				//累加误差	
//     Sum_bias=PWM_Limit(Sum_bias,Limit,-Limit);			                                                
// 	//积分限幅

// 	Pwm = Kp1 * Bias+ Ki1 * Sum_bias + Kd1 * (Bias - Last_bias);   	//位置式PID控制器
// 	PWM_Limit(Pwm,Limit, -Limit);
// 	return Pwm;
// }



// void Control_AB(void)                       //模式一
// {
// 	Speed_Middle=110;
//     float Motor_Left, Motor_Right;            //电机赋值
//     float bias;                             //差速值
//     bias = attitude_dat.yaw-Start_Yaw;      //走直线直接将陀螺仪偏差值作为差速值
// 	//监测到黑线停止
// 	line();
// 	if(Gray_Flag_Cur==2&&a==0)
// 	{
// 		DL_GPIO_clearPins(GPIO_STBY_PORT,GPIO_STBY_PIN_STBY_PIN);
// 		ledbegin=1;
// 		a=1;                //停止标志位，判断是否已经停止，使此if仅进入一次
// 	}
//  	if (ledflag==1)     DL_GPIO_setPins(GPIO_LED_PORT,GPIO_LED_PIN_led_PIN);    //声光模块驱动判断
//     else                DL_GPIO_clearPins(GPIO_LED_PORT,GPIO_LED_PIN_led_PIN);

//     targetA = Speed_Middle+bias;                //基于基准值给电机差速
// 	targetB = Speed_Middle-bias;
//     CurrentA = encoderA_cnt/3;       //左轮当前转速
// 	CurrentB = encoderB_cnt/3;      //右轮当前转速
// 	Motor_Left  = PID_A(CurrentA,targetA);
// 	Motor_Right = PID_B(CurrentB,targetB);		//PWM限幅
// 	Motor_Left=PWM_Limit(Motor_Left,100,-100);
//     Motor_Right=PWM_Limit(Motor_Right,100,-100);  
// 	Set_Pwm(Motor_Left, Motor_Right);
	
//     //printf("%f,%f,%f\r\n",targetB,CurrentB,encoderA_cnt);
// 	//printf("targetA:%f\nCurrentA:%f\n",targetA,CurrentA);
// }




// void Control_ACDB(void)                       //模式一
// {
//     float Motor_Left, Motor_Right;            //电机赋值
//     float bias;                             //差速值
    
// 	//监测到黑线停止0
// 	//隔一个识别到了黑线 
//     Line_Control();   
//    //if(((Gray_Line[0]==0)&&(Gray_Line[2]==0))||((Gray_Line[0]==0)&&(Gray_Line[3]==0))||((Gray_Line[0]==0)&&(Gray_Line[4]==0))||((Gray_Line[1]==0)&&(Gray_Line[3]==0))||((Gray_Line[1]==0)&&(Gray_Line[4]==0)))
//     if(f==0)
//     {
//         if(((Gray_Line[0]==0)&&(Gray_Line[2]==0))||((Gray_Line[0]==0)&&(Gray_Line[3]==0))||((Gray_Line[0]==0)&&(Gray_Line[4]==0))||((Gray_Line[1]==0)&&(Gray_Line[3]==0))||((Gray_Line[1]==0)&&(Gray_Line[4]==0)))
//             {straight_flag=1;
//             f=1;}
//     }
//     if(straight_flag==1)
//     {
//         bias = 60+Start_Yaw;      //走直线直接将陀螺仪偏差值作为差速值
//         Speed_Middle=0;
//         if(attitude_dat.yaw>=90)straight_flag=0;
//         targetA = Speed_Middle-bias;                //基于基准值给电机差速
// 	    targetB = Speed_Middle+bias;
//     }
//     else if(straight_flag==0) 
//     {
//         bias = Line_Control();         //差速为灰度返回值
// 		Speed_Middle=60;
//         targetA = Speed_Middle+bias;                //基于基准值给电机差速
// 	    targetB = Speed_Middle-bias;
//     }
//     CurrentA = encoderA_cnt/3;       //左轮当前转速
// 	CurrentB = encoderB_cnt/3;      //右轮当前转速
// 	Motor_Left  = PID_A(CurrentA,targetA);
// 	Motor_Right = PID_B(CurrentB,targetB);		//PWM限幅
// 	Motor_Left=PWM_Limit(Motor_Left,100,-100);
//     Motor_Right=PWM_Limit(Motor_Right,100,-100);  
// 	Set_Pwm(Motor_Left, Motor_Right);
// }