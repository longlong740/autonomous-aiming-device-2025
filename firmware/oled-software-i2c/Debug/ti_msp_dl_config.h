/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X
#define CONFIG_MSPM0G3507

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)



#define CPUCLK_FREQ                                                     80000000



/* Defines for PWM_Car */
#define PWM_Car_INST                                                       TIMA1
#define PWM_Car_INST_IRQHandler                                 TIMA1_IRQHandler
#define PWM_Car_INST_INT_IRQN                                   (TIMA1_INT_IRQn)
#define PWM_Car_INST_CLK_FREQ                                           80000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_Car_C0_PORT                                               GPIOB
#define GPIO_PWM_Car_C0_PIN                                        DL_GPIO_PIN_4
#define GPIO_PWM_Car_C0_IOMUX                                    (IOMUX_PINCM17)
#define GPIO_PWM_Car_C0_IOMUX_FUNC                   IOMUX_PINCM17_PF_TIMA1_CCP0
#define GPIO_PWM_Car_C0_IDX                                  DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_PWM_Car_C1_PORT                                               GPIOB
#define GPIO_PWM_Car_C1_PIN                                        DL_GPIO_PIN_5
#define GPIO_PWM_Car_C1_IOMUX                                    (IOMUX_PINCM18)
#define GPIO_PWM_Car_C1_IOMUX_FUNC                   IOMUX_PINCM18_PF_TIMA1_CCP1
#define GPIO_PWM_Car_C1_IDX                                  DL_TIMER_CC_1_INDEX



/* Defines for TIMER_Encoder_Read */
#define TIMER_Encoder_Read_INST                                          (TIMG6)
#define TIMER_Encoder_Read_INST_IRQHandler                        TIMG6_IRQHandler
#define TIMER_Encoder_Read_INST_INT_IRQN                        (TIMG6_INT_IRQn)
#define TIMER_Encoder_Read_INST_LOAD_VALUE                                 (3124U)
/* Defines for TIMER_0 */
#define TIMER_0_INST                                                     (TIMA0)
#define TIMER_0_INST_IRQHandler                                 TIMA0_IRQHandler
#define TIMER_0_INST_INT_IRQN                                   (TIMA0_INT_IRQn)
#define TIMER_0_INST_LOAD_VALUE                                           (399U)



/* Defines for UART_0 */
#define UART_0_INST                                                        UART0
#define UART_0_INST_FREQUENCY                                           40000000
#define UART_0_INST_IRQHandler                                  UART0_IRQHandler
#define UART_0_INST_INT_IRQN                                      UART0_INT_IRQn
#define GPIO_UART_0_RX_PORT                                                GPIOA
#define GPIO_UART_0_TX_PORT                                                GPIOA
#define GPIO_UART_0_RX_PIN                                        DL_GPIO_PIN_11
#define GPIO_UART_0_TX_PIN                                        DL_GPIO_PIN_10
#define GPIO_UART_0_IOMUX_RX                                     (IOMUX_PINCM22)
#define GPIO_UART_0_IOMUX_TX                                     (IOMUX_PINCM21)
#define GPIO_UART_0_IOMUX_RX_FUNC                      IOMUX_PINCM22_PF_UART0_RX
#define GPIO_UART_0_IOMUX_TX_FUNC                      IOMUX_PINCM21_PF_UART0_TX
#define UART_0_BAUD_RATE                                                (115200)
#define UART_0_IBRD_40_MHZ_115200_BAUD                                      (21)
#define UART_0_FBRD_40_MHZ_115200_BAUD                                      (45)
/* Defines for UART_3 */
#define UART_3_INST                                                        UART3
#define UART_3_INST_FREQUENCY                                           80000000
#define UART_3_INST_IRQHandler                                  UART3_IRQHandler
#define UART_3_INST_INT_IRQN                                      UART3_INT_IRQn
#define GPIO_UART_3_RX_PORT                                                GPIOA
#define GPIO_UART_3_TX_PORT                                                GPIOA
#define GPIO_UART_3_RX_PIN                                        DL_GPIO_PIN_25
#define GPIO_UART_3_TX_PIN                                        DL_GPIO_PIN_26
#define GPIO_UART_3_IOMUX_RX                                     (IOMUX_PINCM55)
#define GPIO_UART_3_IOMUX_TX                                     (IOMUX_PINCM59)
#define GPIO_UART_3_IOMUX_RX_FUNC                      IOMUX_PINCM55_PF_UART3_RX
#define GPIO_UART_3_IOMUX_TX_FUNC                      IOMUX_PINCM59_PF_UART3_TX
#define UART_3_BAUD_RATE                                                (115200)
#define UART_3_IBRD_80_MHZ_115200_BAUD                                      (43)
#define UART_3_FBRD_80_MHZ_115200_BAUD                                      (26)





/* Port definition for Pin Group GPIO_LED */
#define GPIO_LED_PORT                                                    (GPIOB)

/* Defines for PIN_led: GPIOB.17 with pinCMx 43 on package pin 14 */
#define GPIO_LED_PIN_led_PIN                                    (DL_GPIO_PIN_17)
#define GPIO_LED_PIN_led_IOMUX                                   (IOMUX_PINCM43)
/* Port definition for Pin Group MODE */
#define MODE_PORT                                                        (GPIOB)

/* Defines for ctrl: GPIOB.23 with pinCMx 51 on package pin 22 */
#define MODE_ctrl_PIN                                           (DL_GPIO_PIN_23)
#define MODE_ctrl_IOMUX                                          (IOMUX_PINCM51)
/* Port definition for Pin Group BEGIN */
#define BEGIN_PORT                                                       (GPIOB)

/* Defines for GO: GPIOB.22 with pinCMx 50 on package pin 21 */
#define BEGIN_GO_PIN                                            (DL_GPIO_PIN_22)
#define BEGIN_GO_IOMUX                                           (IOMUX_PINCM50)
/* Port definition for Pin Group Change */
#define Change_PORT                                                      (GPIOB)

/* Defines for all: GPIOB.19 with pinCMx 45 on package pin 16 */
#define Change_all_PIN                                          (DL_GPIO_PIN_19)
#define Change_all_IOMUX                                         (IOMUX_PINCM45)
/* Port definition for Pin Group BUZZER */
#define BUZZER_PORT                                                      (GPIOA)

/* Defines for PIN_28: GPIOA.28 with pinCMx 3 on package pin 35 */
#define BUZZER_PIN_28_PIN                                       (DL_GPIO_PIN_28)
#define BUZZER_PIN_28_IOMUX                                       (IOMUX_PINCM3)
/* Port definition for Pin Group GPIO_jiguang */
#define GPIO_jiguang_PORT                                                (GPIOA)

/* Defines for PIN_0: GPIOA.24 with pinCMx 54 on package pin 25 */
#define GPIO_jiguang_PIN_0_PIN                                  (DL_GPIO_PIN_24)
#define GPIO_jiguang_PIN_0_IOMUX                                 (IOMUX_PINCM54)
/* Port definition for Pin Group GPIO_OLED */
#define GPIO_OLED_PORT                                                   (GPIOA)

/* Defines for PIN_SCL: GPIOA.2 with pinCMx 7 on package pin 42 */
#define GPIO_OLED_PIN_SCL_PIN                                    (DL_GPIO_PIN_2)
#define GPIO_OLED_PIN_SCL_IOMUX                                   (IOMUX_PINCM7)
/* Defines for PIN_SDA: GPIOA.1 with pinCMx 2 on package pin 34 */
#define GPIO_OLED_PIN_SDA_PIN                                    (DL_GPIO_PIN_1)
#define GPIO_OLED_PIN_SDA_IOMUX                                   (IOMUX_PINCM2)
/* Port definition for Pin Group GPIO_IN */
#define GPIO_IN_PORT                                                     (GPIOB)

/* Defines for PIN_AIN1: GPIOB.10 with pinCMx 27 on package pin 62 */
#define GPIO_IN_PIN_AIN1_PIN                                    (DL_GPIO_PIN_10)
#define GPIO_IN_PIN_AIN1_IOMUX                                   (IOMUX_PINCM27)
/* Defines for PIN_BIN1: GPIOB.11 with pinCMx 28 on package pin 63 */
#define GPIO_IN_PIN_BIN1_PIN                                    (DL_GPIO_PIN_11)
#define GPIO_IN_PIN_BIN1_IOMUX                                   (IOMUX_PINCM28)
/* Port definition for Pin Group Line */
#define Line_PORT                                                        (GPIOA)

/* Defines for _1: GPIOA.12 with pinCMx 34 on package pin 5 */
#define Line__1_PIN                                             (DL_GPIO_PIN_12)
#define Line__1_IOMUX                                            (IOMUX_PINCM34)
/* Defines for _2: GPIOA.9 with pinCMx 20 on package pin 55 */
#define Line__2_PIN                                              (DL_GPIO_PIN_9)
#define Line__2_IOMUX                                            (IOMUX_PINCM20)
/* Defines for _3: GPIOA.7 with pinCMx 14 on package pin 49 */
#define Line__3_PIN                                              (DL_GPIO_PIN_7)
#define Line__3_IOMUX                                            (IOMUX_PINCM14)
/* Defines for _4: GPIOA.15 with pinCMx 37 on package pin 8 */
#define Line__4_PIN                                             (DL_GPIO_PIN_15)
#define Line__4_IOMUX                                            (IOMUX_PINCM37)
/* Defines for _5: GPIOA.8 with pinCMx 19 on package pin 54 */
#define Line__5_PIN                                              (DL_GPIO_PIN_8)
#define Line__5_IOMUX                                            (IOMUX_PINCM19)
/* Defines for _6: GPIOA.13 with pinCMx 35 on package pin 6 */
#define Line__6_PIN                                             (DL_GPIO_PIN_13)
#define Line__6_IOMUX                                            (IOMUX_PINCM35)
/* Defines for _7: GPIOA.17 with pinCMx 39 on package pin 10 */
#define Line__7_PIN                                             (DL_GPIO_PIN_17)
#define Line__7_IOMUX                                            (IOMUX_PINCM39)
/* Defines for _8: GPIOA.16 with pinCMx 38 on package pin 9 */
#define Line__8_PIN                                             (DL_GPIO_PIN_16)
#define Line__8_IOMUX                                            (IOMUX_PINCM38)
/* Port definition for Pin Group LEFT_ENCODER */
#define LEFT_ENCODER_PORT                                                (GPIOA)

/* Defines for E1A: GPIOA.30 with pinCMx 5 on package pin 37 */
// pins affected by this interrupt request:["E1A","E1B"]
#define LEFT_ENCODER_INT_IRQN                                   (GPIOA_INT_IRQn)
#define LEFT_ENCODER_INT_IIDX                   (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define LEFT_ENCODER_E1A_IIDX                               (DL_GPIO_IIDX_DIO30)
#define LEFT_ENCODER_E1A_PIN                                    (DL_GPIO_PIN_30)
#define LEFT_ENCODER_E1A_IOMUX                                    (IOMUX_PINCM5)
/* Defines for E1B: GPIOA.31 with pinCMx 6 on package pin 39 */
#define LEFT_ENCODER_E1B_IIDX                               (DL_GPIO_IIDX_DIO31)
#define LEFT_ENCODER_E1B_PIN                                    (DL_GPIO_PIN_31)
#define LEFT_ENCODER_E1B_IOMUX                                    (IOMUX_PINCM6)
/* Port definition for Pin Group PID */
#define PID_PORT                                                         (GPIOB)

/* Defines for KP: GPIOB.12 with pinCMx 29 on package pin 64 */
#define PID_KP_PIN                                              (DL_GPIO_PIN_12)
#define PID_KP_IOMUX                                             (IOMUX_PINCM29)
/* Defines for KI: GPIOB.2 with pinCMx 15 on package pin 50 */
#define PID_KI_PIN                                               (DL_GPIO_PIN_2)
#define PID_KI_IOMUX                                             (IOMUX_PINCM15)
/* Defines for KD: GPIOB.3 with pinCMx 16 on package pin 51 */
#define PID_KD_PIN                                               (DL_GPIO_PIN_3)
#define PID_KD_IOMUX                                             (IOMUX_PINCM16)
/* Port definition for Pin Group RIGHT_ENCODER */
#define RIGHT_ENCODER_PORT                                               (GPIOB)

/* Defines for E2A: GPIOB.1 with pinCMx 13 on package pin 48 */
// pins affected by this interrupt request:["E2A","E2B"]
#define RIGHT_ENCODER_INT_IRQN                                  (GPIOB_INT_IRQn)
#define RIGHT_ENCODER_INT_IIDX                  (DL_INTERRUPT_GROUP1_IIDX_GPIOB)
#define RIGHT_ENCODER_E2A_IIDX                               (DL_GPIO_IIDX_DIO1)
#define RIGHT_ENCODER_E2A_PIN                                    (DL_GPIO_PIN_1)
#define RIGHT_ENCODER_E2A_IOMUX                                  (IOMUX_PINCM13)
/* Defines for E2B: GPIOB.0 with pinCMx 12 on package pin 47 */
#define RIGHT_ENCODER_E2B_IIDX                               (DL_GPIO_IIDX_DIO0)
#define RIGHT_ENCODER_E2B_PIN                                    (DL_GPIO_PIN_0)
#define RIGHT_ENCODER_E2B_IOMUX                                  (IOMUX_PINCM12)

/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_PWM_Car_init(void);
void SYSCFG_DL_TIMER_Encoder_Read_init(void);
void SYSCFG_DL_TIMER_0_init(void);
void SYSCFG_DL_UART_0_init(void);
void SYSCFG_DL_UART_3_init(void);


bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
