#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include "ti_msp_dl_config.h"

#define UART_TX_BUF_SIZE 256 // UART发送缓冲区长度

/* ----------------- 用户可调宏 --------------------------*/
#define MAX_RX_LEN   64        // 一帧最大长度


/* --------------- 函数原型 -----------------------------*/
void uart0_send_char(char ch);
void uart0_send_string(const char *str);
int  asciiToInt(const char *str);
void processUARTData(void);
void uart0_send_data(const uint8_t *data, uint16_t len);

// uint8_t cmd[4];
extern volatile uint8_t UART3TxDMADone;
extern volatile uint8_t rx_data;
extern volatile int  x_1, y_1;  
extern volatile uint8_t yuntai_flag;

void UART_init(void);


void uart3_send_data(const uint8_t *data, uint16_t len);

void UART3_DMADoneTxCallback(void);

// #include "ti_msp_dl_config.h"
//UART0用的
void send_char(UART_Regs *uart, uint8_t data);
void send_str(UART_Regs *uart, uint8_t *data);

#endif
