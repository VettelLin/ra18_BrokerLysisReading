/*
 * crc.c
 *
 *  Created on: Jun 4, 2022
 *      Author: abel
 */

#include <utils/l_crc.h>


#if 1
uint8_t calc_crc8(uint8_t *ptr, int len)
{
	uint8_t i;
	uint8_t crc=0;
	uint8_t crcb;
	uint8_t x;
	for (x=0;x<len;x++) {
		crcb = ptr[x];
		for(i=0; i<8; i++) {
 			if(((crc ^ crcb)&1)!=0) {
				crc ^=0x18;
				crc >>=1;
				crc |= 0x80;
			}
			else
				crc>>=1;

 			crcb >>= 1;
 		}
	}
	return(crc);
}


uint16_t docrc16(uint16_t cdata,uint16_t CRC)
{
	uint16_t CRC16 = CRC;
	uint16_t oddparity[16] = { 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0 };
	cdata = (cdata ^ (CRC16 & 0xff)) & 0xff;
	CRC16 >>= 8;

	if (oddparity[cdata & 0xf] ^ oddparity[cdata >> 4])
		CRC16 ^= 0xc001;

	cdata <<= 6;
	CRC16 ^= cdata;
	cdata <<= 1;
	CRC16 ^= cdata;

	return CRC16;
}

uint16_t calc_crc16(unsigned char *ptr, int len)
{
	uint8_t x;
	uint16_t crc=0;

	for (x=0;x<len;x++) {
		crc = docrc16(ptr[x],crc);

	}

	return(crc);
}
#endif


uint32_t CRCByte(uint32_t crc, uint8_t ch)
{
	uint8_t bb3;
	uint8_t i,temp,temp2;

	bb3 = 0;
	for (i = 0 ;i < 8; i++) {
		temp = (crc >> 24) & 0x000000ff;
		temp2 = ch ^ temp;

		ch = ch << 1;
		if (bb3) ch ++;

		if ((crc & 0x80000000)) bb3 = 1;
			else bb3 = 0;
		crc = crc << 1;
		if ((temp2 & 0x80)) crc ++;

		if ((temp2 & 0x80)) crc = crc ^ 0x04c11db6;
	}
	return crc;
}



uint32_t calc_crc32(uint8_t *point, int length)
{
	uint32_t crc = 0xffffffff;

	uint16_t ii;
	uint8_t bb3,ch;
	uint8_t i,temp,temp2;

	for (ii = 0; ii < length; ii++)
	{
		bb3 = 0;
		ch = point[ii];
		for (i = 0 ;i < 8; i++) {
			temp = (crc >> 24) & 0x000000ff;
			temp2 = ch ^ temp;

			ch = ch << 1;
			if (bb3) ch ++;

			if ((crc & 0x80000000)) bb3 = 1;
				else bb3 = 0;
			crc = crc << 1;
			if ((temp2 & 0x80)) crc ++;

			if ((temp2 & 0x80)) crc = crc ^ 0x04c11db6;
		}
		//crc = CRCByte(crc,point[ii]);
	}

	return crc;
}
