/*
 * l_can.c
 *
 *  Created on: Jun 10, 2022
 *      Author: abel
 */


#include "l_can.h"
#include "l_uart.h"

//#include "stm32f7xx_hal_can.h"


/* Lightweight RX log ring buffer to print from main context (avoid UART in ISR) */
typedef struct {
	uint8_t dlc;
	uint32_t id;
	uint8_t data[8];
} can_rxlog_item_t;
#define CAN_RXLOG_CAP			8u		/* must be power of two */
#define CAN_RXLOG_MASK			(CAN_RXLOG_CAP - 1u)
static volatile uint8_t g_can_rxlog_head = 0;
static volatile uint8_t g_can_rxlog_tail = 0;
static can_rxlog_item_t g_can_rxlog_buf[CAN_RXLOG_CAP];

void canlog_drain(void)
{
	while(g_can_rxlog_tail != g_can_rxlog_head){
		const can_rxlog_item_t *it = &g_can_rxlog_buf[g_can_rxlog_tail & CAN_RXLOG_MASK];
		char line[96];
		char hexline[96];
		int pos = 0;
		int hpos = 0;
		int i;
		pos += snprintf(line + pos, sizeof(line) - pos, "RX id=0x%03lX dlc=%u data:", (unsigned long)it->id, (unsigned int)it->dlc);
		for(i=0; i<it->dlc && pos < (int)sizeof(line) - 4; i++){
			pos += snprintf(line + pos, sizeof(line) - pos, " %02X", it->data[i]);
		}
		uart_printfln(&huart5, "%s", line);
		/* Also echo a pure hex line back to UART5: <StdID_3hex> <DLC_2hex> <data...> */
		hpos += snprintf(hexline + hpos, sizeof(hexline) - hpos, "%03lX %02X", (unsigned long)it->id & 0x7FFUL, (unsigned int)it->dlc);
		for(i=0; i<it->dlc && hpos < (int)sizeof(hexline) - 4; i++){
			hpos += snprintf(hexline + hpos, sizeof(hexline) - hpos, " %02X", it->data[i]);
		}
		uart_printfln(&huart5, "%s", hexline);
		/* pop */
		g_can_rxlog_tail = (uint8_t)((g_can_rxlog_tail + 1u) & CAN_RXLOG_MASK);
	}
}

__weak void can_rxMsg_handler(CAN_HandleTypeDef *hcan, CAN_RxHeaderTypeDef *rxHeader, uint8_t *rxbuff)
{
	UNUSED(hcan);
	UNUSED(rxHeader);
	UNUSED(rxbuff);
}

__weak void can_error_handler(CAN_HandleTypeDef *hcan)
{
	UNUSED(hcan);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef rxHeader;
	uint8_t rxbuff[8];
    char line[96];
    int pos, i;

	do{
		if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader, rxbuff) != HAL_OK){
			Error_Handler();
		}

		/* enqueue RX frame for logging outside ISR */
		{
			uint8_t next_head = (uint8_t)((g_can_rxlog_head + 1u) & CAN_RXLOG_MASK);
			if(next_head == g_can_rxlog_tail){
				/* drop oldest to make room */
				g_can_rxlog_tail = (uint8_t)((g_can_rxlog_tail + 1u) & CAN_RXLOG_MASK);
			}
			can_rxlog_item_t *dst = &g_can_rxlog_buf[g_can_rxlog_head & CAN_RXLOG_MASK];
			dst->dlc = rxHeader.DLC > 8 ? 8 : rxHeader.DLC;
			dst->id = (rxHeader.IDE ? rxHeader.ExtId : rxHeader.StdId);
			memcpy((void*)dst->data, rxbuff, dst->dlc);
			g_can_rxlog_head = next_head;
		}

        /* optional RX debug */
#ifdef VERBOSE_CAN_RX
        pos = 0;
        pos += snprintf(line + pos, sizeof(line) - pos, "RX0 id=0x%03X dlc=%u data:", (unsigned int)(rxHeader.IDE ? rxHeader.ExtId : rxHeader.StdId), (unsigned int)rxHeader.DLC);
        for(i=0; i<rxHeader.DLC && pos < (int)sizeof(line) - 4; i++){
            pos += snprintf(line + pos, sizeof(line) - pos, " %02X", rxbuff[i]);
        }
        uart_printfln(&huart4, "%s", line);
        uart_printfln(&huart5, "%s", line);
#endif

		can_rxMsg_handler(hcan, &rxHeader, rxbuff);
	}while(HAL_CAN_GetRxFifoFillLevel(hcan, CAN_RX_FIFO0) > 0);
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef rxHeader;
	uint8_t rxbuff[8];
    char line[96];
    int pos, i;

	do{
		if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &rxHeader, rxbuff) != HAL_OK){
			Error_Handler();
		}

		/* enqueue RX frame for logging outside ISR */
		{
			uint8_t next_head = (uint8_t)((g_can_rxlog_head + 1u) & CAN_RXLOG_MASK);
			if(next_head == g_can_rxlog_tail){
				/* drop oldest to make room */
				g_can_rxlog_tail = (uint8_t)((g_can_rxlog_tail + 1u) & CAN_RXLOG_MASK);
			}
			can_rxlog_item_t *dst = &g_can_rxlog_buf[g_can_rxlog_head & CAN_RXLOG_MASK];
			dst->dlc = rxHeader.DLC > 8 ? 8 : rxHeader.DLC;
			dst->id = (rxHeader.IDE ? rxHeader.ExtId : rxHeader.StdId);
			memcpy((void*)dst->data, rxbuff, dst->dlc);
			g_can_rxlog_head = next_head;
		}

        /* optional RX debug */
#ifdef VERBOSE_CAN_RX
        pos = 0;
        pos += snprintf(line + pos, sizeof(line) - pos, "RX1 id=0x%03X dlc=%u data:", (unsigned int)(rxHeader.IDE ? rxHeader.ExtId : rxHeader.StdId), (unsigned int)rxHeader.DLC);
        for(i=0; i<rxHeader.DLC && pos < (int)sizeof(line) - 4; i++){
            pos += snprintf(line + pos, sizeof(line) - pos, " %02X", rxbuff[i]);
        }
        uart_printfln(&huart4, "%s", line);
        uart_printfln(&huart5, "%s", line);
#endif

		can_rxMsg_handler(hcan, &rxHeader, rxbuff);
	}while(HAL_CAN_GetRxFifoFillLevel(hcan, CAN_RX_FIFO1) > 0);
}


void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	can_error_handler(hcan);
}

void can_activate_error_it(CAN_HandleTypeDef *hcan)
{
	can_activate_interrupt(hcan, CAN_IT_ERROR);
	can_activate_interrupt(hcan, CAN_IT_BUSOFF);
	can_activate_interrupt(hcan, CAN_IT_LAST_ERROR_CODE);
	can_activate_interrupt(hcan, CAN_IT_ERROR_PASSIVE);
	can_activate_interrupt(hcan, CAN_IT_ERROR_WARNING);
}


//#define CAN_TX_MAILBOX0             (0x00000001U)  /*!< Tx Mailbox 0  */
//#define CAN_TX_MAILBOX1             (0x00000002U)  /*!< Tx Mailbox 1  */
//#define CAN_TX_MAILBOX2             (0x00000004U)  /*!< Tx Mailbox 2  */

int can_sendStdMessage(CAN_HandleTypeDef *hcan, uint16_t stdId, uint8_t *txbuff, uint8_t dsize)
{
	CAN_TxHeaderTypeDef txHeader;
	uint32_t txMailbox;

	txHeader.IDE = CAN_ID_STD;
	txHeader.StdId = stdId;
//	txHeader.ExtId = 0;
	txHeader.RTR = CAN_RTR_DATA;
	txHeader.DLC = dsize;
	txHeader.TransmitGlobalTime = DISABLE;
	if(HAL_CAN_GetTxMailboxesFreeLevel(hcan) > 0){
		return HAL_CAN_AddTxMessage(hcan, &txHeader, txbuff, &txMailbox);
	}else{
		return -1;
	}
}

int can_sendExtMessage(CAN_HandleTypeDef *hcan, uint32_t extId, uint8_t *txbuff, uint8_t dsize)
{
	CAN_TxHeaderTypeDef txHeader;
	uint32_t txMailbox;

	txHeader.IDE = CAN_ID_EXT;
//	txHeader.StdId = stdId;
	txHeader.ExtId = extId;
	txHeader.RTR = CAN_RTR_DATA;
	txHeader.DLC = dsize;
	txHeader.TransmitGlobalTime = DISABLE;

	if(HAL_CAN_GetTxMailboxesFreeLevel(hcan) > 0){
		return HAL_CAN_AddTxMessage(hcan, &txHeader, txbuff, &txMailbox);
	}else{
		return -1;
	}
}



int can_config_filter(CAN_HandleTypeDef *hcan, uint32_t FxR1, uint32_t FxR2, uint8_t bank, uint8_t mode, uint8_t scale, uint8_t rxfifo)
{
    CAN_FilterTypeDef sFilterConfig;

    sFilterConfig.FilterBank           = bank;												// 设置过滤器组编号

	sFilterConfig.FilterMode = mode;
    sFilterConfig.FilterScale = scale ? CAN_FILTERSCALE_32BIT : CAN_FILTERSCALE_16BIT;
    sFilterConfig.FilterIdHigh         = (FxR1 >> 16) & 0xFFFF;	// 16bits mask
    sFilterConfig.FilterIdLow          = (FxR1 & 0xFFFF);
    sFilterConfig.FilterMaskIdHigh     = (FxR2 >> 16) & 0xFFFF;	// 16bits mask
    sFilterConfig.FilterMaskIdLow      = (FxR2 & 0xFFFF);
    sFilterConfig.FilterFIFOAssignment = rxfifo;
    sFilterConfig.FilterActivation     = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;
    if (HAL_CAN_ConfigFilter(hcan, &sFilterConfig) != HAL_OK){
        Error_Handler();
        return -1;
    }

    return 0;
}


void can_default_setup(CAN_HandleTypeDef *hcan)
{
	can_config_filter(hcan, 0x0, 0, 0, CAN_FILTERMODE_IDMASK, 0, CAN_FILTER_FIFO0);

	can_start(hcan);

	can_activate_interrupt(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
	can_activate_interrupt(hcan, CAN_IT_RX_FIFO1_MSG_PENDING);
	can_activate_error_it(hcan);
}

