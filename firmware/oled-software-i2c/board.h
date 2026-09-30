#ifndef _BOARD_H_
#define _BOARD_H_
#include "stdio.h"
#include "string.h"
#include "ti_msp_dl_config.h"


// #define ABS(a)      (a>0 ? a:(-a))
// typedef int32_t  s32;
// typedef int16_t s16;
// typedef int8_t  s8;
// typedef uint8_t u8;  // 定义 u8 为无符号8位整型

// extern int find,x_1,y_1;


// typedef const int32_t sc32;  /*!< Read Only */
// typedef const int16_t sc16;  /*!< Read Only */
// typedef const int8_t sc8;   /*!< Read Only */

// typedef __IO int32_t  vs32;
// typedef __IO int16_t  vs16;
// typedef __IO int8_t   vs8;

// typedef __I int32_t vsc32;  /*!< Read Only */
// typedef __I int16_t vsc16;  /*!< Read Only */
// typedef __I int8_t vsc8;   /*!< Read Only */

// typedef uint32_t  u32;
// typedef uint16_t u16;
// typedef uint8_t  u8;

// typedef const uint32_t uc32;  /*!< Read Only */
// typedef const uint16_t uc16;  /*!< Read Only */
// typedef const uint8_t uc8;   /*!< Read Only */

// typedef __IO uint32_t  vu32;
// typedef __IO uint16_t vu16;
// typedef __IO uint8_t  vu8;

// typedef __I uint32_t vuc32;  /*!< Read Only */
// typedef __I uint16_t vuc16;  /*!< Read Only */
// typedef __I uint8_t vuc8;   /*!< Read Only */


// Enumeration of car types
//С���ͺŵ�ö�ٶ���
typedef enum 
{
	Mec_Car = 0, 
	Omni_Car, 
	Akm_Car, 
	Diff_Car, 
	FourWheel_Car, 
	Tank_Car
} CarMode;

extern int Flag_Stop;


#define MAX_RX_LEN  64  // 最大接收长度

extern volatile uint8_t rxBuffer[MAX_RX_LEN];  // 接收缓冲区
extern volatile uint8_t rxIndex;          // 接收索引
extern volatile bool rxComplete;      // 接收完成标志



// void processUARTData(void);

#endif  /* #ifndef _BOARD_H_ */