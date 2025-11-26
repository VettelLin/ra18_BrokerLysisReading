/*
 * u_can.c
 *
 *  Created on: Jun 10, 2022
 *      Author: abel
 */



#include "u_can.h"
#include "lib/l_uart.h"

__weak void can_user_RxCallback(CAN_HandleTypeDef *hcan, CAN_RxHeaderTypeDef *pHead, uint8_t *rxbuff)
{
	UNUSED(hcan);
	UNUSED(pHead);
	UNUSED(rxbuff);
}



void can_rxMsg_handler(CAN_HandleTypeDef *hcan, CAN_RxHeaderTypeDef *pHead, uint8_t *rxbuff)
{
	int i;

#if defined(VERBOSE_CAN_RX_ISR)
	printf("can rxHeader : ide %#x, stdid %#x, extid %#x\r\n", pHead->IDE, pHead->StdId, pHead->ExtId);
	printf("can rxHeader : RTR %#x, dlc %#x\r\n", pHead->RTR, pHead->DLC);
	printf("can rxHeader : Timestamp %#x, FilterMatchIndex %#x\r\n", pHead->Timestamp, pHead->FilterMatchIndex);

	for(i=0; i<pHead->DLC; i++){
		printf("byte[%d] = %#.2x\r\n", i, rxbuff[i]);
	}
#endif

	can_user_RxCallback(hcan, pHead, rxbuff);
}

void can_error_handler(CAN_HandleTypeDef *hcan)
{
    uart_printfln(&huart5, "CAN ERR code=0x%lx state=0x%lx", (unsigned long)hcan->ErrorCode, (unsigned long)hcan->State);
	if(hcan->ErrorCode){
		if(hcan->ErrorCode & HAL_CAN_ERROR_ACK){
            uart_printfln(&huart5, "CAN ERR detail: No ACK on bus (check other node/normal mode/termination)");
		}
		if(hcan->ErrorCode & HAL_CAN_ERROR_RX_FOV0){
            uart_printfln(&huart5, "CAN ERR detail: RX FIFO0 overrun");
		}
		if(hcan->ErrorCode & HAL_CAN_ERROR_RX_FOV1){
            uart_printfln(&huart5, "CAN ERR detail: RX FIFO1 overrun");
		}
		if(hcan->ErrorCode & HAL_CAN_ERROR_BOF){
            uart_printfln(&huart5, "CAN ERR detail: Bus-Off");
		}
		if(hcan->ErrorCode & HAL_CAN_ERROR_EPV){
            uart_printfln(&huart5, "CAN ERR detail: Error-Passive");
		}
		if(hcan->ErrorCode & HAL_CAN_ERROR_EWG){
            uart_printfln(&huart5, "CAN ERR detail: Error-Warning");
		}
	}
}
