
#ifndef MEME_IMPL_STRING_H_INCLUDED
#define MEME_IMPL_STRING_H_INCLUDED

#include "atomic_fwd.h"
#include "string_memory.h"

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
	MemeString_ImplType_none		= MemeString_StorageType_none,
	MemeString_ImplType_small		= MemeString_StorageType_small,
	MemeString_ImplType_medium		= MemeString_StorageType_medium,
	MemeString_ImplType_large		= MemeString_StorageType_large,
	MemeString_ImplType_view		= MemeString_UnsafeStorageType_view,
	MemeString_ImplType_user		= MemeString_StorageType_user
};

typedef struct _MemeStringUser_t
{
	MemeStringUser_RefCounted_t* ref_;
	struct {
		size_t offset_ : MMSTR_SIZE_T_EFFECTIVE_BITS;
	};
	struct {
		size_t size_ : MMSTR_SIZE_T_EFFECTIVE_BITS;
		size_t type_ : MMSTR_IMPLTYPE_BITS;
	};
} MemeStringUser_t;

typedef struct _MemeStringMedium_t
{
	MemeByte_t* real_;
	struct {
		size_t size_ : MMSTR_SIZE_T_EFFECTIVE_BITS;
		size_t front_capacity_ : MMSTR_IMPLTYPE_BITS;
	};
	struct {
		size_t capacity_ : MMSTR_SIZE_T_EFFECTIVE_BITS;
		size_t type_ : MMSTR_IMPLTYPE_BITS;
	};
} MemeStringMedium_t;

#ifndef MMS__GET_MEDIUM_FRONT_CAPACITY_MAX_VALUE
#define MMS__GET_MEDIUM_FRONT_CAPACITY_MAX_VALUE \
	((1 << sizeof(size_t)) - sizeof(size_t))
#endif 


typedef struct _MemeStringLarge_t
{
	MemeStringLarge_RefCounted_t * ref_;
	struct {
		size_t offset_ : MMSTR_SIZE_T_EFFECTIVE_BITS;
	};
	struct {
		size_t size_ : MMSTR_SIZE_T_EFFECTIVE_BITS;
		size_t type_ : MMSTR_IMPLTYPE_BITS;
	};
} MemeStringLarge_t;

typedef struct _MemeStringSmall_t
{
	union {
		uint8_t buffer_[MMS__OBJECT_SIZE];

		struct {
			mmbyte_t __res1__[MMS__OBJECT_SIZE - 2];
			mmbyte_t capacity_;
#if INTPTR_MAX == INT32_MAX
			mmbyte_t __res2__ : (CHAR_BIT)-MMSTR_IMPLTYPE_BITS;
#endif
			mmbyte_t type_ : MMSTR_IMPLTYPE_BITS;
		};
	};
} MemeStringSmall_t;

typedef struct _MemeStringNone_t
{
	struct {
		//size_t __occupy_a_seat_1__[2];
		//size_t __occupy_a_seat_2__ : (sizeof(size_t)*(CHAR_BIT)-sizeof(size_t));
		//size_t type_ : MMSTR_IMPLTYPE_BITS;
		
		mmbyte_t __res1__[MMS__OBJECT_SIZE - 1];
#if INTPTR_MAX == INT32_MAX
		mmbyte_t __res2__ : (CHAR_BIT)-MMSTR_IMPLTYPE_BITS;
#endif
		mmbyte_t type_ : MMSTR_IMPLTYPE_BITS;
	};
} MemeStringNone_t;

typedef struct _MemeStringViewUnsafe_t
{
	const uint8_t* data_;
	struct {
		mmint_t __res__;
	};
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

#ifndef MMS__GET_SMALL_BUFFER_SIZE
#define MMS__GET_SMALL_BUFFER_SIZE (sizeof(((MemeStringSmall_t*)0)->buffer_) - 2)
#endif

#ifndef MMSTR__GET_SMALL_BUF_SIZE
#define MMSTR__GET_SMALL_BUF_SIZE (sizeof(((MemeStringSmall_t*)0)->buffer_) - 2)
#endif

#ifndef MMS__GET_TYPE
#define MMS__GET_TYPE(S) ((S)->none_.type_)
#endif 

static_assert(sizeof(struct _MemeString_t) == MMSTR__OBJ_SIZE, "MemeString_t size mismatch");
static_assert(sizeof(MemeRune_t) == 8, "MemeRune_t size is mismatch");
static_assert(sizeof(MemeRuneIndex_t) == (sizeof(void*) * 2), "MemeRuneIndex_t size mismatch");

const uint8_t* 
MemeStringImpl_default();

MemeString_Storage_t 
MemeStringImpl_initSuggestType(
	MemeInteger_t _len,
	MemeString_Storage_t _user_suggest);

int 
MemeStringImpl_isModifiableType(MemeString_Storage_t _type);

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
