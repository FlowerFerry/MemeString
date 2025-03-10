
#ifndef MEME_CONC_ATOMIC_FWD_H_INCLUDED
#define MEME_CONC_ATOMIC_FWD_H_INCLUDED

#include <stdint.h>
#include <assert.h>

#include <meme/common.h>

MEME_EXTERN_C_SCOPE_START

typedef enum mmconc_atomic_data_type {
    mmconc_atomic_none_type,
    mmconc_atomic_bool_type,
    mmconc_atomic_int8_type,
    mmconc_atomic_uint8_type,
    mmconc_atomic_int16_type,
    mmconc_atomic_uint16_type,
    mmconc_atomic_int32_type,
    mmconc_atomic_uint32_type,
    mmconc_atomic_int64_type,
    mmconc_atomic_uint64_type,
    mmconc_atomic_flag_type
} mmconc_atomic_data_type_e;

#if INTPTR_MAX == INT64_MAX
enum {
    mmconc_atomic_intptr_type  = mmconc_atomic_int64_type,
    mmconc_atomic_uintptr_type = mmconc_atomic_uint64_type
};
enum {
    mmconc_atomic_size_type = mmconc_atomic_uint64_type,
};
#elif INTPTR_MAX == INT32_MAX
enum {
    mmconc_atomic_intptr_type  = mmconc_atomic_int32_type,
    mmconc_atomic_uintptr_type = mmconc_atomic_uint32_type
};
enum {
    mmconc_atomic_size_type = mmconc_atomic_uint32_type,
};
#else
#  error "Unsupported"
#endif

typedef enum mmconc_memory_order {
    mmconc_memory_order_relaxed = 0,
    mmconc_memory_order_consume = 1,
    mmconc_memory_order_acquire = 2,
    mmconc_memory_order_release = 3,
    mmconc_memory_order_acq_rel = 4,
    mmconc_memory_order_seq_cst = 5
} mmconc_memory_order_e;

#define MMCONC_ATOMIC_BOOL_OBJ_SIZE   4
#define MMCONC_ATOMIC_INT8_OBJ_SIZE   4
#define MMCONC_ATOMIC_UINT8_OBJ_SIZE  4
#define MMCONC_ATOMIC_INT16_OBJ_SIZE  6
#define MMCONC_ATOMIC_UINT16_OBJ_SIZE 6
#define MMCONC_ATOMIC_INT32_OBJ_SIZE  8
#define MMCONC_ATOMIC_UINT32_OBJ_SIZE 8
#define MMCONC_ATOMIC_INT64_OBJ_SIZE  16
#define MMCONC_ATOMIC_UINT64_OBJ_SIZE 16

#if INTPTR_MAX == INT64_MAX
#  define MMCONC_ATOMIC_INTPTR_OBJ_SIZE  MMCONC_ATOMIC_INT64_OBJ_SIZE
#  define MMCONC_ATOMIC_UINTPTR_OBJ_SIZE MMCONC_ATOMIC_UINT64_OBJ_SIZE
#  define MMCONC_ATOMIC_SIZE_OBJ_SIZE    MMCONC_ATOMIC_UINT64_OBJ_SIZE
#elif INTPTR_MAX == INT32_MAX
#  define MMCONC_ATOMIC_INTPTR_OBJ_SIZE  MMCONC_ATOMIC_INT32_OBJ_SIZE
#  define MMCONC_ATOMIC_UINTPTR_OBJ_SIZE MMCONC_ATOMIC_UINT32_OBJ_SIZE
#  define MMCONC_ATOMIC_SIZE_OBJ_SIZE    MMCONC_ATOMIC_UINT32_OBJ_SIZE
#else
#  error "Unsupported"
#endif

typedef struct mmconc_atomic_none {
    uint8_t st_size;
    uint8_t type;
} mmconc_atomic_none_t;

typedef struct mmconc_atomic_bool {
    mmconc_atomic_none_t base;
    uint8_t byte[MMCONC_ATOMIC_BOOL_OBJ_SIZE - sizeof(mmconc_atomic_none_t)];
} mmconc_atomic_bool_t;

typedef struct mmconc_atomic_int8 {
    mmconc_atomic_none_t base;
    uint8_t byte[MMCONC_ATOMIC_INT8_OBJ_SIZE - sizeof(mmconc_atomic_none_t)];
} mmconc_atomic_int8_t;

typedef struct mmconc_atomic_uint8 {
    mmconc_atomic_none_t base;
    uint8_t byte[MMCONC_ATOMIC_UINT8_OBJ_SIZE - sizeof(mmconc_atomic_none_t)];
} mmconc_atomic_uint8_t;

typedef struct mmconc_atomic_int16 {
    mmconc_atomic_none_t base;
    uint8_t byte[MMCONC_ATOMIC_INT16_OBJ_SIZE - sizeof(mmconc_atomic_none_t)];
} mmconc_atomic_int16_t;

typedef struct mmconc_atomic_uint16 {
    mmconc_atomic_none_t base;
    uint8_t byte[MMCONC_ATOMIC_UINT16_OBJ_SIZE - sizeof(mmconc_atomic_none_t)];
} mmconc_atomic_uint16_t;

typedef struct mmconc_atomic_int32 {
    mmconc_atomic_none_t base;
    uint8_t byte[MMCONC_ATOMIC_INT32_OBJ_SIZE - sizeof(mmconc_atomic_none_t)];
} mmconc_atomic_int32_t;

typedef struct mmconc_atomic_uint32 {
    mmconc_atomic_none_t base;
    uint8_t byte[MMCONC_ATOMIC_UINT32_OBJ_SIZE - sizeof(mmconc_atomic_none_t)];
} mmconc_atomic_uint32_t;

typedef struct mmconc_atomic_int64 {
    mmconc_atomic_none_t base;
    uint8_t byte[MMCONC_ATOMIC_INT64_OBJ_SIZE - sizeof(mmconc_atomic_none_t)];
} mmconc_atomic_int64_t;

typedef struct mmconc_atomic_uint64 {
    mmconc_atomic_none_t base;
    uint8_t byte[MMCONC_ATOMIC_UINT64_OBJ_SIZE - sizeof(mmconc_atomic_none_t)];
} mmconc_atomic_uint64_t;

typedef struct mmconc_atomic_flag {
    mmconc_atomic_none_t base;
    uint8_t byte[MMCONC_ATOMIC_BOOL_OBJ_SIZE - sizeof(mmconc_atomic_none_t)];
} mmconc_atomic_flag_t;

static_assert(sizeof(mmconc_atomic_bool_t)   == MMCONC_ATOMIC_BOOL_OBJ_SIZE,   "mmconc_atomic_bool_t size must be 4 bytes");
static_assert(sizeof(mmconc_atomic_int8_t)   == MMCONC_ATOMIC_INT8_OBJ_SIZE,   "mmconc_atomic_int8_t size must be 4 bytes");
static_assert(sizeof(mmconc_atomic_uint8_t)  == MMCONC_ATOMIC_UINT8_OBJ_SIZE,  "mmconc_atomic_uint8_t size must be 4 bytes");
static_assert(sizeof(mmconc_atomic_int16_t)  == MMCONC_ATOMIC_INT16_OBJ_SIZE,  "mmconc_atomic_int16_t size must be 6 bytes");
static_assert(sizeof(mmconc_atomic_uint16_t) == MMCONC_ATOMIC_UINT16_OBJ_SIZE, "mmconc_atomic_uint16_t size must be 6 bytes");
static_assert(sizeof(mmconc_atomic_int32_t)  == MMCONC_ATOMIC_INT32_OBJ_SIZE,  "mmconc_atomic_int32_t size must be 8 bytes");
static_assert(sizeof(mmconc_atomic_uint32_t) == MMCONC_ATOMIC_UINT32_OBJ_SIZE, "mmconc_atomic_uint32_t size must be 8 bytes");
static_assert(sizeof(mmconc_atomic_int64_t)  == MMCONC_ATOMIC_INT64_OBJ_SIZE,  "mmconc_atomic_int64_t size must be 16 bytes");
static_assert(sizeof(mmconc_atomic_uint64_t) == MMCONC_ATOMIC_UINT64_OBJ_SIZE, "mmconc_atomic_uint64_t size must be 16 bytes");


#if INTPTR_MAX == INT64_MAX
typedef mmconc_atomic_int64_t  mmconc_atomic_intptr_t;
typedef mmconc_atomic_uint64_t mmconc_atomic_uintptr_t;
typedef mmconc_atomic_uint64_t mmconc_atomic_size_t;
#elif INTPTR_MAX == INT32_MAX
typedef mmconc_atomic_int32_t  mmconc_atomic_intptr_t;
typedef mmconc_atomic_uint32_t mmconc_atomic_uintptr_t;
typedef mmconc_atomic_uint32_t mmconc_atomic_size_t;
#else
#  error "Unsupported"
#endif

MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_CONC_ATOMIC_FWD_H_INCLUDED
