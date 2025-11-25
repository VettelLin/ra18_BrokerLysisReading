/*
 * crc.h
 *
 *  Created on: Jun 4, 2022
 *      Author: abel
 */

#ifndef UTILS_L_CRC_H_
#define UTILS_L_CRC_H_


#include <stdint.h>


uint8_t calc_crc8(uint8_t *ptr, int len) ;
uint16_t calc_crc16(uint8_t *ptr, int len) ;
uint32_t calc_crc32(uint8_t *point, int length);

#endif /* UTILS_L_CRC_H_ */
