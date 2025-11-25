/*
 * can_msg.h
 *
 *  Created on: May 9, 2025
 *      Author: abel
 */

#ifndef RA18_CAN_MSG_H_
#define RA18_CAN_MSG_H_

#include "app/u_can.h"


#include "iamp_msg.h"

#define can_sendmsg(canId, txbuff, dsize)	can_sendStdMessage(&hcan, canId, txbuff, dsize)


#endif /* RA18_CAN_MSG_H_ */
