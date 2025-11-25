

#ifndef INC_VERSION_H_
#define INC_VERSION_H_



#define VER_MAJOR	0
#define VER_MINOR	0
#define VER_PATCH	8
#define VERSION		(VER_MAJOR << 16 | VER_MINOR << 8 | VER_PATCH)

#define __MAKE_VERSION_STRING(major, minor, patch)	#major"."#minor"."#patch
#define __VERSION_STRING(major, minor, patch)		__MAKE_VERSION_STRING(major, minor, patch)
#define VERSION_STR		__VERSION_STRING(VER_MAJOR, VER_MINOR, VER_PATCH)
//#define VERSION_STR		#VER_MAJOR"."#VER_MINOR"."#VER_PATCH

#endif /* INC_VERSION_H_ */
