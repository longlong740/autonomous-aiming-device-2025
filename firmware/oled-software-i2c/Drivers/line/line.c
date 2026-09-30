#include "line.h"
#include "ti_msp_dl_config.h"
#include "motor.h"
uint8_t Gray_Line[8];//**
uint8_t Translation = 0;//1:白-->黑
uint8_t Gray_Flag_Cur;//1-白 2-黑
uint8_t Gray_Flag_Old;//1-白 2-黑

int8_t err_gray;
int8_t err_last_gray;



float line(void)
{
    // int8_t Left_Num;
    // int8_t Right_Num;
    uint32_t val  = 0;
    uint8_t Temp = 0;
    //识别到黑色时，Gray_Line[] == 0
    val=DL_GPIO_readPins(Line_PORT,Line__1_PIN|Line__2_PIN|Line__3_PIN|Line__4_PIN|Line__5_PIN|Line__6_PIN|Line__7_PIN|Line__8_PIN);
    if(val & Line__1_PIN) Gray_Line[0] = 1;
    else Gray_Line[0] = 0;
    if(val & Line__2_PIN) Gray_Line[1] = 1;
    else Gray_Line[1] = 0;
    if(val & Line__3_PIN) Gray_Line[2] = 1;
    else Gray_Line[2] = 0;
    if(val & Line__4_PIN) Gray_Line[3] = 1;
    else Gray_Line[3] = 0;
    if(val & Line__5_PIN) Gray_Line[4] = 1;
    else Gray_Line[4] = 0;
    if(val & Line__6_PIN) Gray_Line[5] = 1;
    else Gray_Line[5] = 0;
    if(val & Line__7_PIN) Gray_Line[6] = 1;
    else Gray_Line[6] = 0;
    if(val & Line__8_PIN) Gray_Line[7] = 1;
    else Gray_Line[7] = 0;
    //val记录哪一个GPIO读取到了0，即识别到黑线,赋值给Gray_Line[8]
    
    Temp = 0;
    for(uint8_t i = 0; i < 8; i++)Temp += Gray_Line[i];
    Gray_Flag_Old = Gray_Flag_Cur;
    if(Temp == 8) Gray_Flag_Cur = 1;
    //全白
    else Gray_Flag_Cur = 2;
    //存在黑的
    
    if(Gray_Flag_Old == 1 && Gray_Flag_Cur == 2) Translation = 1;
    //由直线进入巡线模式
    else if(Gray_Flag_Old == 2 && Gray_Flag_Cur == 1) Translation = 2;
    //由巡线进入直线模式

    if(Gray_Line[7] == 0) return 35;
    else if(Gray_Line[6] == 0) return 25;
    else if(Gray_Line[5] == 0) return 14;
    else if(Gray_Line[4] == 0) return 7;

    else if(Gray_Line[0] == 0) return -35;
    else if(Gray_Line[1] == 0) return -25;
    else if(Gray_Line[2] == 0) return -14;
    else if(Gray_Line[3] == 0) return -7;
    else return 0;
    // Left_Num = 0;
    // Right_Num = 0;
    // for(uint8_t i = 0; i < 8; i++)
    // {
    //     if(Gray_Line[i]) Left_Num++;
    //     else break;
    //     //左侧1的数量
    // }
    // for(uint8_t i = 7; i >= 0; i--)
    // {
    //     if(Gray_Line[i]) Right_Num++;
    //     else break;
    //     //右侧1的数量
    // }
    // err_gray=Right_Num - Left_Num;
    // err_last_gray=err_gray;
    
}



float line_small(void)
{
    uint32_t val  = 0;
    uint8_t Temp = 0;

    val=DL_GPIO_readPins(Line_PORT,Line__1_PIN|Line__2_PIN|Line__3_PIN|Line__4_PIN|Line__5_PIN|Line__6_PIN|Line__7_PIN|Line__8_PIN);


    if(val & Line__1_PIN) Gray_Line[0] = 1;
    else Gray_Line[0] = 0;
    if(val & Line__2_PIN) Gray_Line[1] = 1;
    else Gray_Line[1] = 0;
    if(val & Line__3_PIN) Gray_Line[2] = 1;
    else Gray_Line[2] = 0;
    if(val & Line__4_PIN) Gray_Line[3] = 1;
    else Gray_Line[3] = 0;
    if(val & Line__5_PIN) Gray_Line[4] = 1;
    else Gray_Line[4] = 0;
    if(val & Line__6_PIN) Gray_Line[5] = 1;
    else Gray_Line[5] = 0;
    if(val & Line__7_PIN) Gray_Line[6] = 1;
    else Gray_Line[6] = 0;
    if(val & Line__8_PIN) Gray_Line[7] = 1;
    else Gray_Line[7] = 0;
    //val记录哪一个GPIO读取到了0，即识别到黑线,赋值给Gray_Line[8]

    if(Gray_Line[7] == 0) return 20;
    else if(Gray_Line[6] == 0) return 15;
    else if(Gray_Line[5] == 0) return 10;
    else if(Gray_Line[4] == 0) return 2;

    else if(Gray_Line[0] == 0) return -20;
    else if(Gray_Line[1] == 0) return -15;
    else if(Gray_Line[2] == 0) return -10;
    else if(Gray_Line[3] == 0) return -2;
    else return 0;
}

int check_line(void)
{
    if(!DL_GPIO_readPins(Line_PORT,Line__1_PIN)) return true;
    if(!DL_GPIO_readPins(Line_PORT,Line__2_PIN)) return true;
    if(!DL_GPIO_readPins(Line_PORT,Line__3_PIN)) return true;
    if(!DL_GPIO_readPins(Line_PORT,Line__4_PIN)) return true;
    if(!DL_GPIO_readPins(Line_PORT,Line__5_PIN)) return true;
    if(!DL_GPIO_readPins(Line_PORT,Line__6_PIN)) return true;
    if(!DL_GPIO_readPins(Line_PORT,Line__7_PIN)) return true;
    if(!DL_GPIO_readPins(Line_PORT,Line__8_PIN)) return true;
    return false;
}



float Line_Control(void)
{
    uint32_t value  = 0;
    value=DL_GPIO_readPins(Line_PORT,Line__1_PIN|Line__2_PIN|Line__3_PIN|Line__4_PIN|Line__5_PIN|Line__6_PIN|Line__7_PIN|Line__8_PIN);


    if(value & Line__1_PIN) Gray_Line[0] = 1;
    else Gray_Line[0] = 0;
    if(value & Line__2_PIN) Gray_Line[1] = 1;
    else Gray_Line[1] = 0;
    if(value & Line__3_PIN) Gray_Line[2] = 1;
    else Gray_Line[2] = 0;
    if(value & Line__4_PIN) Gray_Line[3] = 1;
    else Gray_Line[3] = 0;
    if(value & Line__5_PIN) Gray_Line[4] = 1;
    else Gray_Line[4] = 0;
    if(value & Line__6_PIN) Gray_Line[5] = 1;
    else Gray_Line[5] = 0;
    if(value & Line__7_PIN) Gray_Line[6] = 1;
    else Gray_Line[6] = 0;
    if(value & Line__8_PIN) Gray_Line[7] = 1;
    else Gray_Line[7] = 0;
    //val记录哪一个GPIO读取到了0，即识别到黑线,赋值给Gray_Line[8]
    if(Gray_Line[7] == 1) return 15;
    else if(Gray_Line[6] == 1) return 9.4;
    else if(Gray_Line[5] == 1) return 5.5;
    else if(Gray_Line[4] == 1) return 1.6;

    else if(Gray_Line[0] == 1) return -15;//左4
    else if(Gray_Line[1] == 1) return -9.4;//左3
    else if(Gray_Line[2] == 1) return -5.5;//左2
    else if(Gray_Line[3] == 1) return -1.6;//左1

    // if(Gray_Line[7] == 1) return 15;
    // else if(Gray_Line[6] == 1) return 6;
    // else if(Gray_Line[5] == 1) return 4;
    // else if(Gray_Line[4] == 1) return 2;

    // else if(Gray_Line[0] == 1) return -15;//左4
    // else if(Gray_Line[1] == 1) return -6;//左3
    // else if(Gray_Line[2] == 1) return -4;//左2
    // else if(Gray_Line[3] == 1) return -2;//左1
    //     //if(Gray_Line[7] == 0) return 20;
    // //else if(Gray_Line[6] == 0) return 15;
    //  if(Gray_Line[5] == 0) return 8;
    // else if(Gray_Line[4] == 0) return 2;

    // //else if(Gray_Line[0] == 0) return -20;
    // //else if(Gray_Line[1] == 0) return -15;
    // else if(Gray_Line[2] == 0) return -8;
    // else if(Gray_Line[3] == 0) return -2;
    else return 0;
}