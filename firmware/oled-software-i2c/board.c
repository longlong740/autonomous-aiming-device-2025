// #include "board.h"

// volatile unsigned long tick_ms;
// volatile uint32_t start_time;


// volatile uint8_t rxBuffer[MAX_RX_LEN];  // 接收缓冲区
// volatile uint8_t rxIndex = 0;          // 接收索引
// volatile bool rxComplete = false;      // 接收完成标志



// /**
//  * 重定义fputc函数，实现printf通过UART输出
//  * @param ch 要发送的字符
//  * @param stream 文件流（未使用）
//  * @return 发送的字符
//  */
// int fputc(int ch, FILE *stream)
// {
//     // 等待UART发送完成（不忙）
//     while( DL_UART_isBusy(UART_0_INST) == true );
//     // 通过UART发送字符
//     DL_UART_Main_transmitData(UART_0_INST, ch);
//     return ch;
// }

// /**
//  * 重定义fputs函数，实现字符串通过UART输出
//  * @param s 要发送的字符串
//  * @param stream 文件流（未使用）
//  * @return 发送的字符长度
//  */
// int fputs(const char* restrict s, FILE* restrict stream) {

//     uint16_t char_len=0;
//     // 循环发送字符串中的每个字符，直到遇到结束符'\0'
//     while(*s!=0)
//     {
//         // 等待UART发送完成
//         while( DL_UART_isBusy(UART_0_INST) == true );
//         // 发送当前字符并移动指针
//         DL_UART_Main_transmitData(UART_0_INST, *s++);
//         char_len++;
//     }
//     return char_len;
// }

// /**
//  * 重定义puts函数（简化实现）
//  * @param _ptr 要输出的字符串
//  * @return 0（固定返回值）
//  */
// int puts(const char* _ptr)
// {
//     return 0;
// }


