/*
 * msg.h
 *
 *  Created on: Jan 5, 2022
 *      Author: abel
 */

#ifndef INC_BUSMSG_H_
#define INC_BUSMSG_H_

#include <stdint.h>

#include "iamp_msg.h"


#define FUNC_CODE_START_HEAT		0X61
#define FUNC_CODE_STOP_HEAT			0X62
#define FUNC_CODE_GET_CURRENT_TEMPERATURE		0X63
#define FUNC_CODE_GET_TARGET_TEMPERATURE		0X64
#define FUNC_CODE_SET_TARGET_TEMPERATURE		0X65
#define FUNC_CODE_GET_DURATION		0X66
#define FUNC_CODE_SET_DURATION		0X67




extern void init_bus_msg(void);
extern void process_bus_msg(void);
extern int busmsg_send_data(uint8_t *txbuff, uint8_t *data, uint16_t data_len);
extern int busmsg_send_string(uint8_t *fmt, ...);

extern int write_bus_msg(uint8_t *data, int len);

#endif /* INC_MSG_H_ */
