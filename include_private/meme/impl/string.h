
#ifndef MEME_IMPL_STRING_H_INCLUDED
#define MEME_IMPL_STRING_H_INCLUDED

#include "atomic_fwd.h"
#include "string_memory.h"

#include <stdint.h>
#include <limits.h>

#if INTPTR_MAX == INT16_MAX
#error "16-bit system not supported"
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
//
//typedef struct _MemeString_Basic_t
//{
//	union {
//		MemeInteger_t size_;
//	};
//	union {
//		size_t capacity_ : ((sizeof(size_t) - 1) * (CHAR_BIT));
//		struct {
//			uint8_t __occupy_a_seat__[sizeof(size_t) - 1];
//			uint8_t type_;
//		};
//	};
//} MemeString_Basic_t;

typedef struct _MemeStringUser_t
{
	MemeStringUser_RefCounted_t* ref_;
	struct {
		MemeInteger_t offset_ : (sizeof(size_t)*(CHAR_BIT)-sizeof(size_t));
	};
	struct {
		size_t size_ : (sizeof(size_t)* (CHAR_BIT)-sizeof(size_t));
		size_t type_ : (sizeof(size_t));
	};
} MemeStringUser_t;

typedef struct _MemeStringMedium_t
{
	MemeByte_t* real_;
	struct {
		size_t size_ : (sizeof(size_t)*(CHAR_BIT)-sizeof(size_t));
		size_t front_capacity_ : sizeof(size_t);
	};
	struct {
		size_t capacity_ : (sizeof(size_t)*(CHAR_BIT)-sizeof(size_t));
		size_t type_ : (sizeof(size_t));
	};
} MemeStringMedium_t;

#ifndef MEME_STRING__GET_MEDIUM_FRONT_CAPACITY_MAX_SIZE
#define MEME_STRING__GET_MEDIUM_FRONT_CAPACITY_MAX_SIZE \
	((1 << sizeof(size_t)) - sizeof(size_t))
#endif 


typedef struct _MemeStringLarge_t
{
	MemeStringLarge_RefCounted_t * ref_;
	struct {
		MemeInteger_t offset_ : (sizeof(size_t)* (CHAR_BIT)-sizeof(size_t));
	};
	struct {
		size_t size_ : (sizeof(size_t)* (CHAR_BIT)-sizeof(size_t));
		size_t type_ : (sizeof(size_t));
	};
} MemeStringLarge_t;

#if INTPTR_MAX == INT32_MAX
typedef struct _MemeStringSmall_t
{
	union {
		uint8_t buffer_[MEME_STRING__OBJECT_SIZE];

		struct {
			size_t __occupy_a_seat_1__[2];
			size_t __occupy_a_seat_2__ : ((sizeof(size_t) - 2)* (CHAR_BIT));
			size_t capacity_ : (CHAR_BIT);
			size_t __occupy_a_seat_3__ : (sizeof(size_t));
			size_t type_ : (sizeof(size_t));
		};
	};
} MemeStringSmall_t;
#else
typedef struct _MemeStringSmall_t
{
	union {
		uint8_t buffer_[MEME_STRING__OBJECT_SIZE];

		struct {
			size_t __occupy_a_seat_1__[2];
			size_t __occupy_a_seat_2__ : ((sizeof(size_t)-1)*(CHAR_BIT)-sizeof(size_t));
			size_t capacity_ : (CHAR_BIT);
			size_t type_ : (sizeof(size_t));
		};
	};
} MemeStringSmall_t;
#endif

typedef struct _MemeStringNone_t
{
	struct {
		size_t __occupy_a_seat_1__[2];
		size_t __occupy_a_seat_2__ : (sizeof(size_t)*(CHAR_BIT)-sizeof(size_t));
		size_t type_ : (sizeof(size_t));
	};
} MemeStringNone_t;

typedef struct _MemeStringViewUnsafe_t
{
	const uint8_t* data_;
	struct {
		MemeInteger_t _res_ : (sizeof(size_t)* (CHAR_BIT)-sizeof(size_t));
	};
	struct {
		size_t size_ : (sizeof(size_t)* (CHAR_BIT)-sizeof(size_t));
		size_t type_ : (sizeof(size_t));
	};
} MemeStringViewUnsafe_t;

struct _MemeString_t
{
	union {
		MemeStringNone_t none_;
		MemeStringSmall_t  small_;
		MemeStringMedium_t medium_;
		MemeStringLarge_t  large_;
		MemeStringUser_t   user_;
		MemeStringViewUnsafe_t viewUnsafe_;
	};
};

#ifndef MEME_STRING__GET_SMALL_BUFFER_SIZE
#define MEME_STRING__GET_SMALL_BUFFER_SIZE (sizeof(((MemeStringSmall_t*)0)->buffer_) - 2)
#endif

#ifndef MEME_STRING__GET_TYPE
#define MEME_STRING__GET_TYPE(S) ((S)->none_.type_)
#endif 


const uint8_t* 
MemeStringImpl_default();

MemeString_Storage_t 
MemeStringImpl_initSuggestType(
	MemeInteger_t _len,
	MemeString_Storage_t _user_suggest);

int 
MemeStringImpl_isModifiableType(MemeString_Storage_t _type);

int
MemeStringImpl_dumpToModifiable(const MemeStringStack_t* _s, MemeStringStack_t* _out);

int
MemeStringImpl_capacityExpansionSmallToMedium(MemeStringStack_t* _s, MemeInteger_t _minSizeRequest);

int
MemeStringImpl_capacityExpansionWithModifiable(MemeStringStack_t* _s, MemeInteger_t _minSizeRequest);

void
MemeStringImpl_setDataOffset(MemeStringStack_t* _s, MemeInteger_t _offset);

void
MemeStringImpl_shrinkTailZero(MemeStringStack_t* _s);

MemeByte_t* MemeStringImpl_forcedData(MemeStringStack_t* _s);



MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_IMPL_STRING_H_INCLUDED
