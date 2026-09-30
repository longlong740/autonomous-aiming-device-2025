#ifndef  _MOTOR_H_
#define  _MOTOR_H_	  
#include "ti_msp_dl_config.h"

/*******直流减速电机机械参数*********/
#define ENCODE_X 13 //编码器线数
#define JIANSUBI 28		//减速比
#define BEIPIN 2			//倍频
#define SAMPLE_TIME 0.05  //采样时间10ms
#define C (ENCODE_X*JIANSUBI*BEIPIN*SAMPLE_TIME)
#define PI 3.1415
#define R  6.5   //车轮直径cm 
float PWM_Limit(float IN,float max,float min);
int abs(int p);//取绝对值
float absF(float p);
void Set_Pwm(float motor_left,float motor_right);
void set_Duty(float duty,uint8_t channel);

void left_on();
void right_on();
void left_back();
void right_back();
void motor_off(void);
void motor_on();
#define AIN1_0 DL_GPIO_clearPins(GPIO_IN_PIN_AIN1_PORT,GPIO_IN_PIN_AIN1_PIN);
#define AIN1_1 DL_GPIO_setPins(GPIO_IN_PIN_AIN1_PORT,GPIO_IN_PIN_AIN1_PIN);
#define AIN2_0 DL_GPIO_clearPins(GPIO_IN_PIN_AIN2_PORT,GPIO_IN_PIN_AIN2_PIN);
#define AIN2_1 DL_GPIO_setPins(GPIO_IN_PIN_AIN2_PORT,GPIO_IN_PIN_AIN2_PIN);

#define BIN1_0 DL_GPIO_clearPins(GPIO_IN_PIN_BIN1_PORT,GPIO_IN_PIN_BIN1_PIN);
#define BIN1_1 DL_GPIO_setPins(GPIO_IN_PIN_BIN1_PORT,GPIO_IN_PIN_BIN1_PIN);
#define BIN2_0 DL_GPIO_clearPins(GPIO_IN_PIN_BIN2_PORT,GPIO_IN_PIN_BIN2_PIN);
#define BIN2_1 DL_GPIO_setPins(GPIO_IN_PIN_BIN2_PORT,GPIO_IN_PIN_BIN2_PIN);
#endif
