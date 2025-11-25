
#ifndef INC_IAMP_MSG_H_
#define INC_IAMP_MSG_H_

#include <stdint.h>

#if 1
#define PCR_STARTHEAT		0X61
#define PCR_STOPHEAT		0X62
#define PCR_CURTEMP		0X63
#define PCR_GETTEMP		0X64
#define PCR_SETTEMP		0X65
#define PCR_GETTIME		0X66
#define PCR_SETTIME		0X67
#define PCR_INPLACE		0X68
#else
#define FUNC_CODE_START_HEAT		0X61
#define FUNC_CODE_STOP_HEAT			0X62
#define FUNC_CODE_GET_CURRENT_TEMPERATURE		0X63
#define FUNC_CODE_GET_TARGET_TEMPERATURE		0X64
#define FUNC_CODE_SET_TARGET_TEMPERATURE		0X65
#define FUNC_CODE_GET_DURATION		0X66
#define FUNC_CODE_SET_DURATION		0X67
#define FUNC_CODE_DETECT_DOCK		0X68
#endif



void init_bus_msg(void);
void process_bus_msg(void);

int ps96msg_send_inplace(void);

#endif /* INC_MSG_H_ */
