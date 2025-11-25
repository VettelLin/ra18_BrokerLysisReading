/*
 * net.h
 *
 *  Created on: Jul 20, 2022
 *      Author: abel
 */

#ifndef UTILS_L_NET_H_
#define UTILS_L_NET_H_

#include <stdint.h>

int netmask_bits(uint32_t mask);
uint32_t ip_a2i(char *ipstr);
char *ip_i2a(uint32_t ipaddr);
char *ip_i2str(uint32_t ipaddr, char *ipstr);
char *ip_i2strxxx(uint32_t ipaddr, char *ipstr);
char *mac_arr2str(char *macaddr, char *macstr);
char *mac_arr2str1(char *macaddr, char *macstr);
char *mac_str2arr(char *macstr, char *macaddr);


#endif /* UTILS_L_NET_H_ */
