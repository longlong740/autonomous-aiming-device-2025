#ifndef __CONTROL_H
#define __CONTROL_H
void title_1(void);
void Control_ABCDA(void);
void Control_ACBDA(void);
void Control_ACBDAx4(void);
void openmv_xunxian(void);
#define  Limit		80						//PWM波限幅，百分比制


typedef struct {
    float Kp, Ki, Kd;
    float Target;
    float Actual;
    float Error0, Error1, ErrorInt;
    float Out;
    float OutMax, OutMin;
} volatile PID_t;
extern volatile PID_t  InnerA, InnerB;
extern volatile float speedA;
extern volatile float speedB; //定时器中断中获取脉冲数
extern volatile float Speed_Middle;
extern uint8_t ledbegin,lednum,ledflag,ledflag1,ledflag2,timeslow,start_flag,begingray,grayflag,straight_flag;
extern volatile uint8_t n,cnt_flag;
extern volatile uint8_t stop_flag;
void PID_Update(PID_t *pid);
#endif