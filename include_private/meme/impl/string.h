
#ifndef MEME_IMPL_STRING_H_INCLUDED
#define MEME_IMPL_STRING_H_INCLUDED

#include "atomic_fwd.h"
#include "string_memory.h"

#include <stdint.h>
#include <limits.h>

MEME_EXTERN_C_SCOPE_START

typedef struct _MemeStringLarge_RefCounted_t 
{
	MemeAtomicInteger_t count_;

	//! point to the real buffer; 
	//! In some cases, set aside a pre-buffer to avoid reallocation
	uint8_t * real_;	
	uint8_t * data_;	//< point to data buffer
	MemeString_MallocFunction_t* malloc_fn_;
	MemeString_FreeFunction_t* free_fn_;
} MemeStringLarge_RefCounted_t;

typedef struct _MemeStringUser_RefCounted_t
{
	MemeAtomicInteger_t count_;

	void* user_data_;
	MemeString_UserObjectDestruct_t* destruct_fn_;
	MemeString_UserObjectData_t* data_fn_;

	MemeString_MallocFunction_t* malloc_fn_;
	MemeString_FreeFunction_t* free_fn_;
} MemeStringUser_RefCounted_t;

typedef MemeInteger_t MemeString_ImplType_t;
enum _MemeString_ImplType_t {
	MemeString_ImplType_none		= 0,
	MemeString_ImplType_small		= MemeString_StorageType_small,
	MemeString_ImplType_medium		= MemeString_StorageType_medium,
	MemeString_ImplType_large		= MemeString_StorageType_large,
	MemeString_ImplType_view		= MemeString_UnsafeStorageType_view,
	MemeString_ImplType_user		= MemeString_StorageType_user
};

typedef struct _MemeString_Basic_t
{
	union {
		size_t size_;
	};
	union {
		size_t capacity_ : ((sizeof(size_t) - 1) * (CHAR_BIT));
		struct {
			uint8_t __occupy_a_seat2__[sizeof(size_t) - 1];
			uint8_t type_;
		};
	};
} MemeString_Basic_t;

typedef struct _MemeStringUser_t
{
	MemeStringUser_RefCounted_t* ref_;
	union {
		size_t size_;
	};
	union {
		size_t __reserve__ : ((sizeof(size_t) - 1)* (CHAR_BIT));
		struct {
			uint8_t __occupy_a_seat2__[sizeof(size_t) - 1];
			uint8_t type_;
		};
	};
} MemeStringUser_t;

typedef struct _MemeStringMedium_t
{
	uint8_t * data_;
	MemeString_Basic_t basic_;
} MemeStringMedium_t;

typedef struct _MemeStringLarge_t
{
	MemeStringLarge_RefCounted_t * ref_;
	MemeString_Basic_t basic_;
} MemeStringLarge_t;

typedef struct _MemeStringSmall_t
{
	union {
		uint8_t buffer_[sizeof(MemeStringMedium_t)];
		struct {
			uint8_t __occupy_a_seat1__[sizeof(MemeStringMedium_t) - 2];
			uint8_t capacity_;
			uint8_t type_;
		};
	};
} MemeStringSmall_t;

typedef struct _MemeStringViewUnsafe_t
{
	uint8_t* data_;
	size_t   size_;
	union {
		size_t __reserve__ : ((sizeof(size_t) - 1)* (CHAR_BIT));
		struct {
			uint8_t __occupy_a_seat__[sizeof(size_t) - 1];
			uint8_t type_;
		};
	};
} MemeStringViewUnsafe_t;

struct _MemeString_t
{
	union {
		MemeStringSmall_t  small_;
		MemeStringMedium_t medium_;
		MemeStringLarge_t  large_;
		MemeStringUser_t   user_;
		MemeStringViewUnsafe_t viewUnsafe_;
	};
};

const uint8_t* MemeStringImpl_default();
MemeString_Storage_t MemeStringImpl_initSuggestType(
	size_t _len,
	MemeString_Storage_t _user_suggest);

#ifndef MEME_STRING__GET_SMALL_BUFFER_SIZE
#define MEME_STRING__GET_SMALL_BUFFER_SIZE (sizeof(((MemeStringSmall_t*)0)->buffer_) - 2)
#endif

#ifndef MEME_STRING__GET_TYPE
#define MEME_STRING__GET_TYPE(S) ((S).small_.type_)
#endif 

MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_IMPL_STRING_H_INCLUDED
