#ifndef types_h
#define types_h

#include <stdbool.h>
#include <stddef.h>

// literally isnt used, just put it here as an idea, please remove it eventually

typedef signed char    i8;
typedef signed short   i16;
typedef signed int     i32;
typedef signed long    i64;

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned long  u64;

typedef float           f32;
typedef double         f64;

typedef size_t         usize;
typedef ptrdiff_t      isize;

#define null (void *)0

#endif 