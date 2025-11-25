/*
 * cmd.c
 *
 *  Created on: Sep 4, 2021
 *      Author: abel
 */



#include <stdlib.h>
#include <string.h>

#include "app/u_cmd.h"
#include "l_gpio.h"
#include "l_timer.h"
#include "l_uart.h"

#define CMDLINE_MAX_LEN	128
int cmdline_size = CMDLINE_MAX_LEN;
volatile int cmd_keyed = 0;
char cmdline[CMDLINE_MAX_LEN];
volatile int cmdlen = 0;


int com_gpio(int argc, char *argv[])
{
	int status = 0;
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;

	if(argc < 4){
		return -1;
	}

	switch(*argv[2]){
		case 'a':
		case 'A':
			GPIOx = GPIOA;
			break;
		case 'b':
		case 'B':
			GPIOx = GPIOB;
			break;
		case 'c':
		case 'C':
			GPIOx = GPIOC;
			break;
		case 'd':
		case 'D':
			GPIOx = GPIOD;
			break;
#ifdef GPIOE
		case 'e':
		case 'E':
			GPIOx = GPIOE;
			break;
#endif
#ifdef GPIOF
		case 'f':
		case 'F':
			GPIOx = GPIOF;
			break;
#endif
#ifdef GPIOG
		case 'g':
		case 'G':
			GPIOx = GPIOG;
			break;
#endif
#ifdef GPIOH
		case 'h':
		case 'H':
			GPIOx = GPIOH;
			break;
#endif
#ifdef GPIOI
		case 'i':
		case 'I':
			GPIOx = GPIOI;
			break;
#endif

		default :
			return -1;
	}

	GPIO_Pin = 1 << strtoul(argv[3], NULL, 0);

	if(strcmp(argv[1], "get") == 0){
		status = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
		printfln("input gpio_%s%s : %d", argv[2], argv[3], status);
		printfln("input gpio_%c : %#x ", *argv[2], GPIOx->IDR);
	}
	else if(strcmp(argv[1], "set") == 0){
		if(argc > 4){
			status = atoi(argv[4]) ? GPIO_PIN_SET : GPIO_PIN_RESET;
		}
		printfln("set gpio_%s%s : %d", argv[2], argv[3], status);
		HAL_GPIO_WritePin(GPIOx, GPIO_Pin, status);
	}

	return 0;
}




int com_sys(int argc, char *argv[])
{
	int err = 0;

	if(strcmp(argv[1], "freq") == 0){
		printfln("sysclock frequence : %u \n", HAL_RCC_GetSysClockFreq());
	}



	return err;
}

#include "version.h"
int com_version(int argc, char *argv[])
{
	int err = 0;

	printfln("firmware version : %s", VERSION_STR);

	return err;
}

extern int com_app(int argc, char *argv[]);
int command_handler(int argc, char *argv[])
{
	int err = 0;
	char *cmd = argv[0];
//	argv[0] = argv[argc];

	if(strcmp(cmd, "gpio") == 0){
		err = com_gpio(argc, argv);
	}
	else if(strcmp(cmd, "version") == 0){
		err = com_version(argc, argv);
	}

	else if(strcmp(cmd, "sys") == 0){
		err = com_sys(argc, argv);
	}
	else{
		err = com_app(argc, argv);
	}


	return err;
}

int call_command(char *line)
{
	char *word;
	int argc = 0;
	static char *argv[12];

	if((line == NULL) || (*line == 0)){
		return -1;
	}

	printfln("exec cmd %s", line);

	word= strtok(line, " \t");

	if(word){
		do{
			argv[argc++] = word;
			word= strtok(NULL, " \t");
		}while(word);
//			argv[argc] = (char *)huart;
		return command_handler(argc, argv);
	}

	return -1;
}

void command_line_process(void)
{
	if(cmd_keyed){
		cmd_keyed = 0;
		call_command(cmdline);
	}
}

void command_uart_RxCallback(UART_HandleTypeDef *huart, uint8_t *rx_data, uint16_t rx_len)
{
	int i;

	for(i=0; i<rx_len; i++){
		if((rx_data[i] == '\r') || (rx_data[i] == '\n')){
			lnprint_prompt(huart);
			cmdline[cmdlen] = 0;
			cmdlen = 0;
			cmd_keyed = 1;
			break;
		}else{
			if(cmdlen >= sizeof(cmdline)){
				cmdline[cmdlen] = 0;
				cmdlen = 0;
				break;
			}
			uart_transmit(huart, &rx_data[i], 1);
			cmdline[cmdlen++] = rx_data[i];
		}
	}

	if(cmd_keyed){
		cmd_keyed = 0;
		call_command(cmdline);
	}

}

#define CONSOLE_UART_RXCALLBACK(uart)	uart##_user_RxCallback

#define console_uart_RxCallback(uart)			CONSOLE_UART_RXCALLBACK(uart)

#ifdef USE_CONSOLE_UART

void console_uart_RxCallback(console_uart) (UART_HandleTypeDef *huart, uint8_t *rx_data, uint16_t rx_len)
{
	command_uart_RxCallback(huart, rx_data, rx_len);
}
#endif
