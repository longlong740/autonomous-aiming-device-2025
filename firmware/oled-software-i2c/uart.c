#include "uart.h"
#include "ti_msp_dl_config.h"
#include <string.h>

volatile uint8_t UART3TxDMADone = 1; // UART3发送DMA完成标志
volatile uint8_t rx_data;
/* ----------------- 全局变量 ----------------------------*/
volatile uint8_t  rxBuffer[MAX_RX_LEN];
volatile uint8_t  rxIndex   = 0;
volatile bool     rxComplete = false;

volatile int find, x_1, y_1;           // 业务变量，可按需要改名
volatile uint8_t yuntai_flag = 0;

// void UART_init(void) 
// {
//     //清除定时器中断标志
//     NVIC_ClearPendingIRQ(UART_3_INST_INT_IRQN);
//     //使能定时器中断
//     NVIC_EnableIRQ(UART_3_INST_INT_IRQN);


// }


void uart3_send_data(const uint8_t *data, uint16_t len)
{
    for(uint16_t i=0;i< len; i++)
    {// 等待发送就绪
    while (DL_UART_isBusy(UART_3_INST)== true);
    // 发送单字节
    DL_UART_Main_transmitData(UART_3_INST, data[i]);
    }
}


// UART3中断服务函数
void UART3_IRQHandler(void) {
    switch (DL_UART_getPendingInterrupt(UART_3_INST)) {
        case DL_UART_IIDX_RX:
            rx_data = DL_UART_Main_receiveData(UART_3_INST);
 
            // UART3_sendStrDMA("str",strlen("str"));
        //     break;
        default:
            break;
    }
}

void UART_init(void) 
{
    // //清除定时器中断标志
    // NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
    // //使能定时器中断
    // NVIC_EnableIRQ(UART_0_INST_INT_IRQN);

        //清除定时器中断标志
    NVIC_ClearPendingIRQ(UART_3_INST_INT_IRQN);
    //使能定时器中断
    NVIC_EnableIRQ(UART_3_INST_INT_IRQN);


}
void uart0_send_data(const uint8_t *data, uint16_t len)
{
    for(uint16_t i=0;i< len; i++)
    {// 等待发送就绪
    while (DL_UART_isBusy(UART_0_INST)== true);
    // 发送单字节
    DL_UART_Main_transmitData(UART_0_INST, data[i]);
    }
}


/* --------------- 发送函数 -----------------------------*/
void uart0_send_char(char ch)
{
    while (DL_UART_isBusy(UART_0_INST)) ;
    DL_UART_Main_transmitData(UART_0_INST, (uint8_t)ch);
}

void uart0_send_string(const char *str)
{
    while (*str)
        uart0_send_char(*str++);
}

/* --------------- 字符串转整数 -------------------------*/
int asciiToInt(const char *str)
{
    int result = 0;
    while (*str >= '0' && *str <= '9')
    {
        result = result * 10 + (*str - '0');
        str++;
    }
    return result;
}

// /* --------------- 数据解析 ----------------------------*/
void processUARTData(void)
{
    if (rxComplete) 
    {
        rxComplete = false;                         // 先清标志

    /* 按逗号拆出三段数字 */
        char *token = strtok((char *)rxBuffer, ",");
        int tempX = asciiToInt(token);

        token = strtok(NULL, ",");
        int tempY = asciiToInt(token);

        // 只有非0值才更新
        if(tempX != 0) x_1 = tempX;
        if(tempY != 0) y_1 = tempY;
        yuntai_flag = 1;

        // /* 把结果回显 */
        // char txBuf[64];
        // snprintf(txBuf, sizeof(txBuf), "%d, %d\r\n", x, y);
        // uart0_send_string(txBuf);

        // token = strtok(NULL, ",");
        // int z = asciiToInt(token);
    }
}


/* --------------- UART0 中断 --------------------------*/
void UART_0_INST_IRQHandler(void)
{
    switch (DL_UART_getPendingInterrupt(UART_0_INST))
    {
    case DL_UART_IIDX_RX:
    {
        uint8_t rx = DL_UART_Main_receiveData(UART_0_INST);
        // DL_GPIO_togglePins(BUZZER_PORT,BUZZER_PIN_28_PIN);
        // uart0_send_char(rx);            // 回显调试用，可删

        if (rx == '\n')                 // 以 '\n' 作为一帧结束
        {
            rxBuffer[rxIndex] = '\0';
            rxComplete = true; 
            rxIndex = 0;
        }
        else if (rxIndex < MAX_RX_LEN - 1)
        {
            rxBuffer[rxIndex++] = rx;
        }
        else
        {
            rxIndex = 0;                // 溢出保护
        }
        processUARTData();
        break;

    }
    default:
        break;
    }
}