
#ifndef MEME_STRING_FWD_H_INCLUDED
#define MEME_STRING_FWD_H_INCLUDED

#include "common.h"

#include <stdint.h>
#include <stddef.h>
#include <limits.h>

MEME_EXTERN_C_SCOPE_START

typedef uint8_t  MemeByte_t;
typedef uint8_t  mmbyte_t;

#if   INTPTR_MAX == INT32_MAX
typedef int32_t MemeInteger_t;
typedef int32_t mmint_t;
#elif INTPTR_MAX == INT64_MAX
typedef int64_t MemeInteger_t;
typedef int64_t mmint_t;
#else
# error "Environment not 32, or 64-bit."
#endif

typedef struct _MemeString_t* MemeString_t;
typedef const struct _MemeString_t* MemeString_Const_t;

typedef struct _MemeString_t* mms_t;
typedef const struct _MemeString_t* mms_const_t;
typedef struct _MemeString_t* mmstr_t;
typedef const struct _MemeString_t* mmstr_const_t;
typedef struct _MemeString_t* mmstr_ptr_t;
typedef const struct _MemeString_t* mmstr_cptr_t;

typedef struct _MemeRuneIndex_t 
{
    const MemeByte_t * data;
	int8_t size;
    uint8_t __res__[sizeof(mmint_t) - sizeof(uint8_t)];
} MemeRuneIndex_t;
typedef MemeRuneIndex_t mmrune_idx_t;

typedef struct _MemeRune_t {
	MemeByte_t byte[7];
	struct {
		MemeByte_t capacity : (3);
		MemeByte_t invalid  : (CHAR_BIT - 3);
	} attr;
} MemeRune_t;
typedef MemeRune_t mmrune_t;

typedef MemeInteger_t MemeFlag_CaseSensitivity_t;
enum _MemeFlag_CaseSensitivity_t 
{
	MemeFlag_AllInsensitive = 0,
    MemeFlag_CaseSensitive  = 1,
	MemeFlag_SimplifiedTraditionalSensitive = 2,
	MemeFlag_AllSensitive	= 0xFFFFFFFF
};

typedef MemeFlag_CaseSensitivity_t mmflag_case_sensit_t;

typedef MemeInteger_t MemeString_Storage_t;
typedef MemeString_Storage_t mms_storage_t;
enum _MemeString_Storage_t {
	MemeString_StorageType_none			= 0,
	MemeString_StorageType_small		= 1,
	MemeString_StorageType_medium		= 2,
	MemeString_StorageType_large		= 3,
	MemeString_StorageType_user			= 10
};
enum _MemeString_UnsafeStorage_t {
	MemeString_UnsafeStorageType_view   = 9
};

typedef MemeString_Storage_t mmstr_strg_t;
enum _mmstr_strg_t {
    mmstr_strg_none    = MemeString_StorageType_none,
    mmstr_strg_small   = MemeString_StorageType_small,
    mmstr_strg_medium  = MemeString_StorageType_medium,
    mmstr_strg_large   = MemeString_StorageType_large,
    mmstr_strg_user    = MemeString_StorageType_user
};
enum _mmstr_usfstrg_t {
    mmstr_usfstrg_view = MemeString_UnsafeStorageType_view
};

typedef MemeInteger_t MemeFlag_SplitBehavior_t;
enum _MemeFlag_SplitBehavior_t 
{
	MemeFlag_KeepEmptyParts,
	MemeFlag_SkipEmptyParts
};

typedef MemeFlag_SplitBehavior_t mmflag_split_behav_t;

typedef mmint_t MemeFlag_CallbackProcess_t;
typedef MemeFlag_CallbackProcess_t mmflag_cbproc_t;
enum _MemeFlag_CallbackProcess_t
{
    MemeFlag_CallbackProcess_Continue,
    MemeFlag_CallbackProcess_Break
};
enum _mmflag_cbproc_t
{
    mmflag_cbproc_continue = MemeFlag_CallbackProcess_Continue,
    mmflag_cbproc_break    = MemeFlag_CallbackProcess_Break
};

typedef int MemeString_MatchCondByteFunc_t(MemeByte_t _ch, void* _user_data);
typedef MemeString_MatchCondByteFunc_t mmstr_match_cond_byte_cb_t;

typedef int MemeString_MatchCondRuneFunc_t(const MemeRune_t* _ch, void* _user_data);
typedef MemeString_MatchCondRuneFunc_t mmstr_match_cond_rune_cb_t;

typedef int MemeString_MappingConvertFunc_t(MemeRune_t* _ch, void* _user_data);
typedef MemeString_MappingConvertFunc_t mmstr_mapping_conv_cb_t;

typedef mmflag_cbproc_t MemeString_ForEachRuneFunc_t(const mmrune_t* _ch, void* _user_data);
typedef MemeString_ForEachRuneFunc_t mmstr_foreach_rune_cb_t;

#ifdef MEME_STRING__OBJECT_SIZE
#undef MEME_STRING__OBJECT_SIZE
#endif
#define MEME_STRING__OBJECT_SIZE (sizeof(mmint_t) * 3)

#ifndef MMS__OBJECT_SIZE
#define MMS__OBJECT_SIZE MEME_STRING__OBJECT_SIZE
#endif

#ifndef MMSTR__OBJ_SIZE
#define MMSTR__OBJ_SIZE MEME_STRING__OBJECT_SIZE
#endif

typedef struct _MemeStringStack_t {
	MemeByte_t byte[MEME_STRING__OBJECT_SIZE];
} MemeStringStack_t;
typedef MemeStringStack_t mmsstk_t;
typedef MemeStringStack_t mmstrstk_t;


#ifndef MMINT_MAX
#define MMINT_MAX INTPTR_MAX
#endif

#ifndef MMINT_MIN
#define MMINT_MIN INTPTR_MIN
#endif

MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_STRING_FWD_H_INCLUDED
