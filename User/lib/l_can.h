/*
 * l_can.h
 *
 *  Created on: Jun 10, 2022
 *      Author: abel
 */

#ifndef LIB_L_CAN_H_
#define LIB_L_CAN_H_

#include "can.h"


#define can_start(hcan)		HAL_CAN_Start(hcan)
#define can_stop(hcan)		HAL_CAN_Stop(hcan)

#define can_activate_interrupt(hcan, interrupt)	HAL_CAN_ActivateNotification(hcan, interrupt);
#define can_deactivate_interrupt(hcan, interrupt)	HAL_CAN_DeactivateNotification(hcan, interrupt);

extern void can_activate_error_it(CAN_HandleTypeDef *hcan);

extern int can_sendStdMessage(CAN_HandleTypeDef *hcan, uint16_t stdId, uint8_t *txbuff, uint8_t dsize);
extern int can_sendExtMessage(CAN_HandleTypeDef *hcan, uint32_t extId, uint8_t *txbuff, uint8_t dsize);

extern int can_config_filter(CAN_HandleTypeDef *hcan, uint32_t FxR1, uint32_t FxR2, uint8_t bank, uint8_t mode, uint8_t scale, uint8_t rxfifo);

extern void can_default_setup(CAN_HandleTypeDef *hcan);


#endif /* LIB_L_CAN_H_ */
