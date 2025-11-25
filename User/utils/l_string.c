

#include <string.h>

#include <stdarg.h>


#include "utils/l_string.h"

#include "lib/l_uart.h"


#include "porting.h"


int sprint_bytes(char *databuf, void *p_data, int len)
{
	uint8_t *p_buf;

	int i;
	int row_size = 16;
	int slen = 0;;

	if((databuf==NULL) || (p_data == NULL) || (len <= 0)){
		return 0;
	}

	p_buf = (uint8_t *)p_data;

	for(i=0; i<len; i++){
		if(i>0 && i%row_size){
			slen += sprintf(databuf+slen, EndCharsOfLine);
		}
		slen += sprintf(databuf+slen,"%.2x ", p_buf[i]);
	}

	slen += sprintf(databuf+slen, EndCharsOfLine);

	return slen;
}

void print_bytes(void *p_data, int len)
{
	int i;
	uint8_t *p_buf;

	if((p_data == NULL) || (len <= 0)){
		return;
	}

	p_buf = (uint8_t *)p_data;

	printf("--------------print_bytes, len = %d-----------------"EndCharsOfLine, len);

	for(i=0; i<len; i++){
		if(i>0 && i%16 == 0){
			printf(EndCharsOfLine);
		}
		printf("%.2x ", p_buf[i]);
	}

	printf(EndCharsOfLine);

}

void print_uint16s(uint16_t *p_buff, int size)
{
	int i;
	if((p_buff == NULL) || (size <= 0)){
		return;
	}

//	printf("--------------print_uint16s, size = %d-----------------"EndCharsOfLine, size);

	for(i=0; i<size; i++){
		if(i>0 && i%16 == 0){
			printf(EndCharsOfLine);
		}
		printf("%d ", p_buff[i]);
	}

	printf(EndCharsOfLine);

}


void set_bits (uint8_t *buf, int byte_offset, int startbit, int bitlen,uint32_t data)
{
        uint8_t  *b;
        uint32_t  v;
        uint32_t  mask;
        uint32_t tmp_long;

        mask = (1ULL << bitlen) - 1;
        v = data & mask;

        b = &buf[byte_offset + (startbit / 8)];
        startbit %= 8;

        tmp_long = (uint32_t)( ((*b)<<24) + (*(b+1)<<16) + (*(b+2)<<8) + *(b+3) );

        startbit = 32 - startbit - bitlen;
        v= v << startbit;
        mask = mask << startbit;

        tmp_long &= ~mask;
        tmp_long |= v;

        *b = (tmp_long >> 24);
        *(b+1) = (tmp_long >> 16);
        *(b+2) = (tmp_long >> 8);
        *(b+3) = (tmp_long);
}


uint32_t get_bits(uint8_t *buf, int byte_offset, int startbit, int bitlen)
{
	uint8_t *b;
	uint32_t v;
	uint32_t mask;
	uint32_t tmp_long;

	b = &buf[byte_offset + (startbit / 8)];
	startbit %= 8;
	
	tmp_long = (uint32_t)(((*b) << 24) + (*(b+1) << 16) + (*(b + 2) << 8) + *(b + 3));
	startbit = 32 - startbit - bitlen;

	tmp_long = tmp_long >> startbit;

	mask = (1ULL << bitlen) - 1;

	v = tmp_long & mask;

	return v;
}


uint8_t get_1byte(void *data, int idx)
{
	uint8_t *pb = data;

	return pb[idx];
}

uint16_t get_2bytes(void *data, int idx)
{
	uint16_t *pbs = (uint8_t *)data + idx;

	return *pbs;
}

uint32_t get_4bytes(void *data, int idx)
{
	uint32_t *pbs = (uint8_t *)data + idx;

	return *pbs;
}

void set_1byte(void *data, int idx, uint8_t val)
{
	uint8_t *pb = data;
	pb[idx] = val;
}

void set_2byte(void *data, int idx, uint16_t val)
{
	uint16_t *pb = (uint8_t *)data + idx;;
	memcpy(pb, &val, 2);
}

void set_4byte(void *data, int idx, uint32_t val)
{
	uint32_t *pb = (uint8_t *)data + idx;;
	memcpy(pb, &val, 4);
}

void mem_set(void *p_byte, int size, long long value, int count)
{
	int i;
	count *= size;
	for(i=0; i<count; i+=size){
		memcpy(p_byte+i, &value, size);
	}
}


char hex2upper(char hexnum)
{
	char ch;
	
	if (hexnum>=0 && hexnum<=9){
		ch = hexnum + '0';
	}else if(hexnum>=0xa && hexnum<=0xf){
        ch = hexnum - 10 + 'A' ;
	}else{
		ch = hexnum;
	}

	return ch;
}

char hex2lower(char hexnum)
{
	char ch;
	
	if (hexnum>=0 && hexnum<=9){
		ch = hexnum + '0';
	}else if(hexnum>=0xa && hexnum<=0xf){
        ch = hexnum - 10 + 'a' ;
	}else{
		ch = hexnum;
	}

	return ch;
}

int ascii2num(char ascii)
{
	if((ascii >= '0') && (ascii <= '9')){
		return (ascii - '0');
	}else if((ascii >= 'A') && (ascii <= 'F')){
		return (ascii - 'A' + 10);
	}else if((ascii >= 'a') && (ascii <= 'f')){
		return (ascii - 'a' + 10);
	}

	return 0;
}

int byte2hex(char *p_byte, int len, char* p_hex)
{
    int  i;

	if(p_byte == NULL || len <= 0 || p_hex== NULL){
		return 0;
	}

    for (i=0; i<len; i++)
    {
        *p_hex++ = hex2upper((*p_byte >> 4) & 0xf);
        *p_hex++ = hex2upper(*p_byte & 0xf);
		p_byte++;
    }

    return (len << 1);
}

char *byte2hexStr(char *p_byte, int len, char* p_hex)
{
	len = byte2hex(p_byte, len, p_hex);
	p_hex[len] = 0;

    return p_hex;
}

int hex2byte(char *p_hex, int len, char* p_byte)
{
    int  i;

	if(p_byte == NULL || len <= 0 || p_hex== NULL){
		return 0;
	}

	if(len%2){
		len--;
	}

    for (i=0; i<len; i+=2)
    {
        *p_byte = ascii2num(*p_hex++) << 4;
        *p_byte |= ascii2num(*p_hex++) & 0xf;
		p_byte++;
    }

    return (len/2);
}


int hexStr2bytes(char *p_hex, char* p_byte)
{
	int  i, len=0;

	if(p_byte == NULL || len <= 0 || p_hex== NULL){
		return 0;
	}

	while(*p_hex){
		p_byte[len] = ascii2num(*p_hex++) << 4;
      		p_byte[len] |= ascii2num(*p_hex) & 0xf;
		len++;
		if(!(*p_hex++) ){
			break;
		}
	}


	return len;
}

uint32_t byte2int(char *p_byte, int size)
{
	int i;
	uint32_t num = 0;

	if(size < 0 || size > 8 || p_byte == NULL){
		return 0;
	}
	
	for(i=0; i<size; i++){
		num <<= 8;
		num |= p_byte[i]&0xff;
	}

	return num;
}


uint32_t hex2int(char *p_hex, int len)
{
    int  i;
	uint32_t val = 0;
	
	if(len <= 0 || p_hex== NULL){
		return 0;
	}


    for (i=0; i<len; i++)
    {
    	if(*p_hex == 0)break;
		val <<= 4;
        val |= (ascii2num(*p_hex++)& 0xf);
    }

    return val;
}


void clean_maskbit(void *maskbit, int bit_num)
{
	char *mask;
	int u8_bits;
	
	if(maskbit == NULL){
		return;
	}

	mask = (char *)maskbit;
	u8_bits = 8;
	mask[bit_num/u8_bits] &= ~(1<<(bit_num%u8_bits));
	
}

void set_maskbits(void *maskbit, int offs, int bits)
{
	uint8_t mask, *p_byte;
	int i, v_s32;
	int s_b, e_b;
	
	if((maskbit == NULL) || (bits <= 0)){
		return;
	}

	p_byte = (char *)maskbit;

	s_b = offs;
	e_b = offs + bits - 1;

	i = s_b / 8;

	while (e_b >= 0){
		mask = 0xff << s_b; 
		if(e_b >= 8){
			s_b = 0;
		}else{
			v_s32 = 7 - e_b;
			mask << v_s32;
			mask >> v_s32;
		}
		p_byte[i] |= mask;
		e_b -= 8;
		i++;
	}

}

void set_maskbit(void *maskbit, int bit_num)
{
	char *mask;
	int u8_bits;
	
	if(maskbit == NULL){
		return;
	}

	mask = (char *)maskbit;
	u8_bits = 8;
	mask[bit_num/u8_bits] |= (1<<(bit_num%u8_bits));
	
}

int check_maskbit(void *maskbit, int bit_num)
{
	char *mask;
	int u8_bits;

	if(maskbit == NULL){
		return 0;
	}

	mask = (char *)maskbit;
	u8_bits = 8;

	return mask[bit_num/u8_bits] & (1<<(bit_num%u8_bits));
	
}


//bigend and littleend convert
uint32_t endian_reverse(uint32_t o_int)
{
	uint32_t res;

	res = (o_int & 0xff) << 24;
	res |= (o_int & 0xff00) << 8;
	res |= (o_int >> 8) & 0xff00;
	res |= (o_int >> 24) & 0xff;

	return res;
}

//if the cpu is littleend then the endian convert
uint32_t big_endian(uint32_t o_int)
{
	uint8_t *p_ch;
	uint32_t res;

	p_ch = (uint8_t *)(&res);

	*p_ch++ = (o_int >> 24)  & 0xff;
	*p_ch++ = (o_int >> 16)  & 0xff;
	*p_ch++ = (o_int >> 8)  & 0xff;
	*p_ch++ = (o_int >> 0)  & 0xff;

	return res;
}

//if the cpu is bigend then the endian convert
uint32_t little_endian(uint32_t o_int)
{
	uint8_t *p_ch;
	uint32_t res;

	p_ch = (uint8_t *)(&res);

	*p_ch++ = (o_int >> 0)  & 0xff;
	*p_ch++ = (o_int >> 8)  & 0xff;
	*p_ch++ = (o_int >> 16)  & 0xff;
	*p_ch++ = (o_int >> 24)  & 0xff;

	return res;
}

int is_space(char ch){
	return (ch)&& !(ch >= 33 && ch <= 126);
}

char *strtrim(char *str)
{
	char * tail;
	
	while(is_space(*str))str++;
	if(*str == 0){
		return str;
	}
	tail = str;
	while(*++tail);
	while(is_space(*--tail))*tail = '\0';
	
	return str;
}

int sameStarts(char *str1,char *str2)
{
	int count = 0;
	if(str1 == 0 || str2 == 0)
		return 0;
	while(1){
		if(*str1++ == *str2++)
			count++;
		else
			break;
	}
	return count;
}

int startsWith(char *str1, char *str2)
{

	while(1){
		if(!*str2){
			return 1;
		}else if(!*str1 && *str2){
			return 0;
		}
		if(*str2++ != *str1++){
			return 0;
		}
	}
	return 0;
}

//
int wildcard_match(char *str, char *wildcard)
{
	char *pcur, *pwcur;
	char *pwa = NULL, *pwq = NULL;
	char *pm = NULL, *pwm = NULL;
	char buff[128];
	char curch;
	int slen;

	if(str == NULL){
		return 0;
	}

	if((wildcard == NULL) || (wildcard == "*")){
		return 1;
	}

//	printf("wildcard_match : %s in %s \n", wildcard, str);

	strcpy(buff, wildcard);

	pcur = str;
	pwcur = buff;

	while(*pcur && *pwcur){

//		printf("wildcard_match : *pcur = %c, *pwcur = %c \n", *pcur, *pwcur);

		if(*pwcur == '*'){
			pwa = pwcur;
		}
		else if(*pwcur == '?'){
			if(pwa == NULL){
				pwq = pwcur;
			}
		}
		else {
			if(*pcur != *pwcur){
				if(pwq){
					if(*++pcur != *pwcur){
						return 0;
					}
					pwq = NULL;
				}else if(pwa){

					pwm = strpbrk(pwcur, "*?");
					if(pwm){

						slen = (pwm - pwcur);

						if(*pwm == '*'){
							pwa = pwm;

						}else{
							pwq = pwm;
						}

						curch = *pwm;
						*pwm = 0;

						pm = strstr(pcur, pwcur);

//						printf("wildcard_match : curch = %c, slen = %d, *pm = %c \n", curch, slen, pm?*pm:0);

						if(pm){
							pcur = pm + slen;
						}else{
							return 0;
						}

						pwcur = pwm;

					}else{

						slen = strlen(pwcur);
						pm = strstr(pcur, pwcur);

//						printf("wildcard_match : pwm == NULL, slen = %d, *pm = %c \n", slen, pm?*pm:0);

						if(pm){
							return (pm[slen] == 0);
						}else{
							return 0;
						}

					}

				}else{
					return 0;
				}
			}
		}

		pwcur++;
		if(pwa==NULL && pwq==NULL){
			pcur++;
		}
	}

	return 1;
}
