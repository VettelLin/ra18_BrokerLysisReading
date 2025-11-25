/*
 * tcpip.c
 *
 *  Created on: Dec 29, 2021
 *      Author: abel
 */


#include <stdarg.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lib/l_uart.h"
#include "utils/l_string.h"
#include "can_msg.h"




#define msg_uart	uart5
#define msg_huart	TO_HUART(msg_uart)


#define MSG_BUFF_QTY		4
#define MSG_BUFF_SIZE		256

#define RXBUFF_FLAG_FREE	0
#define RXBUFF_FLAG_RECEVING	1
#define RXBUFF_FLAG_PENDING		2
#define RXBUFF_FLAG_ACTION		3

typedef struct {
	uint8_t flag;
	uint8_t linkid;
	uint16_t msglen;
	uint8_t *pdata;
	uint8_t buff[MSG_BUFF_SIZE];
}msg_rxbuff_st;
msg_rxbuff_st msg_rxbuff[MSG_BUFF_QTY];



#include "lib/l_delay.h"


int forward_to_iamp(uint8_t *data, int len)
{
//	uart_transmit(&msg_huart, data, len);
	uart_transmit_dma(&huart4, data, len);

	return 0;
}


/* Set to 1 to silence all binary forwarding on UART5, keeping console clean */
#ifndef QUIET_UART5_BUS
#define QUIET_UART5_BUS 0
#endif

int write_bus_msg(uint8_t *data, int len)
{
	if(!QUIET_UART5_BUS){
		uart_transmit(&msg_huart, data, len);
	}
//	uart_transmit_dma(&msg_huart, data, len);

	return 0;
}


#include "heating_dock.h"


uint8_t calc_rcc(uint8_t *pdata, int len)
{
	int i;

	uint8_t sum = 0;

	for(i=0; i<len; i++){
		sum += pdata[i];
	}

	return sum;
}

int check_rcc(uint8_t *pdata, int len)
{
	int i;

	uint8_t sum = 0;

	for(i=0; i<len; i++){
		sum += pdata[i];
	}

	return (sum ==  pdata[len]);
}


int busmsg_send_ps96data(uint8_t *txbuff, uint8_t cmd, uint8_t *data, uint8_t data_len)
{

	uint8_t  *pbuff;
	uint8_t rcc = 0;

	txbuff[0] = 0x5E;
	txbuff[1] = cmd;

	txbuff[2] = 0x00;



	pbuff = &txbuff[4];


	if(data_len > 0){
		if(data){
			memcpy(pbuff, data, data_len);
		}
	}


	data_len += 4;

	txbuff[3] = data_len + 1;

	rcc = calc_rcc(txbuff, data_len);

	txbuff[data_len++] = rcc;


	return write_bus_msg(txbuff, data_len);
}



void uart_MsgHandler(uint16_t linkid, uint16_t msg_len, uint8_t *msg_data)
{
	int i;
	uint8_t group_id;
	uint8_t func_code;
	uint8_t *rxdata = msg_data;
	int rxlen = msg_len;

	if(linkid == 2){
		ps96_process_heatdock_msg(rxdata[0], rxdata + 3, rxlen - 3);
	}


}

int check_iamp_msg(uint8_t *msgdata, int msglen)
{
	int i;
	uint16_t linkid, datalen;



//	print_bytes(msgdata, msglen);


	if(msglen < 5){
		goto MsgErr;
	}

	if(msgdata[0] == 0x5E){

		datalen = msgdata[3];

		if(datalen > msglen){
			goto MsgErr;
		}

		if(!check_rcc(msgdata, datalen - 1)){
			goto MsgErr;
		}

		return datalen;
	}


	MsgErr:
//	msg_send_nack();
	return -1;

}

#define BUFFER_TO_FORWARD

int parse_busmsg_head(msg_rxbuff_st *p_msgbuff)
{
	int i;
	uint16_t linkid, datalen;
	uint8_t *msgdata = p_msgbuff->buff;
	int msglen = p_msgbuff->msglen;


//	print_bytes(msgdata, msglen);
#ifdef BUFFER_TO_FORWARD

	if((msgdata[1] & 0xf0) == 0x60){

		p_msgbuff->linkid = 2;
		p_msgbuff->pdata = msgdata + 1;
		p_msgbuff->msglen = msglen - 2;
		return 0;
	}else{
		forward_to_iamp(msgdata, msglen);
//		msglen -= 3;
//		if(msglen > 8){
//			msglen = 8;
//		}
//		can_sendmsg(msgdata[1], msgdata + 3, msglen);
		return -1;
	}
#else

#if 1
	p_msgbuff->linkid = 2;
	p_msgbuff->pdata = msgdata + 1;
	p_msgbuff->msglen = msglen - 2;
	return 0;
#else
	if(msglen < 5){
		goto MsgErr;
	}

	if(msgdata[0] == 0x5E){

		datalen = msgdata[3];

		if(datalen > msglen){
			goto MsgErr;
		}

		if(!check_rcc(msgdata, msglen - 1)){
			goto MsgErr;
		}

		p_msgbuff->linkid = 2;
		p_msgbuff->pdata = msgdata + 1;
		p_msgbuff->msglen = datalen - 2;
		return 0;
	}


	MsgErr:
//	msg_send_nack();
	return -1;
#endif
#endif
}




void init_bus_msg(void)
{
	int i;

	for(i=0; i<MSG_BUFF_QTY; i++){
		msg_rxbuff[i].flag = RXBUFF_FLAG_FREE;
		msg_rxbuff[i].linkid = 255;
		msg_rxbuff[i].msglen = 0;
//		msg_rxbuff[i].buff
	}

}

void process_bus_msg(void)
{
	int i;

	for(i=0; i<MSG_BUFF_QTY; i++){
		if(msg_rxbuff[i].flag == RXBUFF_FLAG_PENDING){
			msg_rxbuff[i].flag = RXBUFF_FLAG_ACTION;
			printfln("process busmsg[%d] %u bytes...", i, msg_rxbuff[i].msglen);

			if(parse_busmsg_head(&msg_rxbuff[i]) == 0){
				uart_MsgHandler(msg_rxbuff[i].linkid, msg_rxbuff[i].msglen, msg_rxbuff[i].pdata);
			}
			msg_rxbuff[i].flag = RXBUFF_FLAG_FREE;
			msg_rxbuff[i].linkid = 255;
			msg_rxbuff[i].msglen = 0;
		}
	}

}

#define MSG_UART_RXCALLBACK(uart)	uart##_user_RxCallback
#define msg_uart_RxCallback(uart)			MSG_UART_RXCALLBACK(uart)

void msg_uart_RxCallback(msg_uart) (UART_HandleTypeDef *huart, uint8_t *rx_data, uint16_t rx_len)
{
	int i, msglen;
	msg_rxbuff_st *p_msgbuff = NULL;

//	uart_transmit(huart, rx_data, rx_len);

	printfln("received msg %d bytes", rx_len);


	do{
		msglen = check_iamp_msg(rx_data, rx_len);

		if(msglen < 0){
			return;
		}

#ifdef BUFFER_TO_FORWARD
		for(i=0; i<MSG_BUFF_QTY; i++){
			if(msg_rxbuff[i].flag == RXBUFF_FLAG_FREE){
				p_msgbuff = &msg_rxbuff[i];
				p_msgbuff->flag = RXBUFF_FLAG_RECEVING;
				p_msgbuff->msglen = msglen;
				memcpy(p_msgbuff->buff, rx_data, msglen);
				p_msgbuff->flag = RXBUFF_FLAG_PENDING;
				break;
			}
		}
#else
		if((rx_data[1] & 0xf0) == 0x60){

			for(i=0; i<MSG_BUFF_QTY; i++){
				if(msg_rxbuff[i].flag == RXBUFF_FLAG_FREE){
					p_msgbuff = &msg_rxbuff[i];
					p_msgbuff->flag = RXBUFF_FLAG_RECEVING;
					p_msgbuff->msglen = msglen;
					memcpy(p_msgbuff->buff, rx_data, msglen);
					p_msgbuff->flag = RXBUFF_FLAG_PENDING;
					break;
				}
			}
		}else{
			forward_to_iamp(rx_data, rx_len);
		}

#endif

		rx_data += msglen;
		rx_len -= msglen;

	}while(rx_len > 4);
}


#ifndef USE_CONSOLE_UART
void uart4_user_RxCallback(UART_HandleTypeDef *huart, uint8_t *rx_data, uint16_t rx_len)
{
	if(!QUIET_UART5_BUS){
		write_bus_msg(rx_data, rx_len);
	}
//	rx_len -= 3;
//	if(rx_len > 8){
//		rx_len = 8;
//	}
//	can_sendmsg(rx_data[1], rx_data + 3, rx_len);
}
#endif

