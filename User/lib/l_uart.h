/*
 * u_uart.h
 *
 *  Created on: Sep 3, 2021
 *      Author: abel
 */

#ifndef INC_L_UART_H_
#define INC_L_UART_H_

#include "usart.h"
#include "porting.h"
#include <stdio.h>

//#define


//#define USE_CONSOLE_UART
//#define CONSOLE_UART_USE_DMA


#define UART_RX_TIMEOUT		100
#define UART_TX_TIMEOUT		100
#define UartPrompt		CmdPrompt



#define uart_transmit_dma(huart, data, len)		HAL_UART_Transmit_DMA((UART_HandleTypeDef *)(huart), data, len)

#define uart_transmit(huart, data, len)		HAL_UART_Transmit((UART_HandleTypeDef *)(huart), data, len, UART_TX_TIMEOUT)
extern int uart_write(UART_HandleTypeDef *huart, uint8_t *data, int len);
extern int uart_putchar(UART_HandleTypeDef *huart, int ch);

#define print_endCharsOfLine(huart)		uart_write((UART_HandleTypeDef *)(huart), EndCharsOfLine, sizeof(EndCharsOfLine) - 1);

#define print_prompt(huart)		uart_transmit(huart, UartPrompt, sizeof(UartPrompt) - 1);
#define lnprint_prompt(huart)		uart_transmit(huart, EndCharsOfLine UartPrompt, sizeof(EndCharsOfLine UartPrompt) - 1)
#define println_prompt(huart)		uart_transmit(huart, UartPrompt EndCharsOfLine, sizeof(UartPrompt EndCharsOfLine) - 1)


extern int __uart_printf(UART_HandleTypeDef *huart, char *fmt, ...);
extern int __uart_printv(UART_HandleTypeDef *huart, char *argv, int argc);

#define uart_printf(huart, fmt, args...)	__uart_printf((UART_HandleTypeDef *)(huart), fmt, ##args)
#define uart_printv(huart, argv, argc)			__uart_printv((UART_HandleTypeDef *)(huart), argv, argc)
#define uart_print(huart, str)			uart_write((UART_HandleTypeDef *)(huart), (uint8_t *)str, strlen(str))
#define uart_println(huart, str)	do{uart_print(huart, str);print_endCharsOfLine(huart);}while(0)
#define uart_printfln(huart, fmt, args...)		do{uart_printf(huart, fmt, ##args);print_endCharsOfLine(huart);}while(0)
#define uart_printvln(huart, argv, argc)		do{uart_printv(huart, argv, argc);print_endCharsOfLine(huart);}while(0)

#define uart_prints(huart, str)			uart_print(huart, str)

#define uart_printx(huart, x, args...)	uart_print##x(huart, ##args)

#define uartx_printx(idx, x, ...)	uart_print##x(&huart##idx, ##__VA_ARGS__)




extern int uart_write_dma(UART_HandleTypeDef *huart, uint8_t *data, int len);
extern int __uart_dma_printf(UART_HandleTypeDef *huart, char *fmt, ...);

#define uart_dma_printf(huart, fmt, args...)	__uart_dma_printf((UART_HandleTypeDef *)(huart), fmt, ##args)
#define uart_dma_printfln(huart, fmt, args...)		__uart_dma_printf(huart, fmt EndCharsOfLine, ##args)
#define uart_dma_printx(huart, x, args...)	uart_dma_print##x(huart, ##args)


#define __TO_HUART(x)		h ## x
#define TO_HUART(x)		__TO_HUART(x)


#ifdef USE_CONSOLE_UART


#define console_uart	uart4

#define console_huart	TO_HUART(console_uart)

#ifdef CONSOLE_UART_USE_DMA
#define console_printx(x, ...)	uart_dma_printx(&console_huart, x, ##__VA_ARGS__)
#else
#define console_printx(x, ...)	uart_printx(&console_huart, x, ##__VA_ARGS__)
#endif



#define USE_STDIO


#ifdef USE_STDIO
#define printfln(fmt, args...)		printf(fmt EndCharsOfLine, ##args)
#else
#define printf(fmt, args...)		console_printx(f, fmt, ##args)
#define printfln(fmt, args...)		console_printx(fln, fmt, ##args)
#endif
#else
#define printf(fmt, args...)
#define printfln(fmt, args...)
#endif

void Init_UART_Receive_IT(void);

int wait_input(UART_HandleTypeDef *huart, uint8_t *buff, int bufflen);


#endif /* INC_L_UART_H_ */
