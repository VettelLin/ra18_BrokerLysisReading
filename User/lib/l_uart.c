/*
 * u_uart.c
 *
 *  Created on: Sep 3, 2021
 *      Author: abel
 */



#include "l_uart.h"

#include <stdarg.h>

#include <stdlib.h>



#define UART5_RX_CHARS_SIZE		512
#define UART4_RX_CHARS_SIZE		512	// 256 * 2


uint8_t uart5RxChars[UART5_RX_CHARS_SIZE];
uint8_t uart4RxChars[UART4_RX_CHARS_SIZE];

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#ifdef USE_STDIO

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(FILE *f)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */
PUTCHAR_PROTOTYPE
{
	uart_transmit(&console_huart, (uint8_t *)&ch, 1);
	return ch;
}
GETCHAR_PROTOTYPE
{
	uint8_t ch = 0;
	HAL_UART_Receive(&console_huart,(uint8_t *)&ch, 1, UART_TX_TIMEOUT);
	return (ch);
}

#endif


int uart_putchar(UART_HandleTypeDef *huart, int ch)
{
	uint8_t data = ch;
	return uart_transmit((huart), &data, 1);
}

int uart_write(UART_HandleTypeDef *huart, uint8_t *data, int len)
{
	int ret = 0;
	if(huart == NULL){
		return 0;
	}

	ret = uart_transmit((huart), data, len);
#ifdef USE_CONSOLE_UART
	if(((huart) == &console_huart) && ((len) >= sizeof(EndCharsOfLine))
			&& (strcmp((data) + len - sizeof(EndCharsOfLine), EndCharsOfLine) == 0)){
		print_prompt(huart);
	}
#endif
	return ret;
}

int __uart_printf(UART_HandleTypeDef *huart, char *fmt, ...)
{
	int len;
	va_list args;
	uint8_t strbuff[256];

	va_start(args, fmt);
	len = vsprintf(strbuff, fmt, args);
	va_end(args);

	return uart_write(huart, strbuff, len);
}

int __uart_printv(UART_HandleTypeDef *huart, char *argv, int argc)
{
	uint8_t strbuff[256];
	uint8_t *pbuff = strbuff;
	int i, len;

	if(argv == NULL || argc < 1){
		return 0;
	}

	len = strlen(argv[0]);
	strncpy(pbuff, argv[0], len);
	pbuff += len;
	for(i=1; i<argc; i++){
		*pbuff++ = 0x20;
		len = strlen(argv[i]);
		strncpy(pbuff, argv[i], len);
		pbuff += len;
	}
//	*pbuff++ = '\r';
//	*pbuff++ = '\n';

	return uart_write(huart, strbuff, pbuff - strbuff);
}

int uart_write_dma(UART_HandleTypeDef *huart, uint8_t *data, int len)
{
	int ret = 0;
	if(huart == NULL){
		return 0;
	}

	ret = uart_transmit_dma((huart), data, len);
#ifdef USE_CONSOLE_UART
	if(((huart) == &console_huart) && ((len) >= sizeof(EndCharsOfLine))
			&& (strcmp((data) + len - sizeof(EndCharsOfLine), EndCharsOfLine) == 0)){
		uart_transmit_dma(huart, UartPrompt, sizeof(UartPrompt));
	}
#endif

	return ret;
}

int __uart_dma_printf(UART_HandleTypeDef *huart, char *fmt, ...)
{
	int len;
	va_list args;
	uint8_t strbuff[256];

	va_start(args, fmt);
	len = vsprintf(strbuff, fmt, args);
	va_end(args);

	return uart_write_dma(huart, strbuff, len);
}

void Init_UART_Receive_IT(void)
{

	if(NVIC_GetEnableIRQ(UART5_IRQn)){
//		HAL_UARTEx_ReceiveToIdle_DMA(&huart5, uart5RxChars, UART5_RX_CHARS_SIZE);
		HAL_UARTEx_ReceiveToIdle_IT(&huart5, uart5RxChars, UART5_RX_CHARS_SIZE);
	}
	if(NVIC_GetEnableIRQ(UART4_IRQn)){
//		HAL_UARTEx_ReceiveToIdle_IT(&huart4, uart4RxChars, UART4_RX_CHARS_SIZE);
		HAL_UARTEx_ReceiveToIdle_DMA(&huart4, uart4RxChars, UART4_RX_CHARS_SIZE);
	}
//	if(NVIC_GetEnableIRQ(DMA1_Stream2_IRQn)){
//	}
//	HAL_UART_Receive_IT(&huart3, uart5RxChars, UART3_RX_CHARS_SIZE);

}



__weak void uart5_user_RxCallback(UART_HandleTypeDef *huart, uint8_t *rx_data, uint16_t rx_len)
{
	UNUSED(huart);
	UNUSED(rx_data);
	UNUSED(rx_len);
}

__weak void uart4_user_RxCallback(UART_HandleTypeDef *huart, uint8_t *rx_data, uint16_t rx_len)
{
	UNUSED(huart);
	UNUSED(rx_data);
	UNUSED(rx_len);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{

	if(huart->Instance == UART5){
		uart5_user_RxCallback(huart, uart5RxChars, Size);
//		HAL_UARTEx_ReceiveToIdle_DMA(huart, uart5RxChars, UART5_RX_CHARS_SIZE);
		while(HAL_UARTEx_ReceiveToIdle_IT(huart, uart5RxChars, UART5_RX_CHARS_SIZE) != HAL_OK);
	}
	else if(huart->Instance == UART4){
		uart4_user_RxCallback(huart, uart4RxChars, Size);
		while(HAL_UARTEx_ReceiveToIdle_DMA(huart, uart4RxChars, UART4_RX_CHARS_SIZE) != HAL_OK);
//		while(HAL_UARTEx_ReceiveToIdle_IT(huart, uart4RxChars, UART4_RX_CHARS_SIZE) != HAL_OK);
	}


}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart->ErrorCode & HAL_UART_ERROR_ORE){
		__HAL_UART_CLEAR_OREFLAG(huart);
	}

	if(huart->Instance == UART5){
//		HAL_UARTEx_ReceiveToIdle_DMA(huart, uart5RxChars, UART5_RX_CHARS_SIZE);
		while(HAL_UARTEx_ReceiveToIdle_IT(huart, uart5RxChars, UART5_RX_CHARS_SIZE) != HAL_OK);
	}
	else if(huart->Instance == UART4){
		HAL_UARTEx_ReceiveToIdle_DMA(huart, uart4RxChars, UART4_RX_CHARS_SIZE);
//		while(HAL_UARTEx_ReceiveToIdle_IT(huart, uart4RxChars, UART4_RX_CHARS_SIZE) != HAL_OK);
	}
}

int wait_input(UART_HandleTypeDef *huart, uint8_t *buff, int bufflen)
{
	int rxlen = 0;

	bufflen--;
	while(rxlen < bufflen){
		if(HAL_UART_Receive(huart, &buff[rxlen], 1, UART_RX_TIMEOUT) == HAL_OK){
			if((buff[rxlen] == '\r') || (buff[rxlen] == '\r')){
				lnprint_prompt(huart);
				break;
			}
			else if(buff[rxlen] == '\b'){
				rxlen--;
			}
			else{
				uart_transmit(huart, &buff[rxlen], 1);
				rxlen++;
			}
		}
	}

	buff[rxlen] = 0;

	return rxlen;
}
