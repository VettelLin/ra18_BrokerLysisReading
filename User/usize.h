#ifndef ___USER_SIZE_DEF_H___
#define ___USER_SIZE_DEF_H___

#define SZ_1				0x00000001
#define SZ_2				0x00000002
#define SZ_4				0x00000004
#define SZ_8				0x00000008
#define SZ_16				0x00000010
#define SZ_32				0x00000020
#define SZ_64				0x00000040
#define SZ_128				0x00000080
#define SZ_256				0x00000100
#define SZ_512				0x00000200

#define SZ_1K				0x00000400
#define SZ_2K				0x00000800
#define SZ_4K				0x00001000
#define SZ_8K				0x00002000
#define SZ_16K				0x00004000
#define SZ_32K				0x00008000
#define SZ_64K				0x00010000
#define SZ_128K				0x00020000
#define SZ_256K				0x00040000
#define SZ_512K				0x00080000

#define SZ_1M				0x00100000
#define SZ_2M				0x00200000
#define SZ_4M				0x00400000
#define SZ_8M				0x00800000
#define SZ_16M				0x01000000
#define SZ_32M				0x02000000
#define SZ_64M				0x04000000
#define SZ_128M				0x08000000
#define SZ_256M				0x10000000
#define SZ_512M				0x20000000

#define SZ_1G				0x40000000
#define SZ_2G				0x80000000

#define SZ_10M				(SZ_8M + SZ_2M)
#define SZ_20M				(SZ_16M + SZ_4M)
#define SZ_30M				(SZ_32M - SZ_2M)
#define SZ_40M				(SZ_32M + SZ_8M)

#define SZ_12M				(SZ_8M + SZ_4M)
#define SZ_24M				(SZ_16M + SZ_8M)
#define SZ_36M				(SZ_32M + SZ_4M)
#define SZ_48M				(SZ_32M + SZ_16M)
#define SZ_60M				(SZ_64M - SZ_4M)

#define BITS_1BYTE			(8*1)
#define BITS_2BYTE			(8*2)
#define BITS_3BYTE			(8*3)
#define BITS_4BYTE			(8*4)


#define N_BITS_MASK(n)		((1ULL<<(n))-1)
#define N_BITS_MASK_OFFS_M(n, m)		(N_BITS_MASK(n)<<m)


#define ARRAY_SIZE(arr)	(sizeof(arr)/sizeof(arr[0]))





#endif
