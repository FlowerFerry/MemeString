
#ifndef MEME_STRING_H_INCLUDED
#define MEME_STRING_H_INCLUDED

#include "meme/common.h"
#include "meme/string_fwd.h"
#include "meme/string_memory_fwd.h"
#include "meme/buffer_fwd.h"
#include <mego/err/ec.h>
#include <mego/predef/symbol/compiler/gcc/format.h>
#include <mego/predef/symbol/compiler/msvc/format.h>
#include <mego/predef/symbol/restrict.h>
#include <mego/predef/symbol/inline.h>
#include <mego/predef/symbol/likely.h>
#include <mego/predef/symbol/deprecated.h>

#include <stdarg.h>
#include <assert.h>

MEME_EXTERN_C_SCOPE_START

/**
 * @brief Set the byte threshold that separates "medium" (stack-allocated) strings
 *        from "large" (heap-allocated, reference-counted) strings.
 *
 * The MemeString implementation uses three internal storage classes:
 *  - **small**  — inline buffer, no allocation; capacity ≤ MMSTR__GET_SMALL_BUF_MAX_SIZE.
 *  - **medium** — stack-allocated (@c MemeStringStack_t); capacity is between the small
 *                 limit and the medium limit set here.
 *  - **large**  — heap-allocated, reference-counted (@c MemeStringLarge_RefCounted_t);
 *                 capacity exceeds the medium limit.
 *
 * Setting this limit controls the crossover point at which newly created or mutated
 * strings graduate from the medium tier to the large tier.
 *
 * @param _value  The new upper bound (in bytes, inclusive) for the medium storage class.
 *                - If @p _value is negative, the function rejects it and returns
 *                  @c MGEC__INVAL without modifying the limit.
 *                - If @p _value is less than or equal to the small-buffer maximum
 *                  (MMSTR__GET_SMALL_BUF_MAX_SIZE), the medium tier is effectively
 *                  disabled: the limit is forced to 0, meaning all strings that
 *                  outgrow the small buffer go directly to the large tier.
 *                - Otherwise, the limit is stored as-is.
 *
 * @return  0 on success, @c MGEC__INVAL if @p _value is negative.
 *
 * @note This function is **not thread-safe**.  Call it once during program
 *       initialization before any MemeString objects are created.
 * @note The default limit is calculated as:
 *       @code sizeof(mmint_t) * 16 + sizeof(MemeStringStack_t)
 *             + sizeof(MemeStringLarge_RefCounted_t) * 3 @endcode
 *
 * @see MemeStringOption_getStorageMediumLimit()
 * @see MemeStringOption_getStorageSmallLimit()
 */
MEME_API int
	MEME_STDCALL MemeStringOption_setStorageMediumLimit(MemeInteger_t _value);

/**
 * @brief Retrieve the current byte threshold for the "medium" storage tier.
 *
 * Returns the value previously set by MemeStringOption_setStorageMediumLimit().
 * A return value of 0 means the medium tier is disabled — strings that cannot
 * fit in the small inline buffer are stored directly in the large
 * (heap-allocated, reference-counted) tier.
 *
 * @return  The current medium-storage limit in bytes, or 0 if the medium tier
 *          is disabled.
 *
 * @see MemeStringOption_setStorageMediumLimit()
 * @see MemeStringOption_getStorageSmallLimit()
 */
MEME_API MemeInteger_t
	MEME_STDCALL MemeStringOption_getStorageMediumLimit();

/**
 * @brief Retrieve the fixed byte limit for the "small" (inline) storage tier.
 *
 * The small storage tier stores the string content directly inside the
 * @c MemeStringSmall_t struct — no heap allocation is performed.  Its capacity
 * is a compile-time constant defined as @c (sizeof(MemeStringSmall_t) - 2) or
 * @c (sizeof(MemeStringSmall_t) - 3) depending on the platform/configuration.
 *
 * Unlike the medium limit (which is runtime-configurable), this value is
 * determined entirely at compile time and cannot be changed at runtime.
 * Strings whose byte length exceeds this threshold are promoted to the medium
 * or large tier.
 *
 * @return  The maximum number of bytes (excluding the null terminator) that can
 *          be stored in the small inline buffer.
 *
 * @see MemeStringOption_getStorageMediumLimit()
 * @see MemeStringOption_setStorageMediumLimit()
 */
MEME_API mmint_t
MEME_STDCALL MemeStringOption_getStorageSmallLimit();

/**
 * @brief Initialize a MemeStringStack object to the empty state.
 *
 * Sets up @p _out as an empty string using the **small** (inline) storage class.
 * No heap allocation is performed.  The object must subsequently be destroyed
 * with MemeStringStack_unInit() when it is no longer needed.
 *
 * @param[out] _out         Pointer to the stack object to initialize.
 *                          Must not be @c NULL.
 * @param[in]  _object_size Byte size of the caller's @c mmsstk_t instance
 *                          (typically @c sizeof(mmsstk_t) or
 *                          @c MMSTR__OBJ_SIZE).
 *                          Must be greater than 0 and must not exceed
 *                          @c MMSTR__OBJ_SIZE.  If it exceeds
 *                          @c MMSTR__OBJ_SIZE the function returns
 *                          @c MGEC__OPNOTSUPP without touching @p _out.
 *
 * @return  0 on success, @c MGEC__OPNOTSUPP if @p _object_size is out of range.
 *
 * @note This function does **not** allocate any memory.
 * @note Pair every successful call with MemeStringStack_unInit() or
 *       MemeStringStack_reset() when done.
 *
 * @see MemeStringStack_unInit()
 * @see MemeStringStack_reset()
 */
MEME_API int
	MEME_STDCALL MemeStringStack_init(mmsstk_t* _out, size_t _object_size);

//! When the size of the external object differs from the size of the internal object,
//! initialization based on the size of the external object is not allowed, as it does not conform to the design.
//! @deprecated Please use MemeStringStack_init instead.
MEME_API mmsstk_t
MEGO_SYMBOL__DEPRECATED
	MEME_STDCALL MemeStringStack_getInitObject(size_t _object_size);

/**
 * @brief Initialize a MemeStringStack by taking ownership of a user-managed object.
 *
 * This function transfers ownership of a user-allocated data object into a
 * MemeStringStack.  Two code paths are taken depending on the data length
 * reported by @p _size_fn:
 *
 *  - **Fits in the small buffer** (≤ @c MMSTR__GET_SMALL_BUF_MAX_SIZE bytes):
 *    The raw bytes are copied from the user object via @p _data_fn into the
 *    inline small storage, then @p _destruct_fn is called immediately to
 *    release the original user object.
 *
 *  - **Larger than the small buffer**: The user object is wrapped inside a
 *    *user* storage node (@c MemeStringUser_t).  @p _destruct_fn will be
 *    called automatically when the last reference to the string is dropped
 *    (i.e. during MemeStringStack_unInit()).
 *
 * @param[out] _out          Pointer to the stack object to initialize.
 *                           Must not be @c NULL.
 * @param[in]  _object_size  Byte size of the caller's @c mmsstk_t instance.
 * @param[in]  _user_data    Opaque pointer to the user-managed data object.
 *                           Must not be @c NULL.
 * @param[in]  _destruct_fn  Callback invoked to destroy @p _user_data once
 *                           it is no longer needed.  Must not be @c NULL.
 * @param[in]  _data_fn      Callback that returns a pointer to the raw byte
 *                           content of @p _user_data.  Must not be @c NULL.
 * @param[in]  _size_fn      Callback that returns the byte length of the
 *                           content in @p _user_data.  Must not be @c NULL.
 *
 * @return  0 on success, or a non-zero error code on failure.
 *          If the small-buffer copy fails, the error code from
 *          MemeStringStack_initByU8bytes() is forwarded.
 *
 * @note After a successful call the caller must not touch @p _user_data
 *       through any pointer it still holds; ownership has been transferred.
 * @note All four pointer parameters (@p _user_data, @p _destruct_fn,
 *       @p _data_fn, @p _size_fn) must be non-@c NULL; passing @c NULL for
 *       any of them results in an immediate failure with return code @c MGEC__INVAL.
 *
 * @see MemeStringStack_unInit()
 */
MEME_API int
	MEME_STDCALL MemeStringStack_initTakeOverUserObject(
		mmsstk_t* _out, size_t _object_size,
		void* _user_data,
		MemeString_UserObjectDestruct_t* _destruct_fn,
		MemeString_UserObjectData_t* _data_fn,
		MemeString_UserObjectSize_t* _size_fn);

/**
 * @brief Initialize a MemeStringStack as a copy of another MemeString.
 *
 * Performs a type-aware copy of @p _other into @p _out.  The exact strategy
 * used depends on the internal storage class of @p _other:
 *
 *  - **small**  — A plain @c memcpy of the fixed-size object is performed;
 *                 no allocation.
 *  - **medium** — A new medium buffer is allocated and the byte content is
 *                 appended into it.
 *  - **large**  — The large (reference-counted) node's reference count is
 *                 incremented; no data copy.
 *  - **user**   — The user node's reference count is incremented; no data copy.
 *  - **view**   — The raw bytes are copied into a new small/medium/large
 *                 string via MemeStringStack_initByU8bytes().
 *
 * @param[out] _out         Pointer to the stack object to initialize.
 *                          Must not be @c NULL.
 * @param[in]  _object_size Byte size of the caller's @c mmsstk_t instance.
 * @param[in]  _other       Source string to copy from.
 *                          If @c NULL, @p _out is initialized to an empty
 *                          string (equivalent to MemeStringStack_init()).
 *
 * @return  0 on success, @c MGEC__CANCELED if @p _out and @p _other point
 *          to the same object, or a non-zero error code on allocation failure.
 *
 * @note @p _out and @p _other must not alias each other; doing so returns
 *       @c MGEC__CANCELED.
 *
 * @see MemeStringStack_unInit()
 * @see MemeStringStack_assign()
 */
MEME_API int
	MEME_STDCALL MemeStringStack_initByOther(
		mmstrstk_t* _out, size_t _object_size, mmstr_cptr_t _other);

//MEME_API mgec_t
//	MEME_STDCALL MemeStringStack_initByOtherStack(
//		mmstrstk_t* _out, size_t _object_size, const mmstrstk_t* _other);

/**
 * @brief Initialize a MemeStringStack from a slice of a MemeBuffer.
 *
 * Copies the byte content of @p _other starting at byte offset @p _offset
 * into @p _out, stripping any trailing null bytes (shrinking to the last
 * non-null byte).  The storage tier is chosen automatically based on the
 * resulting byte length:
 *
 *  - Length 0 → empty small string (MemeStringStack_init()).
 *  - Length ≤ @c MMSTR__GET_SMALL_BUF_MAX_SIZE → small inline storage.
 *  - Otherwise → medium or large storage, matching @p _other's storage type.
 *
 * @param[out] _out         Pointer to the stack object to initialize.
 *                          Must not be @c NULL.
 * @param[in]  _object_size Byte size of the caller's @c mmsstk_t instance.
 * @param[in]  _other       Source buffer to read from.
 *                          If @c NULL, @p _out is initialized to an empty string.
 * @param[in]  _offset      Byte offset into @p _other at which reading begins.
 *                          Negative values are treated as 0.
 *
 * @return  0 on success, @c MGEC__CANCELED if @p _out and @p _other alias
 *          the same memory, @c MGEC__OPNOTSUPP for unsupported buffer storage
 *          types, or a non-zero error code on allocation failure.
 *
 * @see MemeStringStack_unInit()
 * @see MemeBuffer_data()
 * @see MemeBuffer_size()
 */
MEME_API int
MEME_STDCALL MemeStringStack_initByBuffer(
	mmsstk_t* _out, size_t _object_size, MemeBuffer_Const_t _other, MemeInteger_t _offset);

/**
 * @brief Initialize a MemeStringStack from a Unicode rune.
 *
 * Converts the UTF-8 byte sequence of the rune @p _ch and uses it as the
 * string content.  
 *
 * @param[out] _out         Pointer to the stack object to initialize.
 *                          Must not be @c NULL.
 * @param[in]  _object_size Byte size of the caller's @c mmsstk_t instance.
 * @param[in]  _count       Intended repeat count.
 * @param[in]  _ch          The Unicode rune to encode as UTF-8.
 *                          Must be a valid rune (@c MemeRune_isValid() != 0).
 *
 * @return  0 on success, @c MGEC__INVAL if @p _ch is not a valid Unicode rune,
 *          or a non-zero error code on allocation failure.
 *
 * @see MemeRune_isValid()
 * @see MemeStringStack_initByU8bytes()
 * @see MemeStringStack_unInit()
 */
MEME_API int
	MEME_STDCALL MemeStringStack_initByRune(
		mmsstk_t* _out, size_t _object_size, mmint_t _count, MemeRune_t _ch);

/**
 * @brief Initialize a MemeStringStack from a UTF-8 byte sequence.
 *
 * Copies @p _len bytes of UTF-8 data from @p _utf8 into @p _out, choosing
 * the storage tier automatically (small / medium / large) based on the byte
 * length.  This is equivalent to calling MemeStringStack_initByU8bytesAndType()
 * with @c MemeString_StorageType_none as the storage hint.
 *
 * @param[out] _out         Pointer to the stack object to initialize.
 *                          Must not be @c NULL.
 * @param[in]  _object_size Byte size of the caller's @c mmsstk_t instance.
 * @param[in]  _utf8        Pointer to the UTF-8 encoded byte data.
 *                          If @c NULL, @p _out is initialized to an empty string.
 * @param[in]  _len         Number of bytes in @p _utf8 to use.
 *                          Pass a negative value to have the length computed
 *                          automatically via @c strlen().
 *
 * @return  0 on success, or a non-zero error code on allocation failure.
 *
 * @note The input bytes are not validated as well-formed UTF-8.  If you need
 *       guaranteed valid UTF-8 output, use MemeStringStack_toValidUtf8_v2()
 *       on the result.
 *
 * @see MemeStringStack_initByU8bytesAndType()
 * @see MemeStringStack_unInit()
 */
MEME_API int
	MEME_STDCALL MemeStringStack_initByU8bytes(
		mmsstk_t* _out, size_t _object_size, const MemeByte_t* _utf8, MemeInteger_t _len);

/**
 * @brief Initialize a MemeStringStack from a UTF-8 byte sequence with a
 *        storage-tier hint.
 *
 * Copies @p _len bytes of UTF-8 data from @p _utf8 into @p _out.  The
 * @p _suggest parameter provides a preferred storage tier; the implementation
 * may override it to satisfy minimum size requirements:
 *
 *  - @c MemeString_StorageType_none — tier is chosen automatically.
 *  - @c MemeString_StorageType_small — inline storage (no allocation); may be
 *    promoted if the data does not fit.
 *  - @c MemeString_StorageType_medium — stack-allocated variable buffer.
 *  - @c MemeString_StorageType_large — heap-allocated, reference-counted node.
 *
 * @param[out] _out         Pointer to the stack object to initialize.
 *                          Must not be @c NULL.
 * @param[in]  _object_size Byte size of the caller's @c mmsstk_t instance.
 * @param[in]  _utf8        Pointer to the UTF-8 encoded byte data.
 *                          If @c NULL, @p _out is initialized to an empty string.
 * @param[in]  _len         Number of bytes in @p _utf8 to use.
 *                          Pass a negative value to compute length via @c strlen().
 * @param[in]  _suggest     Preferred storage tier.  Use
 *                          @c MemeString_StorageType_none for automatic
 *                          selection.
 *
 * @return  0 on success, or a non-zero error code on allocation failure.
 *
 * @see MemeStringStack_initByU8bytes()
 * @see MemeStringStack_unInit()
 */
MEME_API int
	MEME_STDCALL MemeStringStack_initByU8bytesAndType(
		mmsstk_t* _out, size_t _object_size, const MemeByte_t* _utf8, MemeInteger_t _len,
		MemeString_Storage_t _suggest);

/**
 * @brief Initialize a MemeStringStack from a UTF-16 encoded buffer.
 *
 * Converts the UTF-16 code units in @p _buf to a UTF-8 string stored in
 * @p _out.  Storage tier is chosen automatically based on the resulting
 * UTF-8 byte length.  This is equivalent to calling
 * MemeStringStack_initByU16bytesAndType() with
 * @c MemeString_StorageType_none as the storage hint.
 *
 * @param[out] _out         Pointer to the stack object to initialize.
 *                          Must not be @c NULL.
 * @param[in]  _object_size Byte size of the caller's @c mmsstk_t instance.
 * @param[in]  _buf         Pointer to the UTF-16 code-unit array.
 *                          If @c NULL, @p _out is initialized to an empty string.
 * @param[in]  _len         Number of UTF-16 code units in @p _buf (not bytes).
 *                          Pass a negative value to compute the length via the
 *                          internal @c strlen16() equivalent.
 *
 * @return  0 on success, or a non-zero error code on allocation / conversion
 *          failure.
 *
 * @see MemeStringStack_initByU16bytesAndType()
 * @see MemeStringStack_unInit()
 */
MEME_API int
	MEME_STDCALL MemeStringStack_initByU16bytes(
		mmsstk_t* _out, size_t _object_size, const uint16_t* _buf, MemeInteger_t _len);

/**
 * @brief Initialize a MemeStringStack from a UTF-16 buffer with a
 *        storage-tier hint.
 *
 * Converts the UTF-16 code units in @p _buf to UTF-8, then stores the result
 * in @p _out using the tier indicated by @p _suggest.  The conversion length
 * is calculated first; the appropriate internal storage (small / medium / large)
 * is then allocated and the converted bytes are written into it.
 *
 * @param[out] _out         Pointer to the stack object to initialize.
 *                          Must not be @c NULL.
 * @param[in]  _object_size Byte size of the caller's @c mmsstk_t instance.
 * @param[in]  _buf         Pointer to the UTF-16 code-unit array.
 *                          If @c NULL, @p _out is initialized to an empty string.
 * @param[in]  _len         Number of UTF-16 code units in @p _buf (not bytes).
 *                          Pass a negative value to compute the length
 *                          automatically.
 * @param[in]  _suggest     Preferred storage tier.  Use
 *                          @c MemeString_StorageType_none for automatic
 *                          selection.
 *
 * @return  0 on success, or a non-zero error code on allocation / conversion
 *          failure.
 *
 * @see MemeStringStack_initByU16bytes()
 * @see MemeStringStack_unInit()
 */
MEME_API int
	MEME_STDCALL MemeStringStack_initByU16bytesAndType(
		mmsstk_t* _out, size_t _object_size, const uint16_t* _buf, MemeInteger_t _len,
		MemeString_Storage_t _suggest);

/**
 * @brief Initialize a MemeStringStack as a hexadecimal representation of
 *        raw binary data.
 *
 * Encodes each byte of @p _hexs as two uppercase hexadecimal ASCII characters
 * (@c '0'–@c '9', @c 'A'–@c 'F'), optionally inserting a separator string
 * @p _interval between consecutive byte encodings.
 *
 * For example, with @p _hexs = @c {0xDE, 0xAD, 0xBE}, @p _len = 3, and
 * @p _interval = @c ":", the result is @c "DE:AD:BE".
 *
 * @param[out] _out         Pointer to the stack object to initialize.
 *                          Must not be @c NULL.
 * @param[in]  _object_size Byte size of the caller's @c mmsstk_t instance.
 * @param[in]  _interval    Optional separator bytes inserted between each
 *                          encoded hex pair.  Pass @c NULL or an empty string
 *                          for no separator.
 * @param[in]  _ivlen       Byte length of @p _interval.
 *                          Pass a negative value to compute it via @c strlen().
 *                          Ignored (treated as 0) when @p _interval is @c NULL.
 * @param[in]  _hexs        Pointer to the raw binary data to encode.
 *                          If @c NULL, @p _out is initialized to an empty string.
 * @param[in]  _len         Number of bytes in @p _hexs to encode.
 *                          If ≤ 0, @p _out is initialized to an empty string.
 *
 * @return  0 on success, or a non-zero error code on allocation failure.
 *
 * @note The resulting string length is exactly
 *       @c (_len * 2 + (_len - 1) * _ivlen) characters when @p _len > 0.
 *
 * @see MemeStringStack_unInit()
 */
MEME_API int
	MEME_STDCALL MemeStringStack_initWithHexadecimals(
		mmsstk_t* _out, size_t _object_size,
		const MemeByte_t* _interval, MemeInteger_t _ivlen,
		const uint8_t* _hexs, MemeInteger_t _len);

/**
 * @brief Initialize a MemeStringStack from another MemeString, converting
 *        non-owning storage types to owned copies.
 *
 * This function is a conditional variant of MemeStringStack_initByOther().
 * When @p _other uses a storage type that does not own its byte data — namely
 * the **user** type or the unsafe **view** type — the raw bytes are copied
 * into a new owned string via MemeStringStack_initByU8bytes().  For all other
 * storage types (small, medium, large), MemeStringStack_initByOther() is
 * called directly, which may share reference-counted data.
 *
 * Use this function when you need @p _out to outlive the source backing buffer
 * (e.g., when the source is a view over a temporary buffer).
 *
 * @param[out] _out         Pointer to the stack object to initialize.
 *                          Must not be @c NULL.
 * @param[in]  _object_size Byte size of the caller's @c mmsstk_t instance.
 * @param[in]  _other       Source string to copy from.
 *                          If @c NULL, @p _out is initialized to an empty string.
 *
 * @return  0 on success, or a non-zero error code on allocation failure.
 *
 * @see MemeStringStack_initByOther()
 * @see MemeStringStack_initByU8bytes()
 * @see MemeStringStack_unInit()
 */
MEME_API int
	MEME_STDCALL MemeStringStack_initAndConditionalConvert(
        mmstrstk_t* _out, size_t _object_size, mmstr_cptr_t _other);

/**
 * @brief Destroy a MemeStringStack object and release all associated resources.
 *
 * Releases any memory or reference counts held by @p _out according to its
 * current storage class:
 *
 *  - **small** / **view** — No action; no resources to free.
 *  - **medium**           — The medium buffer is deallocated.
 *  - **large**            — The reference count of the shared node is
 *                           decremented; the node is freed when it reaches 0.
 *  - **user**             — The user-object's destruct callback is invoked.
 *
 * After this call the memory pointed to by @p _out is in an unspecified state
 * and must not be used again without re-initializing.
 *
 * @param[in,out] _out        Pointer to an initialized MemeStringStack object.
 *                            Must not be @c NULL.
 * @param[in]     _object_size Byte size of the caller's @c mmsstk_t instance.
 *
 * @return  0 on success, @c MGEC__OPNOTSUPP for an unrecognized storage type.
 *
 * @note @p _out must have been successfully initialized before this call.
 * @note Calling this function on an already-uninitialized object is undefined
 *       behavior.
 *
 * @see MemeStringStack_init()
 * @see MemeStringStack_reset()
 */
MEME_API int
	MEME_STDCALL MemeStringStack_unInit(mmsstk_t* _out, size_t _object_size);

/**
 * @brief Reset an initialized MemeStringStack to the empty state, retaining
 *        its storage type where possible.
 *
 * Clears the string content without destroying the object.  Unlike
 * MemeStringStack_unInit(), the object remains valid after this call and can
 * be reused directly.  The exact behavior per storage class:
 *
 *  - **small** / **view** — The inline content is cleared in-place; no
 *                           allocation or deallocation.
 *  - **medium**           — The medium buffer is freed (deallocated); the
 *                           object is then re-initialized as an empty **small**
 *                           string.
 *  - **large**            — The reference count is decremented; the heap block
 *                           is freed when it reaches zero.  The object is then
 *                           re-initialized as an empty **small** string.
 *  - **user**             — The user-object's reference count is decremented;
 *                           the destructor is invoked when it reaches zero.
 *                           The object is then re-initialized as an empty
 *                           **small** string.
 *
 * @param[in,out] _out        Pointer to an initialized MemeStringStack object.
 *                            Must not be @c NULL.
 * @param[in]     _object_size Byte size of the caller's @c mmsstk_t instance.
 *
 * @return  0 on success, @c MGEC__OPNOTSUPP for an unrecognized storage type.
 *
 * @note @p _out must have been successfully initialized before this call.
 * @note The object remains valid and initialized after this call; it can be
 *       reused without re-calling any @c init function.
 *
 * @see MemeStringStack_init()
 * @see MemeStringStack_unInit()
 */
MEME_API int
	MEME_STDCALL MemeStringStack_reset (mmsstk_t* _out, size_t _object_size);

//! @param _s The string stack object, must be initialized.
MEME_API int
	MEME_STDCALL MemeStringStack_assign(
		mmsstk_t* _s, size_t _object_size, MemeString_Const_t _other);

//! @param _s The string stack object, must be initialized.
MEME_API int
MEME_STDCALL MemeStringStack_assignByU8bytes(
	mmsstk_t* _s, size_t _object_size, const MemeByte_t* _utf8, MemeInteger_t _len);

//! @param _s The string stack object, must be initialized.
MEME_API int
MEME_STDCALL MemeStringStack_assignByBuffer(
	mmsstk_t* _out, size_t _object_size, MemeBuffer_Const_t _other, MemeInteger_t _offset);

MEME_API int
MEME_STDCALL MemeStringStack_regSize(const mmstrstk_t* _str);

MEME_API int
MEME_STDCALL MemeStringStack_objSize(const mmstrstk_t* _str);

//! @param _s The string stack object, must be initialized.
//! @deprecated May cause ABI issues in the future
MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_mid(
	const mmsstk_t* _s, size_t _object_size, MemeInteger_t _offset, MemeInteger_t _len);

//! @param _str Must be initialized
//! @param _obj_size If negative, it is assumed internally that @c _out is already initialized; if _out is not initialized, please pass the object size of @c _out
MEME_API mgec_t
MEME_STDCALL MemeStringStack_mid_v2(
    const mmstrstk_t* _str, mmint_t _offset, mmint_t _count, mmstrstk_t* _out, mmint_t _obj_size);

//! @param _s The string stack object, must be initialized.
//! @param _other The string stack object, must be initialized.
//! @deprecated May cause ABI issues in the future
MEME_API mmstrstk_t
MEME_STDCALL MemeStringStack_concat(
    const mmstrstk_t* _s, size_t _object_size, const mmstrstk_t* _other);

//! @param _str Must be initialized
//! @param _other Must be initialized
//! @param _out The output string stack object. If the function returns an error, it will initialize the @c _out parameter.
//! @param _obj_size If negative, it is assumed internally that @c _out is already initialized; if _out is not initialized, please pass the object size of @c _out
MEME_API mgec_t 
MEME_STDCALL MemeStringStack_concat_v2(
    const mmstrstk_t* _str, const mmstrstk_t* _other, mmstrstk_t* _out, mmint_t _obj_size);

//! @param _s The string stack object, must be initialized.
//! @deprecated May cause ABI issues in the future
MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_toEnUpper(
	const mmsstk_t* _s, size_t _object_size);

//! @param _out The output string stack object. If the function returns an error, it will initialize the @c _out parameter.
MEME_API mgec_t
MEME_STDCALL MemeStringStack_toEnUpper_v2(
	const mmstrstk_t* _str, mmstrstk_t* _out, mmint_t _obj_size);

//! @param _s The string stack object, must be initialized.
//! @deprecated May cause ABI issues in the future
MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_toEnLower(
	const mmsstk_t* _s, size_t _object_size);

//! @param _out The output string stack object. If the function returns an error, it will initialize the @c _out parameter.
MEME_API mgec_t
MEME_STDCALL MemeStringStack_toEnLower_v2(
	const mmstrstk_t* _str, mmstrstk_t* _out, mmint_t _obj_size);

//! @param _s The string stack object, must be initialized.
//! @deprecated May cause ABI issues in the future
MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_trimSpace(
    const mmsstk_t* _s, size_t _object_size);

MEME_API mgec_t
MEME_STDCALL MemeStringStack_trimSpace_v2(
	const mmstrstk_t* _str, mmstrstk_t* _out, mmint_t _obj_size);

//! @param _s The string stack object, must be initialized.
//! @deprecated May cause ABI issues in the future
MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_trimLeftSpace(
    const mmsstk_t* _s, size_t _object_size);

MEME_API mgec_t
MEME_STDCALL MemeStringStack_trimLeftSpace_v2(
    const mmstrstk_t* _str, mmstrstk_t* _out, mmint_t _obj_size);

//! @param _s The string stack object, must be initialized.
//! @deprecated May cause ABI issues in the future
MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_trimRightSpace(
    const mmsstk_t* _s, size_t _object_size);

MEME_API mgec_t
MEME_STDCALL MemeStringStack_trimRightSpace_v2(
    const mmstrstk_t* _str, mmstrstk_t* _out, mmint_t _obj_size);

//! @param _s The string stack object, must be initialized.
//! @deprecated May cause ABI issues in the future
MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_trimByCuts(
    const mmsstk_t* _s, size_t _object_size,
	const char* _cuts, MemeInteger_t _cuts_len);

MEME_API mgec_t
MEME_STDCALL MemeStringStack_trimByCuts_v2(
    const mmstrstk_t* _str, const char* _cuts, mmint_t _cuts_len, mmstrstk_t* _out, mmint_t _obj_size);

//! @param _s The string stack object, must be initialized.
//! @param _cond_func The condition function; if the function returns true, the byte will be skipped.
//! @deprecated May cause ABI issues in the future
MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_trimByCondByteFunc(
    const mmsstk_t* _s, size_t _object_size,
	MemeString_MatchCondByteFunc_t* _cond_func, void* _user_data);

MEME_API mgec_t
MEME_STDCALL MemeStringStack_trimByCondByteFunc_v2(
    const mmstrstk_t* _str, mmstr_match_cond_byte_cb_t* _cond_func, void* _user_data, 
	mmstrstk_t* _out, mmint_t _obj_size);

//! @param _cond_func The condition function; if the function returns true, the rune will be skipped.
MEME_API mgec_t
MEME_STDCALL MemeStringStack_trimByCondRuneFunc(
    const mmstrstk_t* _str, mmstr_match_cond_rune_cb_t* _cond_func, void* _user_data,
    mmstrstk_t* _out, mmint_t _obj_size);

//! \code
//! MemeStringStack_t s = MemeStringStack_getRepeat(MEME_STRING__OBJECT_SIZE, 3, "abc", 3);
//! // s is "abcabcabc";
//! \endcode
//! @param _s The string stack object, must be initialized.
//! @deprecated May cause ABI issues in the future
MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_getRepeat(
	size_t _object_size, mmint_t _count, const char* _s, mmint_t _len);

//! @param _out The output string stack object. If the function returns an error, it will initialize the @c _out parameter.
MEME_API mgec_t
MEME_STDCALL MemeStringStack_getRepeat_v2(
    mmstrstk_t* _out, mmint_t _obj_size, mmint_t _count, const char* _in, mmint_t _in_len);

//! @param _s The string stack object, must be initialized.
//! @deprecated May cause ABI issues in the future
MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_replace(
    const mmsstk_t* _s, size_t _object_size,
    const char* _from, mmint_t _from_len,
    const char* _to, mmint_t _to_len, mmint_t _max_count);

//! @param _str Must be initialized
//! @param _out The output string stack object. If the function returns an error, it will initialize the @c _out parameter.
//! @param _obj_size If negative, it is assumed internally that @c _out is already initialized; if _out is not initialized, please pass the object size of @c _out
MEME_API mgec_t
MEME_STDCALL MemeStringStack_replace_v2(
    const mmstrstk_t* _str, 
    const char* _from, mmint_t _from_len,
    const char* _to, mmint_t _to_len, mmint_t _max_count,
	mmstrstk_t* _out, mmint_t _obj_size
);

//! @param _s The string stack object, must be initialized.
//! @deprecated May cause ABI issues in the future
MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_toValidUtf8(
    const mmsstk_t* _s, size_t _object_size);

MEME_API mgec_t
MEME_STDCALL MemeStringStack_toValidUtf8_v2(
    const mmstrstk_t* _str, mmstrstk_t* _out, mmint_t _obj_size);

//MEME_API mmstrstk_t
//MEME_STDCALL MemeStringStack_fromInt(
//    size_t _object_size, mmint_t _value, int _radix);
//
//MEME_API mmstrstk_t
//MEME_STDCALL MemeStringStack_fromInt16(
//    size_t _object_size, int16_t _value, int _radix);
//
//MEME_API mmstrstk_t
//MEME_STDCALL MemeStringStack_fromInt32(
//    size_t _object_size, int32_t _value, int _radix);
//
//MEME_API mmstrstk_t
//MEME_STDCALL MemeStringStack_fromInt64(
//    size_t _object_size, int64_t _value, int _radix);
//

MEME_API mgec_t
MEME_STDCALL MemeStringStack_fromUInt16(
   uint16_t _value, int _radix, mmstrstk_t* _out, mmint_t _obj_size);

MEME_API mgec_t
MEME_STDCALL MemeStringStack_fromUInt32(
   uint32_t _value, int _radix, mmstrstk_t* _out, mmint_t _obj_size);

MEME_API mgec_t
MEME_STDCALL MemeStringStack_fromUInt64(
   uint64_t _value, int _radix, mmstrstk_t* _out, mmint_t _obj_size);

//MEME_API mmstrstk_t
//MEME_STDCALL MemeStringStack_fromFloat(
//    size_t _object_size, float _value, int _radix);
//
//MEME_API mmstrstk_t
//MEME_STDCALL MemeStringStack_fromDouble(
//    size_t _object_size, double _value, int _radix);

//! @param _s The string stack object, must be initialized.
//! @param _mapping_fn The mapping function; if the function returns less than 0, the rune will be convert.
//! @deprecated May cause ABI issues in the future
MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_mappingConvert(
    const mmsstk_t* _s, size_t _object_size,
    MemeString_MappingConvertFunc_t _mapping_fn, void* _user_data);

MEME_API mgec_t 
MEME_STDCALL MemeStringStack_mappingConvert_v2(
    const mmstrstk_t* _str, mmstr_mapping_conv_cb_t* _mapping_fn, void* _user_data,
    mmstrstk_t* _out, mmint_t _obj_size);

//! @deprecated May cause ABI issues in the future
MEME_API MemeStringStack_t 
MemeStringStack_vformatInCstyle(
    size_t _object_size, 
	const char* _format,
	MemeInteger_t _size_limit, va_list _args);

//! @deprecated May cause ABI issues in the future
MEME_API MemeStringStack_t
MemeStringStack_formatInCstyle(
    size_t _object_size, 
	const char* _format,
	MemeInteger_t _size_limit, ...);

//! @param _s The string stack object, must be initialized.
//! @deprecated May cause ABI issues in the future
MEME_API mmsstk_t MEME_STDCALL
MemeStringStack_vformatInCstyle_v2(
	size_t _object_size,
	MemeInteger_t _size_limit,
	MEGO_SYMBOL__MSVC_FORMAT_STRING(const char* _format),
	va_list _args);

//! @deprecated May cause ABI issues in the future
MEME_API mmstrstk_t MEME_STDCALL
MemeStringStack_vformatWithLimitInCstyle(
	size_t _object_size,
	mmint_t _size_limit,
	mmint_t _pre_size,
	MEGO_SYMBOL__MSVC_FORMAT_STRING(const char* _format),
	va_list _args);

MEME_API mgec_t MEME_STDCALL
MemeStringStack_vformatWithLimitInCstyle_v2(
	mmstrstk_t* _str,
	mmint_t _object_size,
	mmint_t _size_limit,
	mmint_t _pre_size,
	MG_SYM__MSVC_FMT_STR(const char* _format),
	va_list _args);

MEME_API mgec_t MEME_STDCALL
MemeStringStack_formatWithLimitInCstyle(
	mmstrstk_t* _str,
	mmint_t _object_size,
	mmint_t _size_limit,
	mmint_t _pre_size,
	MG_SYM__MSVC_FMT_STR(const char* _format),
	...)
	MG_SYM__GCC_ATTR_FMT(printf, 5, 6);

//! @param _s The string stack object, must be initialized.
//! @deprecated May cause ABI issues in the future
MEME_API mmsstk_t
MemeStringStack_formatInCstyle_v2(
	size_t _object_size,
	MemeInteger_t _size_limit,
	MEGO_SYMBOL__MSVC_FORMAT_STRING(const char* _format),
	...)
	MEGO_SYMBOL__GCC_ATTRIBUTE_FORMAT(printf, 3, 4);

MEME_API mgec_t
MEME_STDCALL MemeStringStack_join(
	mmstrstk_t* _str, mmint_t _obj_size, const char* _separator, mmint_t _separator_len, 
	const mmstrstk_t* _items, mmint_t _item_count);


//! @brief Split the string into substrings.
//! @param _s The string.
//! @param _key The key to split the string.
//! @param _key_len The length of the key.
//! @param _behavior The behavior of the split.
//! @param _cs The case sensitivity of the split.
//! @param _out The output array of substrings.
//! @param _out_count The number of substrings.
MEME_API mmint_t
MEME_STDCALL MemeStringStack_split(
	const mmstrstk_t* _s,
	const char* _key, mmint_t _key_len,
	mmflag_split_behav_t, mmflag_case_sensit_t,
	mmstrstk_t* MEGO_SYMBOL__RESTRICT _out, mmint_t _obj_size,
	mmint_t* MEGO_SYMBOL__RESTRICT _out_count,
	mmint_t* MEGO_SYMBOL__RESTRICT _search_index
);

MEME_API mmint_t
MEME_STDCALL MemeStringStack_splitByCondByteFunc(
	const mmstrstk_t* _str,
	mmstr_match_cond_byte_cb_t* _cond_func, void* _user_data,
    mmstrstk_t* MEGO_SYMBOL__RESTRICT _out, mmint_t _obj_size,
	mmint_t* MEGO_SYMBOL__RESTRICT _out_count,
	mmint_t* MEGO_SYMBOL__RESTRICT _search_index
);

//MEME_API MemeInteger_t
//MEME_STDCALL MemeStringStack_toDouble(
//    const MemeStringStack_t* _s, size_t _object_size, double* _out);

MEME_API MemeString_Storage_t
	MEME_STDCALL MemeString_storageType(MemeString_Const_t _s);

MEME_API int 
	MEME_STDCALL MemeString_create(MemeString_t* _out);

//MEME_API int 
//	MEME_STDCALL MemeString_createWithType(MemeString_t* _out, MemeString_Storage_t);

//MEME_API int 
//	MEME_STDCALL MemeString_createByOther(mmstr_ptr_t* _out, mmstr_cptr_t _other);
//
//MEME_API int 
//	MEME_STDCALL MemeString_createByRune(mmstr_ptr_t* _out, size_t _len, mmrune_t _ch);
//
//MEME_API int 
//	MEME_STDCALL MemeString_createByUtf8bytes(
//		mmstr_ptr_t* _out, const mmbyte_t* _utf8, mmint_t _len);

MEME_API int 
	MEME_STDCALL MemeString_destroy(MemeString_t* _out);

MEME_API int 
	MEME_STDCALL MemeString_reset(mmstr_ptr_t _out);

MEME_API int 
	MEME_STDCALL MemeString_assign(MemeString_t _s, MemeString_Const_t _other);

MEME_API int 
	MEME_STDCALL MemeString_swap(MemeString_t _lhs, MemeString_t _rhs);

MEME_API int 
	MEME_STDCALL MemeString_isNonempty(MemeString_Const_t _s);

MEME_API int
	MEME_STDCALL MemeString_isEmpty(MemeString_Const_t _s);

MEME_API const char* 
	MEME_STDCALL MemeString_cStr(MemeString_Const_t _s);

//! @brief Get the length of the string in bytes.
//! @return The length of the string in bytes.
//! @note The length of the string in bytes is not necessarily equal to the length of the string in words.
MEME_API MemeInteger_t 
	MEME_STDCALL MemeString_byteSize(MemeString_Const_t _s);

//! @brief Get the length of the string in words.
//! @return The length of the string in words.
//! @note The length of the string in words is not necessarily equal to the length of the string in bytes.
MEME_API MemeInteger_t 
	MEME_STDCALL MemeString_runeSize(mmstr_cptr_t _s);
MEME_API MemeInteger_t
	MEME_STDCALL MemeString_u16CharSize(mmstr_cptr_t _s);
MEME_API MemeInteger_t 
	MEME_STDCALL MemeString_availableByteCapacity(MemeString_Const_t _s);
MEME_API MemeInteger_t
	MEME_STDCALL MemeString_maxByteSize(MemeString_Const_t _s);
MEME_API MemeInteger_t
	MEME_STDCALL MemeString_maxByteCapacity(MemeString_Const_t _s);

//! @brief Get the byte pointer at the specified index.
//! @param _s The string.
//! @param _index The index of the byte.
//! @return The byte pointer at the specified index.
//! @note The index must be in the range [0, MemeString_wordSize(_s)).
MEME_API const MemeByte_t*
	MEME_STDCALL MemeString_at(MemeString_Const_t _s, MemeInteger_t _index);

MEME_API const MemeByte_t*
	MEME_STDCALL MemeString_front(mmstr_cptr_t _s);

MEME_API const MemeByte_t*
	MEME_STDCALL MemeString_back(mmstr_cptr_t _s);

MEME_API mmrune_t
	MEME_STDCALL MemeString_runeFront(mmstr_cptr_t _s);

MEME_API mmrune_t
	MEME_STDCALL MemeString_runeBack(mmstr_cptr_t _s);

MEME_API int
	MEME_STDCALL MemeString_isEqual(
		MemeString_Const_t _s, const char* _str, MemeInteger_t _len, int* _result);
MEME_API int
	MEME_STDCALL MemeString_isEqualWithOther(
		MemeString_Const_t _lhs, MemeString_Const_t _rhs, int* _result);

//! @deprecated The function name is ambiguous and is not recommended for use.
MEME_API int
	MEME_STDCALL MemeString_containsOnlyAscii(mmstr_cptr_t _s, int* _result);

MEME_API int
	MEME_STDCALL MemeString_isOnlyAscii(mmstr_cptr_t _s, int* _result);

MEME_API int
MEME_STDCALL MemeString_compare(mmstr_cptr_t _s, mmstr_cptr_t _other);

MEME_API int
MEME_STDCALL MemeString_compareByUtf8bytes(
    mmstr_cptr_t _s, const mmbyte_t* _other, mmint_t _len);
//
//MEME_API int
//MEME_STDCALL MemeString_runeCompare(mmstr_cptr_t _s, mmstr_cptr_t _other);
//
//MEME_API int
//MEME_STDCALL MemeString_runeCompareByUtf8bytes(
//	mmstr_cptr_t _s, const mmbyte_t* _other, mmint_t _len);

MEME_API MemeInteger_t
	MEME_STDCALL MemeString_indexOfWithUtf8bytes(
		MemeString_Const_t _s, MemeInteger_t _offset,
		const MemeByte_t* _needle, MemeInteger_t _needle_len,
		MemeFlag_CaseSensitivity_t _cs);

MEME_API MemeInteger_t
	MEME_STDCALL MemeString_indexOfWithUtf8bytesAndSizeLimit(
		mmstr_cptr_t _s, mmint_t _offset, mmint_t _limit,
		const mmbyte_t* _needle, mmint_t _needle_len,
		MemeFlag_CaseSensitivity_t _cs);

MEME_API mmint_t
	MEME_STDCALL MemeString_indexOfUtf8bytes(
		mmstr_cptr_t _str, mmint_t _offset, mmint_t _limit,
		const mmbyte_t* _needle, mmint_t _needle_len,
		int _full_match, mmflag_case_sensit_t _cs);

MEME_API MemeInteger_t
MEME_STDCALL MemeString_indexOfWithByte(
	MemeString_Const_t _s, MemeInteger_t _offset, MemeByte_t _byte,
	MemeFlag_CaseSensitivity_t _cs);

MEME_API MemeInteger_t
	MEME_STDCALL MemeString_indexOfWithOther(
		MemeString_Const_t _s, MemeInteger_t _offset,
		MemeString_Const_t _other, MemeFlag_CaseSensitivity_t _cs);

MEME_API mmint_t
	MEME_STDCALL MemeString_indexOfOther(
		mmstr_cptr_t _str, mmint_t _offset, mmint_t _limit,
		mmstr_cptr_t _other, mmint_t _count, int _full_match, mmflag_case_sensit_t _cs);

//! param _cond_func The function to determine whether the byte at the specified index meets the condition; return type is mmflag_cbproc_t.
MEME_API MemeInteger_t
MEME_STDCALL MemeString_indexByCondByteFunc(
    MemeString_Const_t _s, MemeInteger_t _offset,
    MemeString_MatchCondByteFunc_t* _cond_func, void* _user_data);

MEME_API MemeInteger_t
MEME_STDCALL MemeString_lastIndexOfWithUtf8bytes(
    MemeString_Const_t _s, MemeInteger_t _limit,
    const MemeByte_t* _needle, MemeInteger_t _needle_len,
    MemeFlag_CaseSensitivity_t _cs);

MEME_API mmint_t
MEME_STDCALL MemeString_lastIndexOfUtf8bytes(
	mmstr_cptr_t _s, mmint_t _offset, mmint_t _limit,
    const mmbyte_t* _needle, mmint_t _needle_len, 
	int _full_match, mmflag_case_sensit_t _cs);

MEME_API mmint_t
MEME_STDCALL MemeString_lastIndexOfOther(
	mmstr_cptr_t _s, mmint_t _offset, mmint_t _limit, 
	mmstr_cptr_t _other, mmint_t _count, int _full_match, mmflag_case_sensit_t _cs);

MEME_API MemeInteger_t
MEME_STDCALL MemeString_matchCountWithUtf8bytes(
    MemeString_Const_t _s, MemeInteger_t _offset,
    const MemeByte_t* _needle, MemeInteger_t _needle_len,
    MemeFlag_CaseSensitivity_t _cs);

MEME_API MemeInteger_t
MEME_STDCALL MemeString_startsMatchWithOther(
	MemeString_Const_t _s, MemeString_Const_t _other, MemeFlag_CaseSensitivity_t _cs);

MEME_API MemeInteger_t
MEME_STDCALL MemeString_startsMatchWithUtf8bytes(
    MemeString_Const_t _s, 
	const MemeByte_t* _needle, MemeInteger_t _needle_len, MemeFlag_CaseSensitivity_t _cs);

MEME_API MemeInteger_t
MEME_STDCALL MemeString_endsMatchWithOther(
	MemeString_Const_t _s, MemeString_Const_t _other, MemeFlag_CaseSensitivity_t _cs);

MEME_API MemeInteger_t
MEME_STDCALL MemeString_endsMatchWithUtf8bytes(
	MemeString_Const_t _s, 
	const MemeByte_t* _needle, MemeInteger_t _needle_len, MemeFlag_CaseSensitivity_t _cs);

MEME_API mmint_t
MEME_STDCALL MemeString_foreach(
    mmstr_cptr_t _str, mmstr_foreach_rune_cb_t* _cb, void* _user_data);

//MEME_API mgec_t
//MEME_STDCALL MemeString_extractInDelimiters(
//	mmstr_cptr_t _str, 
//	const mmbyte_t* _delim_left,  mmint_t _left_len,
//	const mmbyte_t* _delim_right, mmint_t _right_len,
//	mmint_t* _begin, mmint_t* _end);

//
//MEME_API mgec_t
//MEME_STDCALL MemeString_toInteger(mmstr_cptr_t _s, mmint_t* _out, int _base);

//! @brief Split the string into substrings.
//! @param _s The string.
//! @param _key The key to split the string.
//! @param _key_len The length of the key.
//! @param _behavior The behavior of the split.
//! @param _cs The case sensitivity of the split.
//! @param _out The output array of substrings.
//! @param _out_count The number of substrings.
//! @deprecated 将来可能会有ABI问题
MEME_API MemeInteger_t 
	MEME_STDCALL MemeString_split(
	MemeString_Const_t _str,
	const char* _key, MemeInteger_t _key_len, 
	MemeFlag_SplitBehavior_t, MemeFlag_CaseSensitivity_t,
	MemeStringStack_t* MEGO_SYMBOL__RESTRICT _out, 
	MemeInteger_t* MEGO_SYMBOL__RESTRICT _out_count,
	MemeInteger_t* MEGO_SYMBOL__RESTRICT _search_index
);

//! This is an example of how to use MemeString_splitByCondByteFunc.
//! \code 
//! int notalpha_func(void*, MemeByte_t _byte) {
//!     return !isalpha(_byte);
//! }
//! 
//! ...
//! 
//! MemeStringStack_t stack[3];
//! MemeInteger_t count = 3;
//! MemeInteger_t search_index;
//! MemeInteger_t result = 
//!     MemeString_splitByCondByteFunc(
//!         _s, // _s is ":::abc-def77vv..."
//!         notalpha_func, NULL,
//!         &stack, &count, &search_index
//!     );
//! // stack list is [ "abc", "def", "vv" ]
//! 
//! ...
//! 
//! \endcode
//! @deprecated May cause ABI issues in the future
MEME_API mmint_t
	MEME_STDCALL MemeString_splitByCondByteFunc
	(
		mmstr_cptr_t _s,
		MemeString_MatchCondByteFunc_t* _cond_func, void* _user_data,
		mmstrstk_t* MEGO_SYMBOL__RESTRICT _out,
		mmint_t * MEGO_SYMBOL__RESTRICT _out_count,
		mmint_t * MEGO_SYMBOL__RESTRICT _search_index
	);

MEME_API mmint_t
MEME_STDCALL MemeString_writeBytes(
	mmstr_cptr_t _s, mmint_t _offset, mmint_t _count, mmbyte_t* _out);

MEME_API MemeInteger_t
MEME_STDCALL MemeString_writeU16Chars(mmstr_cptr_t _s, uint16_t* _out);

MEME_API const MemeByte_t*
	MEME_STDCALL MemeString_byteData(MemeString_Const_t _s);

MEME_API MemeInteger_t
	MEME_STDCALL MemeString_isSharedStorageTypes(MemeString_Const_t _s);

MEME_API MemeInteger_t
	MEME_STDCALL MemeString_getSharedHeapByteSize(MemeString_Const_t _s);

MEME_API MemeInteger_t
	MEME_STDCALL MemeString_getPrivateHeapByteSize(MemeString_Const_t _s);

MEME_API MemeInteger_t
	MEME_STDCALL MemeString_checkHeadTailMemory(MemeString_Const_t _s);



//****************************************************************************/

MG_CAPI_INLINE mmstrstk_t
mmstrstk_get_init_v0(size_t _object_size)
{
    assert(_object_size != 0 && "mmstrstk_get_init_v0");
	
	mmstrstk_t _out;
	MemeStringStack_init(&_out, _object_size);
	return _out;
}

MG_CAPI_INLINE mmstrstk_t
mmstrstk_get_init()
{	
	mmstrstk_t _out;
	MemeStringStack_init(&_out, MMSTR__OBJ_SIZE);
	return _out;
}

MG_CAPI_INLINE int 
mmstrstk_init_v0(mmstrstk_t* _out, size_t _obj_size)
{
	assert(_out   != NULL && "mmstrstk_init_v0");
    assert(_obj_size != 0 && "mmstrstk_init_v0");

	return MemeStringStack_init(_out, _obj_size);
}

MG_CAPI_INLINE int 
mmstrstk_init(mmstrstk_t* _out) 
{
	assert(_out != NULL && "mmstrstk_init");
	
	return MemeStringStack_init(_out, MMSTR__OBJ_SIZE);
}

MG_CAPI_INLINE int 
mmstrstk_init_by_other_v0(mmstrstk_t* _out, size_t _object_size, mmstr_cptr_t _other)
{
    assert(_out != NULL && "mmstrstk_init_by_other_v0");
    assert(_object_size != 0 && "mmstrstk_init_by_other_v0");
	
	if (MG_SYM__UNLIKELY(_other == NULL))
		return mmstrstk_init_v0(_out, _object_size);
    return MemeStringStack_initByOther(_out, _object_size, _other);
}

MG_CAPI_INLINE int 
mmstrstk_init_by_other(mmstrstk_t* _out, mmstr_cptr_t _other)
{
    assert(_out != NULL && "mmstrstk_init_by_other");
	
	if (MG_SYM__UNLIKELY(_other == NULL))
		return mmstrstk_init_v0(_out, MMSTR__OBJ_SIZE);
    return MemeStringStack_initByOther(_out, MMSTR__OBJ_SIZE, _other);
}

MG_CAPI_INLINE int 
mmstrstk_init_by_buf_v0(
	mmstrstk_t* _out, size_t _object_size, mmbuf_cptr_t _other, mmint_t _offset)
{
    assert(_out != NULL && "mmstrstk_init_by_buf_v0");
    assert(_object_size != 0 && "mmstrstk_init_by_buf_v0");
	
    if (MG_SYM__UNLIKELY(_other == NULL))
        return mmstrstk_init_v0(_out, _object_size);
    return MemeStringStack_initByBuffer(_out, _object_size, _other, _offset);
}

MG_CAPI_INLINE int 
mmstrstk_init_by_buf(
	mmstrstk_t* _out, mmbuf_cptr_t _other, mmint_t _offset)
{
    assert(_out != NULL && "mmstrstk_init_by_buf");
	
    if (MG_SYM__UNLIKELY(_other == NULL))
        return mmstrstk_init_v0(_out, MMSTR__OBJ_SIZE);
    return MemeStringStack_initByBuffer(_out, MMSTR__OBJ_SIZE, _other, _offset);
}

MG_CAPI_INLINE int
mmstrstk_init_by_rune_v0(
	mmstrstk_t* _out, size_t _obj_size, mmint_t _count, mmrune_t _rune)
{
    assert(_out != NULL && "mmstrstk_init_by_rune_v0");
    assert(_obj_size != 0 && "mmstrstk_init_by_rune_v0");
	
    return MemeStringStack_initByRune(_out, _obj_size, _count, _rune);
}
	
MG_CAPI_INLINE int
mmstrstk_init_by_rune(
	mmstrstk_t* _out, mmint_t _count, mmrune_t _rune)
{
    assert(_out != NULL && "mmstrstk_init_by_rune");
	
    return MemeStringStack_initByRune(_out, MMSTR__OBJ_SIZE, _count, _rune);
}
	
MG_CAPI_INLINE int
mmstrstk_init_by_u8_v0(
    mmstrstk_t* _out, size_t _object_size, const char* _utf8, mmint_t _utf8_len)
{
    assert(_out != NULL && "mmstrstk_init_by_u8_v0");
    assert(_object_size != 0 && "mmstrstk_init_by_u8_v0");
	
    if (MG_SYM__UNLIKELY(_utf8 == NULL))
        return mmstrstk_init_v0(_out, _object_size);
    return MemeStringStack_initByU8bytes(_out, _object_size, (const mmbyte_t*)_utf8, _utf8_len);
}

MG_CAPI_INLINE int
mmstrstk_init_by_u8(
    mmstrstk_t* _out, const char* _utf8, mmint_t _utf8_len)
{
    assert(_out != NULL && "mmstrstk_init_by_u8");
	
    if (MEGO_SYMBOL__UNLIKELY(_utf8 == NULL))
        return mmstrstk_init_v0(_out, MMSTR__OBJ_SIZE);
    return MemeStringStack_initByU8bytes(_out, MMSTR__OBJ_SIZE, (const mmbyte_t*)_utf8, _utf8_len);
}

MG_CAPI_INLINE int
mmstrstk_init_by_u8_with_type_v0(
    mmstrstk_t* _out, size_t _object_size, 
	const char* _utf8, mmint_t _utf8_len, mmstr_strg_t _suggest)
{
    assert(_out != NULL && "mmstrstk_init_by_u8_with_type_v0");
    assert(_object_size != 0 && "mmstrstk_init_by_u8_with_type_v0");
	
    if (MEGO_SYMBOL__UNLIKELY(_utf8 == NULL))
        return mmstrstk_init_v0(_out, _object_size);
    return MemeStringStack_initByU8bytesAndType(_out, _object_size, (const mmbyte_t*)_utf8, _utf8_len, _suggest);
}

MG_CAPI_INLINE int
mmstrstk_init_by_u8_with_type(
    mmstrstk_t* _out, const char* _utf8, mmint_t _utf8_len, mmstr_strg_t _suggest)
{
    assert(_out != NULL && "mmstrstk_init_by_u8_with_type");
	
    if (MEGO_SYMBOL__UNLIKELY(_utf8 == NULL))
        return mmstrstk_init_v0(_out, MMSTR__OBJ_SIZE);
    return MemeStringStack_initByU8bytesAndType(_out, MMSTR__OBJ_SIZE, (const mmbyte_t*)_utf8, _utf8_len, _suggest);
}

MG_CAPI_INLINE int
mmstrstk_init_by_u16_v0(
    mmstrstk_t* _out, size_t _object_size, const uint16_t* _utf16, mmint_t _utf16_len)
{
    assert(_out != NULL && "mmstrstk_init_by_u16_v0");
    assert(_object_size != 0 && "mmstrstk_init_by_u16_v0");
	
    if (MG_SYM__UNLIKELY(_utf16 == NULL))
        return mmstrstk_init_v0(_out, _object_size);
    return MemeStringStack_initByU16bytes(_out, _object_size, _utf16, _utf16_len);
}

MG_CAPI_INLINE int
mmstrstk_init_by_u16(
    mmstrstk_t* _out, const uint16_t* _utf16, mmint_t _utf16_len)
{
    assert(_out != NULL && "mmstrstk_init_by_u16");
	
    if (MG_SYM__UNLIKELY(_utf16 == NULL))
        return mmstrstk_init_v0(_out, MMSTR__OBJ_SIZE);
    return MemeStringStack_initByU16bytes(_out, MMSTR__OBJ_SIZE, _utf16, _utf16_len);
}

MG_CAPI_INLINE int
mmstrstk_init_by_u16_with_type_v0(
    mmstrstk_t* _out, size_t _object_size,
    const uint16_t* _utf16, mmint_t _utf16_len, mmstr_strg_t _suggest)
{
    assert(_out != NULL && "mmstrstk_init_by_u16_with_type_v0");
    assert(_object_size != 0 && "mmstrstk_init_by_u16_with_type_v0");
	
    if (MG_SYM__UNLIKELY(_utf16 == NULL))
        return mmstrstk_init_v0(_out, _object_size);
    return MemeStringStack_initByU16bytesAndType(_out, _object_size, _utf16, _utf16_len, _suggest);
}

MG_CAPI_INLINE int
mmstrstk_init_by_u16_with_type(
    mmstrstk_t* _out, 
    const uint16_t* _utf16, mmint_t _utf16_len, mmstr_strg_t _suggest)
{
    assert(_out != NULL && "mmstrstk_init_by_u16_with_type");
	
    if (MG_SYM__UNLIKELY(_utf16 == NULL))
        return mmstrstk_init_v0(_out, MMSTR__OBJ_SIZE);
    return MemeStringStack_initByU16bytesAndType(_out, MMSTR__OBJ_SIZE, _utf16, _utf16_len, _suggest);
}

MG_CAPI_INLINE int
mmstrstk_init_by_hex_v0(
	mmstrstk_t* _out, size_t _object_size,
	const mmbyte_t* _interval, mmint_t _ivlen, const uint8_t* _hexs, mmint_t _len)
{
    assert(_out != NULL && "mmstrstk_init_by_hex_v0");
    assert(_object_size != 0 && "mmstrstk_init_by_hex_v0");

	if (MG_SYM__UNLIKELY(_hexs == NULL))
		return mmstrstk_init_v0(_out, _object_size);
	return MemeStringStack_initWithHexadecimals(_out, _object_size, _interval, _ivlen, _hexs, _len);
}

MG_CAPI_INLINE int
mmstrstk_init_by_hex(
	mmstrstk_t* _out, 
	const mmbyte_t* _interval, mmint_t _ivlen, const uint8_t* _hexs, mmint_t _len)
{
    assert(_out != NULL && "mmstrstk_init_by_hex");

	if (MG_SYM__UNLIKELY(_hexs == NULL))
		return mmstrstk_init_v0(_out, MMSTR__OBJ_SIZE);
	return MemeStringStack_initWithHexadecimals(_out, MMSTR__OBJ_SIZE, _interval, _ivlen, _hexs, _len);
}

MG_CAPI_INLINE int
mmstrstk_init_cond_copy_v0(
	mmstrstk_t* _out, size_t _object_size, mmstr_cptr_t _other)
{
    assert(_out != NULL && "mmstrstk_init_cond_copy_v0");
    assert(_object_size != 0 && "mmstrstk_init_cond_copy_v0");
	
    return MemeStringStack_initAndConditionalConvert(_out, _object_size, _other);
}

MG_CAPI_INLINE int
mmstrstk_init_cond_copy(mmstrstk_t* _out, mmstr_cptr_t _other)
{
    assert(_out != NULL && "mmstrstk_init_cond_copy");
	
    return MemeStringStack_initAndConditionalConvert(_out, MMSTR__OBJ_SIZE, _other);
}

MG_CAPI_INLINE int
mmstrstk_init_by_user_v0(
	mmstrstk_t* _out, size_t _object_size,
	void* _user_data,
	MemeString_UserObjectDestruct_t* _destruct_fn,
	MemeString_UserObjectData_t* _data_fn,
	MemeString_UserObjectSize_t* _size_fn)
{
    assert(_out != NULL && "mmstrstk_init_by_user_v0");
    assert(_object_size != 0 && "mmstrstk_init_by_user_v0");
	
	if (_user_data == NULL
		|| _destruct_fn == NULL
		|| _data_fn == NULL
		|| _size_fn == NULL)
		return mmstrstk_init_v0(_out, _object_size);
	
	return MemeStringStack_initTakeOverUserObject(
		_out, _object_size, _user_data, _destruct_fn, _data_fn, _size_fn);
}

MG_CAPI_INLINE int
mmstrstk_init_by_user(
	mmstrstk_t* _out, 
	void* _user_data,
	MemeString_UserObjectDestruct_t* _destruct_fn,
	MemeString_UserObjectData_t* _data_fn,
	MemeString_UserObjectSize_t* _size_fn)
{
    assert(_out != NULL && "mmstrstk_init_by_user");
	
	if (_user_data == NULL
		|| _destruct_fn == NULL
		|| _data_fn == NULL
		|| _size_fn == NULL)
		return mmstrstk_init_v0(_out, MMSTR__OBJ_SIZE);
	
	return MemeStringStack_initTakeOverUserObject(
		_out, MMSTR__OBJ_SIZE, _user_data, _destruct_fn, _data_fn, _size_fn);
}

MG_CAPI_INLINE int
mmstrstk_uninit_v0(mmstrstk_t* _out, size_t _object_size)
{
    assert(_out != NULL && "mmstrstk_uninit_v0");
	
	return MemeStringStack_unInit(_out, _object_size);
}

MG_CAPI_INLINE int
mmstrstk_uninit(mmstrstk_t* _out)
{
    assert(_out != NULL && "mmstrstk_uninit");
	
	return MemeStringStack_unInit(_out, MMSTR__OBJ_SIZE);
}

MG_CAPI_INLINE int
mmstrstk_reset_v0(mmstrstk_t* _out, size_t _object_size)
{
    assert(_out != NULL && "mmstrstk_reset_v0");
	
	return MemeStringStack_reset(_out, _object_size);
}

MG_CAPI_INLINE int
mmstrstk_reset(mmstrstk_t* _out)
{
    assert(_out != NULL && "mmstrstk_reset");
	
	return MemeStringStack_reset(_out, MMSTR__OBJ_SIZE);
}

MG_CAPI_INLINE int
mmstrstk_init_or_reset(mmstrstk_t* _out, mmint_t _obj_size)
{
	assert(_out != NULL   && "mmstrstk_init_or_reset");
	assert(_obj_size != 0 && "mmstrstk_init_or_reset");

	if (_obj_size > 0)
		return mmstrstk_init_v0 (_out, (size_t)_obj_size);
	else
		return mmstrstk_reset_v0(_out, 0);
}

MG_CAPI_INLINE int
mmstrstk_assign_v0(
	mmstrstk_t* _out, size_t _object_size, mmstr_cptr_t _other)
{
    assert(_out != NULL && "mmstrstk_assign");

    if (_other == NULL)
        return mmstrstk_reset_v0(_out, _object_size);
    return MemeStringStack_assign(_out, _object_size, _other);
}

MG_CAPI_INLINE int
mmstrstk_assign(
	mmstrstk_t* _out, mmstr_cptr_t _other)
{
    assert(_out != NULL && "mmstrstk_assign");

    if (_other == NULL)
        return mmstrstk_reset_v0 (_out, MMSTR__OBJ_SIZE);
    return MemeStringStack_assign(_out, MMSTR__OBJ_SIZE, _other);
}

MG_CAPI_INLINE int
mmstr_assign(mmstr_ptr_t _s, mmstr_cptr_t _other)
{
    assert(_s != NULL && "mmstr_assign");
	
	if (_other == NULL)
		return MemeString_reset(_s);
	return MemeString_assign(_s, _other);
}

MG_CAPI_INLINE int
mmstrstk_assign_by_utf8_v0(mmstrstk_t* _s, size_t _obj_size, const mmbyte_t* _utf8, mmint_t _len)
{
    assert(_s != NULL && "mmstrstk_assign_by_utf8_v0");

	if (_utf8 == NULL)
		return mmstrstk_reset_v0(_s, _obj_size);
	return MemeStringStack_assignByU8bytes(_s, _obj_size, _utf8, _len);
}

MG_CAPI_INLINE int
mmstrstk_assign_by_utf8(mmstrstk_t* _s, const mmbyte_t* _utf8, mmint_t _len)
{
    assert(_s != NULL && "mmstrstk_assign_by_utf8");

	if (_utf8 == NULL)
		return mmstrstk_reset_v0(_s, MMSTR__OBJ_SIZE);
	return MemeStringStack_assignByU8bytes(_s, MMSTR__OBJ_SIZE, _utf8, _len);
}

MG_CAPI_INLINE int
mmstrstk_assign_by_buf_v0(mmstrstk_t* _out, size_t _obj_size, mmbuf_cptr_t _other, mmint_t _offset)
{
    assert(_out != NULL && "mmstrstk_assign_by_buf");
	
	if (_other == NULL)
		return mmstrstk_reset_v0(_out, _obj_size);
	return MemeStringStack_assignByBuffer(_out, _obj_size, _other, _offset);
}

MG_CAPI_INLINE int
mmstrstk_assign_by_buf(mmstrstk_t* _out, mmbuf_cptr_t _other, mmint_t _offset)
{
    assert(_out != NULL && "mmstrstk_assign_by_buf");
	
	if (_other == NULL)
		return mmstrstk_reset_v0(_out, MMSTR__OBJ_SIZE);
	return MemeStringStack_assignByBuffer(_out, MMSTR__OBJ_SIZE, _other, _offset);
}

MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_STRING_H_INCLUDED
