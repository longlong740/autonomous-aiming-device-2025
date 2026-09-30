#include "ti_msp_dl_config.h"
#include "stdio.h"
#include "string.h"


// int fputc(int c, FILE* stream)
// {
// 	DL_UART_Main_transmitDataBlocking(UART_1_INST, c);
//     return c;
// }

// int fputs(const char* restrict s, FILE* restrict stream)
// {
//     uint16_t i, len;
//     len = strlen(s);
//     for(i=0; i<len; i++)
//     {
//         DL_UART_Main_transmitDataBlocking(UART_1_INST, s[i]);
//     }
//     return len;
// }

// int puts(const char *_ptr)
// {
//     int count = fputs(_ptr, stdout);
//     count += fputs("\n", stdout);
//     return count;
// }