#ifndef _MY_MD5_H_
#define	_MY_MD5_H_


#define MD5_HASH_SIZE	16
#define MD5_HASH_HEXSIZE	(MD5_HASH_SIZE * 2)

#define MD5_DIGEST_SIZE 	MD5_HASH_SIZE

#define MD5_HASH_TXTSIZE    MD5_HASH_HEXSIZE
#define MD5_DIGEST_HEXSIZE 	MD5_HASH_TXTSIZE
#define MD5_DIGEST_TXTSIZE 	MD5_HASH_TXTSIZE

typedef struct {
    unsigned int state[4];
    unsigned int count[2];
    unsigned char buffer[64];
} md5_context_st;

int calc_md5(char *digest, unsigned char *buf, unsigned int length);
int calc_md5hex(char *digest, unsigned char *buf, unsigned int length);

char *get_md5(unsigned char *strbuff, unsigned char *data, unsigned int length, unsigned char *prefix);


int check_md5(char* data, int len, char *md5_digest);


#endif
