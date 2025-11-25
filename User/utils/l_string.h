#ifndef _UTILS_STRING_H_
#define _UTILS_STRING_H_

#include <stdint.h>



void set_bits (uint8_t *buf, int byte_offset, int startbit, int bitlen,uint32_t data);
uint32_t get_bits(uint8_t *buf, int byte_offset, int startbit, int bitlen);

uint8_t get_1byte(void *data, int idx);
uint16_t get_2bytes(void *data, int idx);
uint32_t get_4bytes(void *data, int idx);

void set_1byte(void *data, int idx, uint8_t val);
void set_2byte(void *data, int idx, uint16_t val);
void set_4byte(void *data, int idx, uint32_t val);

void print_bytes(void *p_buf, int len);
int sprint_bytes(char *databuf, void *p_data, int len);
void print_uint16s(uint16_t *p_buff, int size);

void mem_set(void *p_byte, int size, long long value, int count);

uint32_t hex2int(char *p_hex, int len);
uint32_t byte2int(char *p_byte, int size);
int hex2byte(char *p_hex, int len, char* p_byte);
int byte2hex(char *p_byte, int len, char* p_hex);
char *byte2hexStr(char *p_byte, int len, char* p_hex);
int hexStr2bytes(char *p_hex, char* p_byte);

int ascii2num(char ascii);


void clean_maskbit(void *maskbit, int bit_num);
void set_maskbit(void *maskbit, int bit_num);
int	check_maskbit(void *maskbit, int bit_num);
void set_maskbits(void *maskbit, int offs, int bits);

char *strtrim(char *str);
int sameStarts(char *str1,char *str2);
int startsWith(char *str1, char *str2);

int wildcard_match(char *str, char *wildcard);



uint32_t little_endian(uint32_t o_int);
uint32_t big_endian(uint32_t o_int);
uint32_t endian_reverse(uint32_t o_int);




#endif
