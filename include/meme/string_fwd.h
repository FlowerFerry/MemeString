
#ifndef MEME_STRING_FWD_H_INCLUDED
#define MEME_STRING_FWD_H_INCLUDED

#include "common.h"

#include <stdint.h>
#include <stddef.h>

MEME_EXTERN_C_SCOPE_START

typedef uint8_t  MemeByte_t;
typedef intptr_t MemeInteger_t;

typedef uint8_t  mmbyte_t;
typedef intptr_t mmint_t;

typedef struct _MemeString_t* MemeString_t;
typedef const struct _MemeString_t* MemeString_Const_t;

typedef struct _MemeString_t* mms_t;
typedef const struct _MemeString_t* mms_const_t;
typedef struct _MemeString_t* mmstr_t;
typedef const struct _MemeString_t* mmstr_const_t;

typedef struct _MemeRuneIndex_t {
	MemeInteger_t size;
    const MemeByte_t * data;
} MemeRuneIndex_t;
typedef MemeRuneIndex_t mmrune_index_t;

typedef struct _MemeRune_t {
	MemeByte_t byte[7];
	struct {
		MemeByte_t capacity : 3;
		MemeByte_t invalid  : 5;
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

typedef int MemeString_MatchCondByteFunc_t(MemeByte_t _ch, void* _user_data);

typedef int MemeString_MatchCondRuneFunc_t(const MemeRune_t* _ch, void* _user_data);

typedef int MemeString_MappingConvertFunc_t(MemeRune_t* _ch, void* _user_data);

#ifdef MEME_STRING__OBJECT_SIZE
#undef MEME_STRING__OBJECT_SIZE
#endif
#define MEME_STRING__OBJECT_SIZE (sizeof(intptr_t) * 3)

#ifndef MMS__OBJECT_SIZE
#define MMS__OBJECT_SIZE MEME_STRING__OBJECT_SIZE
#endif

typedef struct _MemeStringStack_t {
	MemeByte_t byte[MEME_STRING__OBJECT_SIZE];
} MemeStringStack_t;
typedef MemeStringStack_t mmsstk_t;
typedef MemeStringStack_t mmstrstk_t;

//#ifndef MMENO__OFFSET
//#define MMENO__OFFSET (-100000)
//#endif

//#ifndef MMENO__POSIX_OFFSET
//#define MMENO__POSIX_OFFSET(VALUE) (-100000-(VALUE))
//#endif

MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_STRING_FWD_H_INCLUDED
