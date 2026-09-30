#include "ti_msp_dl_config.h"
#include "main.h"
#include "stdio.h"
// #include "atk_ms901m.h"
#include "motor.h"
#include "line.h"
#include "Delay.h"
#include "control.h"
#include "Encoder.h"
#include "printf.h"
#include "interrupt.h"
#include "uart.h"
#include <string.h>
#include "board.h"
#include "Yuntai.h"

/*
 *                    _ooOoo_
 *                   o8888888o
 *                   88" . "88
 *                   (| -_- |)
 *                    O\ = /O
 *                ____/`---'\____
 *              .   ' \\| |// `.
 *               / \\||| : |||// \
 *             / _||||| -:- |||||- \
 *               | | \\\ - /// | |
 *             | \_| ''\---/'' | |
 *              \ .-\__ `-` ___/-. /
 *           ___`. .' /--.--\ `. . __
 *        ."" '< `.___\_<|>_/___.' >'"".
 *       | | : `- \`.;`\ _ /`;.`/ - ` : | |
 *         \ \ `-. \_ __\ /__ _/ .-` / /
 * ======`-.____`-.___\_____/___.-`____.-'======
 *                    `=---='
 */
char oled_buffer[30];
uint16_t cx;
int8_t motor_left,motor_right;//占空比
 
//extern声明一下方便按键调节数值
extern float Kp1;
extern float Kd1;
extern float Ki1;
extern volatile float Speed_Middle;
extern uint8_t run;//小车转圈标志位控制转几圈
extern volatile uint8_t stop_flag;
extern uint8_t cycle_speedl,cycle_speedr,straight_flag;//转弯时左右轮差速
uint8_t mode=0;     //小车运动模式    ：1，2，3，4，0
uint8_t begin=0;      //小车启动标志位：1启动、0停止
uint8_t jiguang = 0;//1为激光模式 不走title_1
uint8_t turn_delay = 3;
extern volatile uint8_t yuntai_flag;
extern volatile int  x_1, y_1;  
extern volatile float x_p;
extern volatile float y_p;

uint8_t flag = 1;

extern uint8_t Gray_Line[8];


extern volatile uint8_t n,cnt_flag;

// float speedA_target = 15;
// float speedB_target = 15;


extern volatile int16_t Get_Encoder_countA,Get_Encoder_countB;
extern volatile float Speed_Middle;


uint8_t y=0;
uint8_t cycle=1;
uint8_t change=0;
void button(void)
{
        //速度修改
        if(!DL_GPIO_readPins(MODE_PORT, MODE_ctrl_PIN))            //修改小车直行时的速度
        {
            Delay_ms(10);//软件消抖
            if(!DL_GPIO_readPins(MODE_PORT, MODE_ctrl_PIN))
            {
                Speed_Middle=Speed_Middle+1;
            }
            while(!DL_GPIO_readPins(MODE_PORT, MODE_ctrl_PIN));
        }
        //小车启动
        if(!DL_GPIO_readPins(BEGIN_PORT, BEGIN_GO_PIN))       //小车启动
        {
            Delay_ms(10);//软件消抖
            if(!DL_GPIO_readPins(BEGIN_PORT, BEGIN_GO_PIN))
            {
                Delay_ms(700);//启动延时
                // DL_GPIO_setPins(GPIO_STBY_PORT,GPIO_STBY_PIN_STBY_PIN);
                // Set_Pwm(0,0);
                begin=1;
                DL_GPIO_setPins(BUZZER_PORT,BUZZER_PIN_28_PIN);
                Delay_ms(5);
                DL_GPIO_clearPins(BUZZER_PORT,BUZZER_PIN_28_PIN);
            }
            while(!DL_GPIO_readPins(BEGIN_PORT, BEGIN_GO_PIN));
        }
        //状态改变
        if(!DL_GPIO_readPins(Change_PORT, Change_all_PIN))     
        {
            Delay_ms(10);//软件消抖
            if(!DL_GPIO_readPins(Change_PORT, Change_all_PIN))
            {
                change=change+1;
                change=change%6;
            }
            while(!DL_GPIO_readPins(Change_PORT, Change_all_PIN));
        }
        if(change==0)//调节转弯幅度和圈数
        {   
            //调节左轮差速
            if(!DL_GPIO_readPins(PID_PORT, PID_KP_PIN))    
            {
                Delay_ms(10);//软件消抖
                if(!DL_GPIO_readPins(PID_PORT, PID_KP_PIN))
                {
                    // cycle_speedl+=1;
                    jiguang = 1;
                }
                while(!DL_GPIO_readPins(PID_PORT, PID_KP_PIN));
            }
            //调节右轮差速
            if(!DL_GPIO_readPins(PID_PORT, PID_KI_PIN))      
            {
                Delay_ms(10);//软件消抖
                if(!DL_GPIO_readPins(PID_PORT, PID_KI_PIN))
                {
                    // cycle_speedr+=1;
                    turn_delay += 1;
                }
                while(!DL_GPIO_readPins(PID_PORT, PID_KI_PIN));
            }
            //调节转圈数
            if(!DL_GPIO_readPins(PID_PORT, PID_KD_PIN))       
            {
                Delay_ms(10);//软件消抖
                if(!DL_GPIO_readPins(PID_PORT, PID_KD_PIN))
                {
                    cycle=cycle+1;
                    cycle=cycle%6;
                    run=cycle*4;
                }
                while(!DL_GPIO_readPins(PID_PORT, PID_KD_PIN));
            }
        }
        else if(change==1)//调节转向环PID
        {
            //调节kp
            if(!DL_GPIO_readPins(PID_PORT, PID_KP_PIN))    
            {
                Delay_ms(10);//软件消抖
                if(!DL_GPIO_readPins(PID_PORT, PID_KP_PIN))
                {
                    Kp1+=0.05;
                }
                while(!DL_GPIO_readPins(PID_PORT, PID_KP_PIN));
            }
            //调节ki
            if(!DL_GPIO_readPins(PID_PORT, PID_KI_PIN))      
            {
                Delay_ms(10);//软件消抖
                if(!DL_GPIO_readPins(PID_PORT, PID_KI_PIN))
                {
                    Ki1+=0.05;
                }
                while(!DL_GPIO_readPins(PID_PORT, PID_KI_PIN));
            }
            //调节kd
            if(!DL_GPIO_readPins(PID_PORT, PID_KD_PIN))       
            {
                Delay_ms(10);//软件消抖
                if(!DL_GPIO_readPins(PID_PORT, PID_KD_PIN))
                {
                    Kd1+=0.01;
                }
                while(!DL_GPIO_readPins(PID_PORT, PID_KD_PIN));
            }
        }
        else if (change == 2)
        {
            if(!DL_GPIO_readPins(PID_PORT, PID_KP_PIN))    
            {
                Delay_ms(10);//软件消抖
                if(!DL_GPIO_readPins(PID_PORT, PID_KP_PIN))
                {
                    cycle_speedl+=1;
                }
                while(!DL_GPIO_readPins(PID_PORT, PID_KP_PIN));
            }
            
            if(!DL_GPIO_readPins(PID_PORT, PID_KI_PIN))      
            {
                Delay_ms(10);//软件消抖
                if(!DL_GPIO_readPins(PID_PORT, PID_KI_PIN))
                {
                    cycle_speedr+=1;
                }
                while(!DL_GPIO_readPins(PID_PORT, PID_KI_PIN));
            }
        }
        else if(change == 3)
        {
            
            if(!DL_GPIO_readPins(PID_PORT, PID_KP_PIN))    
            {
                Delay_ms(10);//软件消抖
                if(!DL_GPIO_readPins(PID_PORT, PID_KP_PIN))
                {
                    InnerA.Kp+=0.5;
                }
                while(!DL_GPIO_readPins(PID_PORT, PID_KP_PIN));
            }
            
            if(!DL_GPIO_readPins(PID_PORT, PID_KI_PIN))      
            {
                Delay_ms(10);//软件消抖
                if(!DL_GPIO_readPins(PID_PORT, PID_KI_PIN))
                {
                    InnerA.Ki+=0.1;
                }
                while(!DL_GPIO_readPins(PID_PORT, PID_KI_PIN));
            }
            //调节kd
            if(!DL_GPIO_readPins(PID_PORT, PID_KD_PIN))       
            {
                Delay_ms(10);//软件消抖
                if(!DL_GPIO_readPins(PID_PORT, PID_KD_PIN))
                {
                    InnerA.Kd+=0.01;
                }
                while(!DL_GPIO_readPins(PID_PORT, PID_KD_PIN));
            }
        }
        else if(change == 4)
        {
            if(!DL_GPIO_readPins(PID_PORT, PID_KP_PIN))    
            {
                Delay_ms(10);//软件消抖
                if(!DL_GPIO_readPins(PID_PORT, PID_KP_PIN))
                {
                    InnerB.Kp+=0.5;
                }
                while(!DL_GPIO_readPins(PID_PORT, PID_KP_PIN));
            }
            
            if(!DL_GPIO_readPins(PID_PORT, PID_KI_PIN))      
            {
                Delay_ms(10);//软件消抖
                if(!DL_GPIO_readPins(PID_PORT, PID_KI_PIN))
                {
                    InnerB.Ki+=0.1;
                }
                while(!DL_GPIO_readPins(PID_PORT, PID_KI_PIN));
            }
            //调节kd
            if(!DL_GPIO_readPins(PID_PORT, PID_KD_PIN))       
            {
                Delay_ms(10);//软件消抖
                if(!DL_GPIO_readPins(PID_PORT, PID_KD_PIN))
                {
                    InnerB.Kd+=0.01;
                }
                while(!DL_GPIO_readPins(PID_PORT, PID_KD_PIN));
            }            
        }
        else if(change == 5)
        {
            if(!DL_GPIO_readPins(PID_PORT, PID_KP_PIN))    
            {
                Delay_ms(10);//软件消抖
                if(!DL_GPIO_readPins(PID_PORT, PID_KP_PIN))
                {
                    x_p+=0.05;
                }
                while(!DL_GPIO_readPins(PID_PORT, PID_KP_PIN));
            }
            
            if(!DL_GPIO_readPins(PID_PORT, PID_KI_PIN))      
            {
                Delay_ms(10);//软件消抖
                if(!DL_GPIO_readPins(PID_PORT, PID_KI_PIN))
                {
                    y_p+=0.05;
                }
                while(!DL_GPIO_readPins(PID_PORT, PID_KI_PIN));
            }            
        }
}

// volatile int i = 0;
int main(void)
{
    SYSCFG_DL_init();
    SysTick_Init();
    DL_Timer_startCounter(PWM_Car_INST);                      
    //开启pwm电机调速
    NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
    NVIC_ClearPendingIRQ(LEFT_ENCODER_INT_IRQN);                 //清除外部中断标志位
    NVIC_ClearPendingIRQ(RIGHT_ENCODER_INT_IRQN);                 //清除外部中断标志位
    NVIC_ClearPendingIRQ(TIMER_Encoder_Read_INST_INT_IRQN); //清除定时器50ms中断标志位
        // 清除定时器0中断挂起标志
    NVIC_ClearPendingIRQ(TIMER_0_INST_INT_IRQN);
    UART_init();
    // NVIC_ClearPendingIRQ(UART_OPENMV_INST_INT_IRQN);

    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
    NVIC_EnableIRQ(LEFT_ENCODER_INT_IRQN);                       //使能外部中断
    NVIC_EnableIRQ(RIGHT_ENCODER_INT_IRQN);                       //使能外部中断
    NVIC_EnableIRQ(TIMER_Encoder_Read_INST_INT_IRQN);       //使能定时器50ms中断标志位
    // NVIC_EnableIRQ(UART_OPENMV_INST_INT_IRQN);//UART接收中断

    // 使能定时器0中断
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
    DL_Timer_startCounter(TIMER_Encoder_Read_INST);         //50ms定时器开始计时
    
    // Delay_ms(20);

    OLED_Init(); 
    Delay_ms(200);
    
    // Emm_V5_Vel_Control(2,1,10,0,0);
    // Delay_ms(2);
    // Emm_V5_Vel_Control(1, 0, 1000, 10, 0);

//     // 旋转90° (3200脉冲/圈 ÷ 4 = 800脉冲)
//     Emm_V5_Pos_Control(
//     0x02,           // 地址1
//     1,              // CW方向
//     400,            // 400 RPM
//     80,             // 中等加速度
//     50,            // 90°对应的脉冲数 (3200/4)
//     false,          // 相对运动
//     false         // 多机同步标志
// );

// Delay_ms(2);
//     Emm_V5_Pos_Control(
//         0x01,           // 地址1
//         0,              // CW方向
//         400,            // 400 RPM
//         80,             // 中等加速度
//         250,            // 90°对应的脉冲数 (3200/4)
//         false,          // 相对运动
//         false           // 多机同步标志
//     );
  
    //OLED初始化 
    y=1;//不让定时器里面的内容过早的执行

    // straight_flag=1;

    // Set_Pwm(10,0);
    // UART3_printfDMA("%c","d");
    // yuntai_motor_control(-5,0);
    // Delay_ms(2);
    // DL_GPIO_clearPins(GPIO_jiguang_PORT, GPIO_jiguang_PIN_0_PIN);
    while (1) 
    {     
        // printf("%d,%d\n",n,cnt_flag);
        // printf("%d,%d\n\r",1,1);
        // printf("%d",Gray_Line[0]);
        button();//按键脱机修改参数
        // printf("%d,%d\n\r",x_1,y_1);
        // send_str(UART0,(uint8_t *)Speed_Middle);
        // send_str(UART0,(uint8_t *)InnerA.Actual);
        if(change==0)
        {
            sprintf(oled_buffer, "jiguang:%d", jiguang);
            OLED_ShowString(0,1,(uint8_t *)oled_buffer,8);
            sprintf(oled_buffer, "turndelay:%d",turn_delay);
            OLED_ShowString(0,2,(uint8_t *)oled_buffer,8);           
            sprintf(oled_buffer, "cycle:%d",cycle);
            OLED_ShowString(0,3,(uint8_t *)oled_buffer,8);
        }
        else if(change==1)
        {
            sprintf(oled_buffer, "KP:%.2f Kd:%.2f      ", Kp1,Kd1);
            OLED_ShowString(0,1,(uint8_t *)oled_buffer,8);
            sprintf(oled_buffer, "KI:%.2f         ",Ki1);
            OLED_ShowString(0,2,(uint8_t *)oled_buffer,8);
            sprintf(oled_buffer,"                        ");
            OLED_ShowString(0,3,(uint8_t *)oled_buffer,8);
        }
        else if(change == 2)
        {
            sprintf(oled_buffer, "cycle_speed1:%d       ", cycle_speedl);
            OLED_ShowString(0,1,(uint8_t *)oled_buffer,8);
            sprintf(oled_buffer, "cycle_speedr:%d             ", cycle_speedr);
            OLED_ShowString(0,2,(uint8_t *)oled_buffer,8);
            sprintf(oled_buffer,"                            ");
            OLED_ShowString(0,3,(uint8_t *)oled_buffer,8);
        }
        else if(change == 3)
        {
            sprintf(oled_buffer, "right_KP:%.2f Kd:%.2f      ", InnerA.Kp,InnerA.Kd);
            OLED_ShowString(0,1,(uint8_t *)oled_buffer,8);
            sprintf(oled_buffer, "KI:%.2f                ",InnerA.Ki);
            OLED_ShowString(0,2,(uint8_t *)oled_buffer,8);            
            sprintf(oled_buffer,"                   ");
            OLED_ShowString(0,3,(uint8_t *)oled_buffer,8);
        }
        else if(change == 4)
        {
            sprintf(oled_buffer, "left_KP:%.2f Kd:%.2f", InnerB.Kp,InnerB.Kd);//左轮
            OLED_ShowString(0,1,(uint8_t *)oled_buffer,8);
            sprintf(oled_buffer, "KI:%.2f",InnerB.Ki);
            OLED_ShowString(0,2,(uint8_t *)oled_buffer,8);     
                        sprintf(oled_buffer,"           ");
            OLED_ShowString(0,3,(uint8_t *)oled_buffer,8);          
        }
        else if(change == 5)
        {
            sprintf(oled_buffer, "X_KP:%.2f Y_KP:%.2f", x_p,y_p);//左轮
            OLED_ShowString(0,1,(uint8_t *)oled_buffer,8);           
                        sprintf(oled_buffer,"              ");
            OLED_ShowString(0,3,(uint8_t *)oled_buffer,8);
                        sprintf(oled_buffer,"             ");
            OLED_ShowString(0,3,(uint8_t *)oled_buffer,8); 
        }
        sprintf(oled_buffer, "speed:%f",Speed_Middle);
        OLED_ShowString(0,0,(uint8_t *)oled_buffer,8);

    }
}

void TIMER_0_INST_IRQHandler(void)
{
    //如果产生了定时器中断
    switch( DL_TimerG_getPendingInterrupt(TIMER_0_INST) )
    {
        case DL_TIMER_IIDX_ZERO://如果是0溢出中断
                //             DL_GPIO_setPins(BUZZER_PORT,BUZZER_PIN_28_PIN);
                // Delay_ms(2);
                // DL_GPIO_clearPins(BUZZER_PORT,BUZZER_PIN_28_PIN);
            if(yuntai_flag == 1 && begin == 1 && jiguang == 1)
           {
                if(flag == 1)
                {
                DL_GPIO_clearPins(GPIO_jiguang_PORT, GPIO_jiguang_PIN_0_PIN);
                    flag = 0;
                }

                controlGimbal(x_1,y_1); 
                yuntai_motor_control(speed_x,speed_y);

                yuntai_flag = 0;
           }
                    
                
            break;
        default://其他的定时器中断
            break;
    }
}


void TIMER_Encoder_Read_INST_IRQHandler(void)//50ms定时中断
{
	if(DL_TimerA_getPendingInterrupt(TIMER_Encoder_Read_INST))
	{
		if(DL_TIMER_IIDX_ZERO)
		{
        //获取编码器计数值
            speedA = (float)-Get_Encoder_countA;//A是右轮
            speedB = (float)Get_Encoder_countB;
            Get_Encoder_countA = 0;
            Get_Encoder_countB = 0;

                // DL_GPIO_togglePins(BUZZER_PORT,BUZZER_PIN_28_PIN);

        if (y==1) 
        {
            //函数逻辑
            if (begin==1  && jiguang == 0)
            {
                title_1();//执行绕任意圈数
                if(stop_flag == 1)
                    begin = 0;
                /* 内环速度 PID */
                
                // InnerA.Actual = speedA;//B是左轮
                // InnerB.Actual = speedB;
                // PID_Update(&InnerA);
                // PID_Update(&InnerB);

                // Motor_Left=PWM_Limit(InnerB.out,100,-100);
                // Motor_Right=PWM_Limit(InnerA.out,100,-100);  
                // Set_Pwm(Motor_Left, Motor_Right);
            }
    
                      
        }
 
            if(begingray==1)
            {
                if(timeslow==turn_delay)
                {
                    timeslow=0;
                    begingray=0;
                    grayflag=1;
                }
                timeslow++;
            }
        }

	}
}

// //下面的都用不上
// uint16_t rxbuf;
// void UART_OPENMV_INST_IRQHandler(void)
// {
//     uint8_t gData;
//     switch(DL_UART_Main_getPendingInterrupt(UART_OPENMV_INST))
//     {
//         case DL_UART_MAIN_IIDX_RX:
//             gData = DL_UART_Main_receiveData(UART_OPENMV_INST);
//             // if(gData =='s')
//             if(gData =='#')rxbuf =0;
//             else if(gData =='$')
//             cx = rxbuf;
//             else
//             rxbuf = rxbuf*10 +(gData -'0');
//             break;
//         default:
//             break;
//     }


// }

void HardFault_Handler(void)
{
     __BKPT();
}




