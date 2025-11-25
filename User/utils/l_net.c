/*
 * net.c
 *
 *  Created on: Jul 20, 2022
 *      Author: abel
 */



#include <stdio.h>
#include <string.h>
#include "l_net.h"
#include "l_string.h"

int netmask_bits(uint32_t mask)
{
	int i;

	for(i=31; i>=0; i--){
		if(!((mask>>i) & 1)){
			return (31-i);
		}
	}

	return 32;
}

//little end
char *ip_i2strxxx(uint32_t ipaddr, char *ipstr)
{
	uint8_t *pch_ip;

	pch_ip = (char *)(&ipaddr);

	sprintf(ipstr, "%.3u.%.3u.%.3u.%.3u", pch_ip[3], pch_ip[2], pch_ip[1], pch_ip[0]);

	return ipstr;
}

//little end
char *ip_i2str(uint32_t ipaddr, char *ipstr)
{
	uint8_t *pch_ip;

	pch_ip = (char *)(&ipaddr);

	sprintf(ipstr, "%u.%u.%u.%u", pch_ip[3], pch_ip[2], pch_ip[1], pch_ip[0]);

	return ipstr;
}

//little end
char *ip_i2a(uint32_t ipaddr)
{
	uint8_t *pch_ip;
	static char ipstr[16];

	pch_ip = (char *)(&ipaddr);

	sprintf(ipstr, "%u.%u.%u.%u", pch_ip[3], pch_ip[2], pch_ip[1], pch_ip[0]);

	return ipstr;
}

//little end
uint32_t ip_a2i(char *ipstr)
{
	uint8_t *p_ch, *pch_ip;
	uint32_t ipaddr;
	long i, val;

	pch_ip = (char *)(&ipaddr);

	p_ch = ipstr;

	for(i=3; i>=0; i--){
		val = (strtol(p_ch, &p_ch, 10) & 0xff);
		if(val == 0){
			if(i>0){
				if(*p_ch != '.'){
					return 0;
				}
			}else if((*p_ch > 0x20)){
				return 0;
			}
		}else if((val < 0) || (val > 255)){
			return 0;
		}
		pch_ip[i] = val;
		p_ch++;
	}

	//ToPrintf("ip_a2i :  %s [%#.8x] \n", ipstr, ipaddr);

	return ipaddr;
}

char *mac_arr2str(char *macaddr, char *macstr)
{
	static char strbuf[24];

	if(macstr==NULL){
		macstr = strbuf;
	}

	sprintf(macstr, "%02X:%02X:%02X:%02X:%02X:%02X"
		, macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);

	return macstr;
}

char *mac_arr2str1(char *macaddr, char *macstr)
{
	static char strbuf[24];

	if(macstr==NULL){
		macstr = strbuf;
	}

	sprintf(macstr, "%02X%02X%02X:%02X%02X%02X"
		, macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);

	return macstr;
}

char *mac_str2arr(char *macstr, char *macaddr)
{
	macaddr[0] = ((ascii2num(macstr[0])) << 4 & 0xf0) |((ascii2num(macstr[1])) & 0x0f);
	macaddr[1] = ((ascii2num(macstr[3])) << 4 & 0xf0) |((ascii2num(macstr[4])) & 0x0f);
	macaddr[2] = ((ascii2num(macstr[6])) << 4 & 0xf0) |((ascii2num(macstr[7])) & 0x0f);
	macaddr[3] = ((ascii2num(macstr[9])) << 4 & 0xf0) |((ascii2num(macstr[10])) & 0x0f);
	macaddr[4] = ((ascii2num(macstr[12])) << 4 & 0xf0) |((ascii2num(macstr[13])) & 0x0f);
	macaddr[5] = ((ascii2num(macstr[15])) << 4 & 0xf0) |((ascii2num(macstr[16])) & 0x0f);

	return macaddr;
}
