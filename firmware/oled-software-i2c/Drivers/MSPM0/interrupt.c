#include "ti_msp_dl_config.h"
#include "interrupt.h"
#include "clock.h"
#include "Encoder.h"


void SysTick_Handler(void)
{
    tick_ms++;
}



//计算捕获的脉冲转化为实际的速度值
// CurrentVelocity_RIGHT=(float)encoderA_cnt / (C) * PI * R;//单位为cm/s
// CurrentVelocity_LEFT=-(float)encoderB_cnt / (C) * PI * R;//单位为cm/s
