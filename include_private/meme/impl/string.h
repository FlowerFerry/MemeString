
#ifndef MEME_IMPL_STRING_H_INCLUDED
#define MEME_IMPL_STRING_H_INCLUDED

#include "atomic_fwd.h"
#include "string_memory.h"

#include <mego/predef/symbol/inline.h>
#include <stdint.h>
#include <limits.h>
#include <assert.h>

#ifndef INTPTR_MAX
#error "INTPTR_MAX not defined"
#endif

#if INTPTR_MAX == INT16_MAX
#error "16-bit system not supported"
#endif

#if INTPTR_MAX == INT32_MAX
#  ifndef MMSTR_IMPLTYPE_BITS
#  define MMSTR_IMPLTYPE_BITS (4)
#  endif
#else
#  ifndef MMSTR_IMPLTYPE_BITS
#  define MMSTR_IMPLTYPE_BITS (CHAR_BIT)
#  endif
#endif

#ifndef MMSTR_SIZE_T_EFFECTIVE_BITS
#define MMSTR_SIZE_T_EFFECTIVE_BITS (sizeof(size_t) * CHAR_BIT - MMSTR_IMPLTYPE_BITS)
#endif

MEME_EXTERN_C_SCOPE_START

typedef struct _MemeStringLarge_RefCounted_t 
{
	volatile MemeAtomicInteger_t count_;
	
	//! point to the real buffer; 
	//! In some cases, set aside a pre-buffer to avoid reallocation
	MemeByte_t * real_;	

	//MemeString_MallocFunction_t* malloc_fn_;
	//MemeString_FreeFunction_t* free_fn_;
} MemeStringLarge_RefCounted_t;

typedef struct _MemeStringUser_RefCounted_t
{
	volatile MemeAtomicInteger_t count_;

	void* user_data_;
	MemeString_UserObjectDestruct_t* destruct_fn_;
	MemeString_UserObjectData_t* data_fn_;

	//MemeString_MallocFunction_t* malloc_fn_;
	//MemeString_FreeFunction_t* free_fn_;
} MemeStringUser_RefCounted_t;

typedef MemeInteger_t MemeString_ImplType_t;
enum _MemeString_ImplType_t {
	MemeString_ImplType_none		= 1,
	MemeString_ImplType_small		= 0,
	MemeString_ImplType_medium		= MemeString_StorageType_medium,
	MemeString_ImplType_large		= MemeString_StorageType_large,
	MemeString_ImplType_view		= MemeString_UnsafeStorageType_view,
	MemeString_ImplType_user		= MemeString_StorageType_user
};
typedef MemeString_ImplType_t mmstr_impltype_e;
enum {
    mmstr_impltype_none   = MemeString_ImplType_none,
    mmstr_impltype_small  = MemeString_ImplType_small,
    mmstr_impltype_medium = MemeString_ImplType_medium,
	mmstr_impltype_large  = MemeString_ImplType_large,
    mmstr_impltype_view   = MemeString_ImplType_view,
    mmstr_impltype_user   = MemeString_ImplType_user
};

typedef struct _MemeStringUser_t
{
	struct {
#if   INTPTR_MAX == INT32_MAX
		size_t reg_size_ : 4;
#elif INTPTR_MAX == INT64_MAX
        size_t reg_size_ : 4;
        size_t __reserved1__ : 4;
#endif
		size_t offset_ : MMSTR_SIZE_T_EFFECTIVE_BITS;
	};
	MemeStringUser_RefCounted_t* ref_;
	struct {
		size_t size_ : MMSTR_SIZE_T_EFFECTIVE_BITS;
		size_t type_ : MMSTR_IMPLTYPE_BITS;
	};
} MemeStringUser_t;

typedef struct _MemeStringMedium_t
{
	struct {
#if   INTPTR_MAX == INT32_MAX
		size_t reg_size_ : 4;
#elif INTPTR_MAX == INT64_MAX
        size_t reg_size_ : 4;
        size_t front_capacity_ : 4;
#endif
		size_t capacity_ : MMSTR_SIZE_T_EFFECTIVE_BITS;
	};
	MemeByte_t* real_;
	struct {
		size_t size_ : MMSTR_SIZE_T_EFFECTIVE_BITS;
		size_t type_ : MMSTR_IMPLTYPE_BITS;
	};
} MemeStringMedium_t;

//#ifndef MMS__GET_MEDIUM_FRONT_CAPACITY_MAX_VALUE
//#define MMS__GET_MEDIUM_FRONT_CAPACITY_MAX_VALUE ((1 << sizeof(size_t)) - sizeof(size_t))
//#endif 

#ifndef MMSTR__GET_MEDIUM_FRONT_CAP_MAX_SIZE
#if INTPTR_MAX == INT64_MAX
#define MMSTR__GET_MEDIUM_FRONT_CAP_MAX_SIZE (15)
#else
#define MMSTR__GET_MEDIUM_FRONT_CAP_MAX_SIZE ( 0)
#endif
#endif 

typedef struct _MemeStringLarge_t
{
	struct {
#if   INTPTR_MAX == INT32_MAX
		size_t reg_size_ : 4;
#elif INTPTR_MAX == INT64_MAX
		size_t reg_size_ : 4;
		size_t __reserved1__ : 4;
#endif
		size_t offset_ : MMSTR_SIZE_T_EFFECTIVE_BITS;
	};
	MemeStringLarge_RefCounted_t * ref_;
	struct {
		size_t size_ : MMSTR_SIZE_T_EFFECTIVE_BITS;
		size_t type_ : MMSTR_IMPLTYPE_BITS;
	};
} MemeStringLarge_t;

typedef struct _MemeStringSmall_t
{
	mmbyte_t reg_size_ : 4;
	union {
		uint8_t buffer_[MMSTR__OBJ_SIZE - 1];

		struct {
#if   INTPTR_MAX == INT32_MAX
			mmbyte_t __reserved1__[MMSTR__OBJ_SIZE - 2];
			mmbyte_t capacity_ : 4;
			mmbyte_t type_ : 4;
#elif INTPTR_MAX == INT64_MAX
			mmbyte_t __reserved1__[MMSTR__OBJ_SIZE - 3];
			mmbyte_t capacity_;
            mmbyte_t type_;
#endif
		};
	};
} MemeStringSmall_t;

typedef struct _MemeStringNone_t
{
	mmbyte_t reg_size_ : 4;
	struct {
		mmbyte_t __reserved1__[MMSTR__OBJ_SIZE - 2];
#if INTPTR_MAX == INT32_MAX
		mmbyte_t __reserved2__ : 4;
		mmbyte_t type_ : 4;
#elif INTPTR_MAX == INT64_MAX
		mmbyte_t type_;
#endif
	};
} MemeStringNone_t;

typedef struct _MemeStringViewUnsafe_t
{
	mmbyte_t reg_size_ : 4;
	struct {
		uint8_t __reserved1__[sizeof(void*) - 1];
	};
	const uint8_t* data_;
	struct {
		size_t size_ : MMSTR_SIZE_T_EFFECTIVE_BITS;
		size_t type_ : MMSTR_IMPLTYPE_BITS;
	};
} MemeStringViewUnsafe_t;

struct _MemeString_t
{
	union {
		MemeStringNone_t       none_;
		MemeStringSmall_t      small_;
		MemeStringMedium_t     medium_;
		MemeStringLarge_t      large_;
		MemeStringUser_t       user_;
		MemeStringViewUnsafe_t viewUnsafe_;
	};
};

//#ifndef MMS__GET_SMALL_BUFFER_SIZE
//#define MMS__GET_SMALL_BUFFER_SIZE (sizeof(MemeStringSmall_t) - 2)
//#endif

//#ifndef MMSTR__GET_SMALL_BUF_SIZE
//#define MMSTR__GET_SMALL_BUF_SIZE  (sizeof(MemeStringSmall_t) - 2)
//#endif

#ifndef MMSTR__GET_SMALL_BUF_MAX_SIZE
#if   INTPTR_MAX == INT32_MAX
#define MMSTR__GET_SMALL_BUF_MAX_SIZE (sizeof(MemeStringSmall_t) - 2)
#elif INTPTR_MAX == INT64_MAX
#define MMSTR__GET_SMALL_BUF_MAX_SIZE (sizeof(MemeStringSmall_t) - 3)
#endif
#endif

//#ifndef MMS__GET_TYPE
////! @deprecated
//#define MMS__GET_TYPE(S) ((S)->none_.type_)
//#endif 

#ifndef MMSTR__GET_IMPLTYPE
#define MMSTR__GET_IMPLTYPE(S) ((S)->none_.type_)
#endif 

static_assert(sizeof(struct _MemeString_t) == MMSTR__OBJ_SIZE, "MemeString_t size mismatch");
static_assert(sizeof(MemeRune_t) == 8, "MemeRune_t size is mismatch");
static_assert(sizeof(MemeRuneIndex_t) == (sizeof(void*) * 2), "MemeRuneIndex_t size mismatch");

const uint8_t* 
MemeStringImpl_default();

MG_CAPI_INLINE mmstr_impltype_e mmstrimpl_strgtype_to_impltype(mmstr_strg_e _strg)
{
	switch (_strg) {
	case mmstr_strg_none:
        return mmstr_impltype_none;
    case mmstr_strg_small:
        return mmstr_impltype_small;
	default:
		return (mmstr_impltype_e)_strg;
	}
}

MG_CAPI_INLINE mmstr_strg_e mmstrimpl_impltype_to_strgtype(mmstr_impltype_e _strg)
{
    switch (_strg) {
    case mmstr_impltype_none:
        return mmstr_strg_none;
    case mmstr_impltype_small:
        return mmstr_strg_small;
    default:
        return (mmstr_strg_e)_strg;
    }
}

MemeString_ImplType_t
MemeStringImpl_initSuggestType(
	MemeInteger_t _len,
	MemeString_ImplType_t _user_suggest);

int 
MemeStringImpl_isModifiableType(MemeString_ImplType_t _type);

MG_CAPI_INLINE int MemeStringImpl_objRegSize(mmstr_cptr_t _str)
{
	return _str->none_.reg_size_;
}

MG_CAPI_INLINE int MemeStringImpl_objByteSize(mmstr_cptr_t _str)
{
    return _str->none_.reg_size_ * sizeof(mmint_t);
}

//int
//MemeStringImpl_dumpToModifiable(const MemeStringStack_t* _s, MemeStringStack_t* _out);

//! @param _s must be initialized
int
MemeStringImpl_capacityExpansionSmallToMedium(MemeStringStack_t* _s, MemeInteger_t _minSizeRequest);

//! @param _s must be initialized
int
MemeStringImpl_capacityExpansionWithModifiable(MemeStringStack_t* _s, MemeInteger_t _minSizeRequest);

//! @param _s must be initialized
void
MemeStringImpl_setDataOffset(MemeStringStack_t* _s, MemeInteger_t _offset);

//! @param _s must be initialized
void
MemeStringImpl_shrinkTailZero(MemeStringStack_t* _s);

//! @param _s must be initialized
MemeByte_t* 
MemeStringImpl_forcedData(MemeStringStack_t* _s);



MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_IMPL_STRING_H_INCLUDED
