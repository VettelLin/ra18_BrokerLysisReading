/*
 * porting.h
 *
 *  Created on: Jun 4, 2022
 *      Author: abel
 */

#ifndef PORTING_H_
#define PORTING_H_



#define EndCharsOfLine		"\r\n"
#define CmdPrompt		"ra18@broker:# "

// Set to 1 if your board wires CAN1 to PB8(PHY RX) / PB9(PHY TX) via remap.
// Set to 0 to use default PA11/PA12.
#ifndef USE_CAN1_REMAP_PB8_PB9
#define USE_CAN1_REMAP_PB8_PB9		0
#endif


#endif /* PORTING_H_ */
