#ifndef __TYPES_H__
#define __TYPES_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Size constants */
#define KB(x) ((x) * 1024)
#define MB(x) ((x) * 1024 * 1024)
#define GB(x) ((x) * 1024 * 1024 * 1024)

/* Common type aliases */
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ullong;

/* NULL */
#ifndef NULL
#define NULL ((void *)0)
#endif

/* Boolean values */
#ifndef true
#define true 1
#define false 0
#endif

#endif // __TYPES_H__
