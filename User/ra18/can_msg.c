/*
 * can_msg.c
 *
 *  Created on: May 9, 2025
 *      Author: abel
 */


#include "can_msg.h"
#include "lib/l_uart.h"
#include "main.h"
#include <string.h>



static inline int can_replymsg(uint16_t canId, uint8_t *txbuff, uint8_t dsize)
{
	return can_sendStdMessage(&hcan, canId | 0x80, txbuff, dsize);
}

/* Burst aggregator: print exactly three rows per request cycle */
static struct {
	uint8_t expecting;
	uint8_t mask;           /* bit0->Row1, bit1->Row2, bit2->Row3 */
	uint32_t deadline_ms;   /* flush deadline */
    uint8_t row[4][5];      /* saved 5 bytes for Row1..3 */
} temp_burst_ctx = {0};

void tempburst_start(uint32_t now_ms)
{
	temp_burst_ctx.expecting = 1;
	temp_burst_ctx.mask = 0;
    temp_burst_ctx.deadline_ms = now_ms + 1500; /* allow up to 1.5s for responses at 10 kbps */
    /* preset all bytes to missing marker */
    memset(&temp_burst_ctx.row[0][0], 0xFF, sizeof(temp_burst_ctx.row));
}

/* Called from main loop to print once ready or timeout */
void tempburst_task(void)
{
	if(!temp_burst_ctx.expecting){
		return;
	}
	if((temp_burst_ctx.mask & 0x07) == 0x07 || (int32_t)(HAL_GetTick() - temp_burst_ctx.deadline_ms) >= 0){
		/* Build all three rows in ONE TX to avoid any mid-line fragmentation */
		char out[128];
		int pos = 0;
        uint8_t dash[5] = {0xFF,0xFF,0xFF,0xFF,0xFF};
		uint8_t *r1 = (temp_burst_ctx.mask & 0x01) ? temp_burst_ctx.row[1] : dash;
		uint8_t *r2 = (temp_burst_ctx.mask & 0x02) ? temp_burst_ctx.row[2] : dash;
		uint8_t *r3 = (temp_burst_ctx.mask & 0x04) ? temp_burst_ctx.row[3] : dash;
        pos += snprintf(out + pos, sizeof(out) - pos,
            "Row1: %s %s %s %s %s\r\n",
            (r1[0]==0xFF?"--":(snprintf(out+pos,0,""),"")),"","","",""); /* placeholder to align below appends */
        /* Rewrite last appended line properly with per-byte formatting */
        pos = 0;
        pos += snprintf(out + pos, sizeof(out) - pos, "Row1: ");
        pos += snprintf(out + pos, sizeof(out) - pos, (r1[0]==0xFF?"--":"%02X"), r1[0]); pos += snprintf(out + pos, sizeof(out) - pos, " ");
        pos += snprintf(out + pos, sizeof(out) - pos, (r1[1]==0xFF?"--":"%02X"), r1[1]); pos += snprintf(out + pos, sizeof(out) - pos, " ");
        pos += snprintf(out + pos, sizeof(out) - pos, (r1[2]==0xFF?"--":"%02X"), r1[2]); pos += snprintf(out + pos, sizeof(out) - pos, " ");
        pos += snprintf(out + pos, sizeof(out) - pos, (r1[3]==0xFF?"--":"%02X"), r1[3]); pos += snprintf(out + pos, sizeof(out) - pos, " ");
        pos += snprintf(out + pos, sizeof(out) - pos, (r1[4]==0xFF?"--":"%02X"), r1[4]);
        pos += snprintf(out + pos, sizeof(out) - pos, "\r\n");

        pos += snprintf(out + pos, sizeof(out) - pos, "Row2: ");
        pos += snprintf(out + pos, sizeof(out) - pos, (r2[0]==0xFF?"--":"%02X"), r2[0]); pos += snprintf(out + pos, sizeof(out) - pos, " ");
        pos += snprintf(out + pos, sizeof(out) - pos, (r2[1]==0xFF?"--":"%02X"), r2[1]); pos += snprintf(out + pos, sizeof(out) - pos, " ");
        pos += snprintf(out + pos, sizeof(out) - pos, (r2[2]==0xFF?"--":"%02X"), r2[2]); pos += snprintf(out + pos, sizeof(out) - pos, " ");
        pos += snprintf(out + pos, sizeof(out) - pos, (r2[3]==0xFF?"--":"%02X"), r2[3]); pos += snprintf(out + pos, sizeof(out) - pos, " ");
        pos += snprintf(out + pos, sizeof(out) - pos, (r2[4]==0xFF?"--":"%02X"), r2[4]);
        pos += snprintf(out + pos, sizeof(out) - pos, "\r\n");

        pos += snprintf(out + pos, sizeof(out) - pos, "Row3: ");
        pos += snprintf(out + pos, sizeof(out) - pos, (r3[0]==0xFF?"--":"%02X"), r3[0]); pos += snprintf(out + pos, sizeof(out) - pos, " ");
        pos += snprintf(out + pos, sizeof(out) - pos, (r3[1]==0xFF?"--":"%02X"), r3[1]); pos += snprintf(out + pos, sizeof(out) - pos, " ");
        pos += snprintf(out + pos, sizeof(out) - pos, (r3[2]==0xFF?"--":"%02X"), r3[2]); pos += snprintf(out + pos, sizeof(out) - pos, " ");
        pos += snprintf(out + pos, sizeof(out) - pos, (r3[3]==0xFF?"--":"%02X"), r3[3]); pos += snprintf(out + pos, sizeof(out) - pos, " ");
        pos += snprintf(out + pos, sizeof(out) - pos, (r3[4]==0xFF?"--":"%02X"), r3[4]);
        pos += snprintf(out + pos, sizeof(out) - pos, "\r\n");
        // Print temperature rows on UART4
        uart_write(&huart4, (uint8_t *)out, pos);
		temp_burst_ctx.expecting = 0;
		temp_burst_ctx.mask = 0;
	}
}



void can_user_RxCallback(CAN_HandleTypeDef *hcan, CAN_RxHeaderTypeDef *pHead, uint8_t *rxbuff)
{
	int ret = 0;
	uint32_t canid;
	uint8_t *rxdata;
	uint8_t rxlen;

	if(pHead->RTR || pHead->DLC==0){
		return;
	}

	if(pHead->IDE){
		canid = pHead->ExtId;
	}else{
		canid = pHead->StdId;
	}

	rxdata = rxbuff;
	rxlen = pHead->DLC;

	heatdock_response_handler((canid >> 8) & 0xf, canid & 0xff, rxdata, rxlen);

	/* Capture rows for the current burst; we flush in order later */

    // Capture temperature rows:
    // - Accept frames with header 0x5E,<idx>
    // - Accept frames starting directly with <idx> (some nodes omit 0x5E)
    if(pHead->DLC >= 2){
        uint8_t hasHeader = (rxdata[0] == 0x5E);
        uint8_t idxRaw = hasHeader ? rxdata[1] : rxdata[0];   /* 0..2 or 1..3 */
        uint8_t tcIdx = idxRaw;
        /* Normalize 0..2 -> 1..3 to match internal row slots */
        if(tcIdx <= 2) tcIdx = (uint8_t)(tcIdx + 1u);
        if(tcIdx >= 1 && tcIdx <= 3){
            if(temp_burst_ctx.expecting){
                /* Fill row buffer with available content; ensure first two bytes are 5E,idx */
                int i;
                /* Preset all bytes to missing marker; then fill as available */
                for(i=0; i<5; i++){
                    temp_burst_ctx.row[tcIdx][i] = 0xFF;
                }
                temp_burst_ctx.row[tcIdx][0] = 0x5E;
                temp_burst_ctx.row[tcIdx][1] = (uint8_t)tcIdx;
                /* Copy remaining bytes:
                 * - If header is present: source starts at rxdata[2]
                 * - If header omitted:    source starts at rxdata[1] (shift by one)
                 */
                {
                    int src_start = hasHeader ? 2 : 1;
                    int dst = 2;
                    while(dst < 5 && src_start < rxlen){
                        temp_burst_ctx.row[tcIdx][dst++] = rxdata[src_start++];
                    }
                }
                /* mark row present */
                temp_burst_ctx.mask |= (1U << (tcIdx - 1));
            }
        }
    }

}
