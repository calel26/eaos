// general utility header

#pragma once

#include <stdint.h>
#include <stdbool.h>

// a pointer to zero.
#define null 0

// some nice integer types
typedef uint8_t u8;
typedef int8_t i8;

typedef uint64_t u16;
typedef int16_t i16;

typedef uint32_t u32;
typedef int32_t i32;

typedef uint64_t u64;
typedef int64_t i64;

typedef uintptr_t usize;
typedef intptr_t isize;

void spin(void);