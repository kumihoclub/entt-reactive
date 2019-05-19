#pragma once

#include <numeric>
#include <cstdio>
#include <exception>

#include "stdint.h"

#define UNIT_PIXELS 32.0f

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float  f32;
typedef double f64;

typedef uintptr_t uptr;
typedef intptr_t iptr;

typedef i8  b8;
typedef i16 b16;
typedef i32 b32;

#define U8_MIN UINT8_MIN
#define U8_MAX UINT8_MAX
#define I8_MIN INT8_MIN
#define I8_MAX INT8_MAX

#define U16_MIN UINT16_MIN
#define U16_MAX UINT16_MAX
#define I16_MIN INT16_MIN
#define I16_MAX INT16_MAX

#define U32_MIN UINT32_MIN
#define U32_MAX UINT32_MAX
#define I32_MIN INT32_MIN
#define I32_MAX INT32_MAX

#define U64_MIN UINT64_MIN
#define U64_MAX UINT64_MAX
#define I64_MIN INT64_MIN
#define I64_MAX INT64_MAX

#define F32_MIN std::numeric_limits<float>::min()
#define F32_MAX std::numeric_limits<float>::max()

#define F64_MIN std::numeric_limits<double>::min()
#define F64_MAX std::numeric_limits<double>::max()

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
