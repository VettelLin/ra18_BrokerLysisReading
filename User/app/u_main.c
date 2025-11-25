/*
 * u_main.c
 *
 *  Created on: Sep 3, 2021
 *      Author: abel
 */




#include "version.h"
#include "lib/l_gpio.h"
#include "lib/l_timer.h"
#include "lib/l_uart.h"
#include "lib/l_delay.h"

#include "ra18/busmsg.h"
#include "app/u_can.h"
/* Start a temperature request burst so RX prints exactly 3 rows */
extern void tempburst_start(uint32_t now_ms);
extern void tempburst_task(void);
/* Drain deferred CAN RX logs (prints in main context) */
extern void canlog_drain(void);

// One-shot CAN self-test in loopback: send and synchronously read back
static void can_self_test(void)
{
	uint8_t d[2] = {0x5e, 0x55};
	CAN_RxHeaderTypeDef rxh;
	uint8_t rxb[8];
	uint32_t t0 = HAL_GetTick();
    int ret = can_sendStdMessage(&hcan, 0x0123, d, 2);
    (void)ret;
	while(HAL_GetTick() - t0 < 200){
		if(HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0) > 0){
			if(HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &rxh, rxb) == HAL_OK){
                /* silent */
				return;
			}
		}
	}
    /* silent */
}
// Periodic CAN request: send three frames every 5 seconds
// Tunable timings and verbose flags
#ifndef TEMP_QUERY_PERIOD_MS
#define TEMP_QUERY_PERIOD_MS 60000
#endif
#ifndef TEMP_QUERY_GAP_MS
#define TEMP_QUERY_GAP_MS     80
#endif
#ifdef VERBOSE_CAN_TX
#define LOG_TX(fmt, ...)  uart_printfln(&huart5, fmt, ##__VA_ARGS__)
#else
#define LOG_TX(fmt, ...)
#endif
static void send_periodic_temp_queries(void)
{
	static uint32_t lastTick = 0;
	uint32_t now = HAL_GetTick();
    // Send immediately on first call, then every TEMP_QUERY_PERIOD_MS
    if(lastTick != 0 && (now - lastTick) < TEMP_QUERY_PERIOD_MS){
		return;
	}
	lastTick = now;

	/* debug: confirm we are firing the temperature query burst */
	uart_printfln(&huart5, "TQ fire t=%lu", (unsigned long)now);

	/* announce a new burst for row aggregation */
	tempburst_start(now);

	uint8_t d1[2] = {0x5e, 0x00};
	uint8_t d2[2] = {0x5e, 0x01};
	uint8_t d3[2] = {0x5e, 0x02};

    // CAN StdID 0x63 as requested
    {
        int ret = can_sendStdMessage(&hcan, 0x0063, d1, 2);
        LOG_TX("TX1 ret=%d", ret); uart_printfln(&huart5, "TX1 ret=%d", ret);
    }

    /* Leave small spacing at 10 kbps so the node can process sequentially */
    HAL_Delay(TEMP_QUERY_GAP_MS);

    {
        int ret = can_sendStdMessage(&hcan, 0x0063, d2, 2);
        LOG_TX("TX2 ret=%d", ret); uart_printfln(&huart5, "TX2 ret=%d", ret);
    }

    HAL_Delay(TEMP_QUERY_GAP_MS);

    {
        int ret = can_sendStdMessage(&hcan, 0x0063, d3, 2);
        LOG_TX("TX3 ret=%d", ret); uart_printfln(&huart5, "TX3 ret=%d", ret);
    }

    /* silent */
}

// Polling fallback to verify CAN RX path (useful in loopback or if IRQ misconfigured)
static void poll_can_rx(void)
{
	CAN_RxHeaderTypeDef rxHeader;
	uint8_t rxbuff[8];
	while(HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0) > 0){
		if(HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &rxHeader, rxbuff) == HAL_OK){
			can_rxMsg_handler(&hcan, &rxHeader, rxbuff);
		}
	}
	while(HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO1) > 0){
		if(HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO1, &rxHeader, rxbuff) == HAL_OK){
			can_rxMsg_handler(&hcan, &rxHeader, rxbuff);
		}
	}
}

void user_setup(void)
{

	Init_UART_Receive_IT();

//	start_delay_timer();

    // Boot banner only on UART5
    uart_printfln(&huart5, "Broker boot ok, UART5=115200 8N1, tick=%lu", HAL_GetTick());
	printfln("start up device, system core clock frequence : %u, tick : %u ...", SystemCoreClock, HAL_GetTick());
	printfln("delay timer : psc %lu, arr %lu, cnt %lu", DELAY_TIMER->PSC, DELAY_TIMER->ARR, DELAY_TIMER->CNT);
	printfln("firmware version : %s", VERSION_STR);

	InitHeatingDock();
	init_bus_msg();


	// Accept all IDs, assign to FIFO0, use 32-bit scale to be robust across modes
	can_config_filter(&hcan, 0x0, 0, 0, CAN_FILTERMODE_IDMASK, 1, CAN_FILTER_FIFO0);
	can_start(&hcan);
	can_activate_interrupt(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
	can_activate_interrupt(&hcan, CAN_IT_RX_FIFO1_MSG_PENDING);
	can_activate_error_it(&hcan);

	// Run loopback self-test once
	can_self_test();

	// Trigger an immediate temperature query burst at boot
	send_periodic_temp_queries();

}


void user_loop(void)
{

	process_bus_msg();

	// Periodically poll temperatures of three heating strips
	send_periodic_temp_queries();

	// Poll CAN RX as a fallback (and for loopback verification)
	poll_can_rx();

	// Print any deferred CAN RX logs (hex dump)
	canlog_drain();

	// Flush temperature rows in main context to avoid ISR fragmentation
	tempburst_task();
}



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

}

//
void timer_base_user_handler(TIM_HandleTypeDef *htim)
{


}
