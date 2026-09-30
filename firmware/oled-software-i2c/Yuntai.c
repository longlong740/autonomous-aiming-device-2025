#include "Yuntai.h"

// void uart0_send_data(const uint8_t *data, uint16_t len)
// {
//     for (uint16_t i = 0; i < len; i++) {
//         // 等待发送就绪
//         while (DL_UART_isBusy(UART_3_INST) == true);
//         // 发送单字节
//         DL_UART_Main_transmitData(UART_3_INST, data[i]);
//     }
// }

volatile float x_p =  0.15f;//0.35
volatile float y_p = -0.15f;//-0.25

uint8_t cmd[32] = {0};

/**
  * @brief    读取系统参数
  * @param    addr  ：电机地址
  * @param    s     ：系统参数类型
  * @retval   地址 + 功能码 + 命令状态 + 校验字节
  */
void Emm_V5_Read_Sys_Params(uint8_t addr, SysParams_t s)
{
  uint8_t i = 0;
  memset(cmd,0,sizeof(cmd));
  
  // 装载命令
  cmd[i] = addr; ++i;                   // 地址

  switch(s)                             // 功能码
  {
    case S_VER  : cmd[i] = 0x1F; ++i; break;
    case S_RL   : cmd[i] = 0x20; ++i; break;
    case S_PID  : cmd[i] = 0x21; ++i; break;
    case S_VBUS : cmd[i] = 0x24; ++i; break;
    case S_CPHA : cmd[i] = 0x27; ++i; break;
    case S_ENCL : cmd[i] = 0x31; ++i; break;
    case S_TPOS : cmd[i] = 0x33; ++i; break;
    case S_VEL  : cmd[i] = 0x35; ++i; break;
    case S_CPOS : cmd[i] = 0x36; ++i; break;
    case S_PERR : cmd[i] = 0x37; ++i; break;
    case S_FLAG : cmd[i] = 0x3A; ++i; break;
    case S_ORG  : cmd[i] = 0x3B; ++i; break;
    case S_Conf : cmd[i] = 0x42; ++i; cmd[i] = 0x6C; ++i; break;
    case S_State: cmd[i] = 0x43; ++i; cmd[i] = 0x7A; ++i; break;
    default: break;
  }

  cmd[i] = 0x6B; ++i;                   // 校验字节
  
  // 发送命令
  uart3_send_data(cmd, i);
}

/**
  * @brief    速度模式
  * @param    addr：电机地址
  * @param    dir ：方向       ，0为CW，其余值为CCW
  * @param    vel ：速度       ，范围0 - 5000RPM
  * @param    acc ：加速度     ，范围0 - 255，注意：0是直接启动
  * @param    snF ：多机同步标志，false为不启用，true为启用
  * @retval   地址 + 功能码 + 命令状态 + 校验字节
  */
void Emm_V5_Vel_Control(uint8_t addr, uint8_t dir, uint16_t vel, uint8_t acc, bool snF)
{
//  uint8_t cmd[16] = {0};
   memset(cmd,0,sizeof(cmd));
  // 装载命令
  cmd[0] =  addr;                       // 地址
  cmd[1] =  0xF6;                       // 功能码
  cmd[2] =  dir;                        // 方向
  cmd[3] =  (uint8_t)(vel >> 8);        // 速度(RPM)高8位字节
  cmd[4] =  (uint8_t)(vel >> 0);        // 速度(RPM)低8位字节
  cmd[5] =  acc;                        // 加速度，注意：0是直接启动
  cmd[6] =  snF;                        // 多机同步运动标志
  cmd[7] =  0x6B;                       // 校验字节
  
  // 发送命令
//  HAL_UART_Transmit(&huart5, (uint8_t *)cmd, 8,10);
//  HAL_UART_Transmit(&huart6, (uint8_t *)cmd, 8,10);
		uart3_send_data(cmd, 8);
	
}


/**
  * @brief    位置模式
  * @param    addr：电机地址
  * @param    dir ：方向        ，0为CW，其余值为CCW
  * @param    vel ：速度(RPM)   ，范围0 - 5000RPM
  * @param    acc ：加速度      ，范围0 - 255，注意：0是直接启动
  * @param    clk ：脉冲数      ，范围0- (2^32 - 1)个
  * @param    raF ：相位/绝对标志，false为相对运动，true为绝对值运动
  * @param    snF ：多机同步标志 ，false为不启用，true为启用
  * @retval   地址 + 功能码 + 命令状态 + 校验字节
  */
void Emm_V5_Pos_Control(uint8_t addr, uint8_t dir, uint16_t vel, uint8_t acc, uint32_t clk, bool raF, bool snF)
{
//  uint8_t cmd[16] = {0};
   memset(cmd,0,sizeof(cmd));
  // 装载命令
  cmd[0]  =  addr;                      // 地址
  cmd[1]  =  0xFD;                      // 功能码
  cmd[2]  =  dir;                       // 方向
  cmd[3]  =  (uint8_t)(vel >> 8);       // 速度(RPM)高8位字节
  cmd[4]  =  (uint8_t)(vel >> 0);       // 速度(RPM)低8位字节 
  cmd[5]  =  acc;                       // 加速度，注意：0是直接启动
  cmd[6]  =  (uint8_t)(clk >> 24);      // 脉冲数(bit24 - bit31)
  cmd[7]  =  (uint8_t)(clk >> 16);      // 脉冲数(bit16 - bit23)
  cmd[8]  =  (uint8_t)(clk >> 8);       // 脉冲数(bit8  - bit15)
  cmd[9]  =  (uint8_t)(clk >> 0);       // 脉冲数(bit0  - bit7 )
  cmd[10] =  raF;                       // 相位/绝对标志，false为相对运动，true为绝对值运动
  cmd[11] =  snF;                       // 多机同步运动标志，false为不启用，true为启用
  cmd[12] =  0x6B;                      // 校验字节
  
  // 发送命令
//  HAL_UART_Transmit(&huart5, (uint8_t *)&cmd, 13,10);
		uart3_send_data(cmd, 13);
}

// 云台电机控制函数
void yuntai_motor_control(int16_t motor_x, int16_t motor_y) 
{
    
    // 地址 方向 转速 加速度 多机同步标志
    if(motor_x < 0)
    {
        motor_x = -motor_x;
        Emm_V5_Vel_Control(2, 1, motor_x, 0, false); // 底层电机
        // Emm_V5_Pos_Control(1,0,15,0,motor_x,false,false);
    }
    else
    {
        Emm_V5_Vel_Control(2, 0, motor_x, 0, false); // 底层电机
        // Emm_V5_Pos_Control(1,1,15,0,motor_x,false,false);
    }
    Delay_ms(2);

    if(motor_y < 0)
    {
        motor_y = -motor_y;
        // Emm_V5_Pos_Control(2,0,15,0,motor_x,false,false);
        Emm_V5_Vel_Control(1, 0, motor_y, 0, false); // 高层电机
    }
    else
    {
        // Emm_V5_Pos_Control(2,1,15,0,motor_x,false,false);
        Emm_V5_Vel_Control(1, 1, motor_y, 0, false); // 高层电机
    }
    Delay_ms(2);
}

// 全局变量声明
float last_error_x;  // 上一次X轴误差
float last_error_y;  // 上一次Y轴误差

// 图像参数
int wide = 240;  // 图像宽度
int high = 320;  // 图像高度

// 激光安装误差（需实际标定）
float offset_x = 0.0f;  // X轴补偿
float offset_y = 0.0f; // Y轴补偿

// 电机参数
float speed_low = 100;   // X轴最大速度
float speed_high = 100;  // Y轴最大速度

float speed_x ;
float speed_y ;// 输出的电机速度

// 在文件全局变量区添加滤波状态变量
static float filtered_error = 0.0f; // 保存滤波后的error值
static float prev_filtered_error = 0.0f; // 前一次滤波结果

// 一阶低通滤波函数（简化版）
int intlowPassFilter(int input, int* prev_out, float alpha) {
    int output = alpha * input + (1 - alpha) * *prev_out;
    *prev_out = output;
    return output;
}

float lowPassFilter(float input, float* prev_out, float alpha) {
    float output = alpha * input + (1 - alpha) * *prev_out;
    *prev_out = output;
    return output;
}

// 云台控制函数
void controlGimbal(int current_x, int current_y) 
{
    static int current_x_last,current_y_last;
    static int current_x_now,current_y_now;
    current_x_now = intlowPassFilter(current_x,&current_x_last,0.8);
    current_y_now = intlowPassFilter(current_y,&current_y_last,0.8);

    current_x_last=current_x;
    current_y_last=current_y;

    // 1. 计算目标靶心坐标（图像中心+激光偏移）
    const float target_x = wide / 2.0f + offset_x;
    const float target_y = high / 2.0f + offset_y;

    // 2. 计算当前误差（靶心坐标 - 当前坐标）
    const float error_x = target_x - current_x_now;
    const float error_y = target_y - current_y_now;

    // 3. 计算误差变化率（当前误差 - 上次误差）
    const float delta_error_x = error_x - last_error_x;
    const float delta_error_y = error_y - last_error_y;

    static float raw_speed_x_now,raw_speed_y_now;
    static float raw_speed_x_last,raw_speed_y_last;

    // 4. 保存当前误差供下次使用
    last_error_x = error_x;
    last_error_y = error_y;

    // 5. 计算X轴速度（比例项 + 微分项）
    float raw_speed_x = (error_x * x_p + delta_error_x * 0.0f)*0.4;
    // float raw_speed_x = (error_x * 0.0f + delta_error_x * 0.0f)*0.4;


    // 6. 计算Y轴速度（比例项 + 微分项）
    float raw_speed_y = (error_y * -0.25f + delta_error_y * 0.0f)*0.4;

    raw_speed_x_now = lowPassFilter(raw_speed_x,&raw_speed_x_last,0.9);
    raw_speed_y_now = lowPassFilter(raw_speed_y,&raw_speed_y_last,0.9);

    raw_speed_x_last = raw_speed_x;
    raw_speed_y_last = raw_speed_y;

    // 7. 应用速度限制 (X轴限制在±speed_low)
    if (fabs(raw_speed_x_now) > speed_low) {
        speed_x = (raw_speed_x_now > 0)? speed_low : -speed_low;
    } else {
        speed_x = raw_speed_x_now;
    }

    // 8. 应用速度限制 (Y轴限制在±speed_high)
    if (fabs(raw_speed_y_now) > speed_high) {
        speed_y = (raw_speed_y_now > 0)? speed_high : -speed_high;
    } else {
        speed_y = raw_speed_y_now;
    }

    // 9. 当接近靶心时强制速度为0（避免抖动）
    if (fabs(error_x) < 1.0f && fabs(error_y) < 1.0f) {
        speed_x = 0;
        speed_y = 0;
        
    }

    // printf("%.2f,%.2f\n",speed_x,speed_y);
}


