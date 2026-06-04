
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
 * @note On failure, @p _out is not modified and remains in an indeterminate state;
 *       do not call MemeStringStack_unInit() on it.
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
 * @note On failure with @c MGEC__INVAL (any pointer argument is @c NULL),
 *       @p _out is not modified and remains in an indeterminate state.
 * @note On failure in the small-buffer copy path (MemeStringStack_initByU8bytes()
 *       returns non-zero), @p _destruct_fn is **not** called and ownership of
 *       @p _user_data is **not** transferred; the caller retains responsibility for it.
 *       @p _out is left in an indeterminate state; do not call MemeStringStack_unInit() on it.
 * @note On failure in the large-buffer path (MemeStringUser_initTakeOver() returns
 *       non-zero), the ownership state of @p _user_data is unspecified; @p _out is
 *       in an indeterminate state and must not be passed to MemeStringStack_unInit().
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
 * @note On failure with @c MGEC__CANCELED or @c MGEC__OPNOTSUPP, @p _out is
 *       not modified and remains in an indeterminate state.
 * @note On allocation failure in the medium copy path, the implementation
 *       explicitly uninitializes @p _out before returning; @p _out is left in
 *       an indeterminate state and must not be passed to MemeStringStack_unInit().
 * @note On all other failures, @p _out is in an indeterminate state;
 *       do not call MemeStringStack_unInit() on it.
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
 * @note On failure with @c MGEC__CANCELED or @c MGEC__OPNOTSUPP, @p _out is
 *       not modified and remains in an indeterminate state.
 * @note When the source buffer uses medium storage and the content-append step
 *       fails, @p _out is explicitly uninitialized before returning; 
 * 		 @p _out is left in an indeterminate state and must not be passed to MemeStringStack_unInit().
 * @note For large/user source storage, if the internal copy fails, @p _out is in
 *       an indeterminate state; do not call MemeStringStack_unInit() on it.
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
 * @note On failure with @c MGEC__INVAL (invalid rune), @p _out is not modified
 *       and remains in an indeterminate state; do not call MemeStringStack_unInit() on it.
 * @note On allocation failure, @p _out is in an indeterminate state;
 *       do not call MemeStringStack_unInit() on it.
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
 * @note On failure, @p _out is in an indeterminate state; do not call
 *       MemeStringStack_unInit() on it.
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
 * @note On failure, @p _out is in an indeterminate state.  In the medium
 *       storage path, if capacity allocation succeeds but a subsequent resize or copy step fails,
 *       @p _out is explicitly uninitialized before returning; it must not be passed to
 *       MemeStringStack_unInit(). For all other failure paths, @p _out is left in an indeterminate state; do not call
 *       MemeStringStack_unInit() on it.
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
 * @note On failure, @p _out is in an indeterminate state; do not call
 *       MemeStringStack_unInit() on it.
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
 * @note On failure, @p _out is in an indeterminate state.  In the medium
 *       storage path, if capacity allocation succeeds but a subsequent conversion or copy step fails,
 * 	     @p _out is explicitly uninitialized before returning; it must not be passed to
 * 	     MemeStringStack_unInit(). For all other failure paths, @p _out is left in an indeterminate state; do not call
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
 * @note On failure, @p _out is in an indeterminate state; do not call
 *       MemeStringStack_unInit() on it.
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
 * @note On failure, @p _out is in an indeterminate state; do not call
 *       MemeStringStack_unInit() on it.
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
 * @note On failure (@c MGEC__OPNOTSUPP — unrecognized storage type), @p _out is
 *       not modified; any resources it held are **not** released.
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
 * @note On failure (@c MGEC__OPNOTSUPP — unrecognized storage type), @p _out is
 *       not modified; it remains initialized in its current (unrecognized) state.
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

/**
 * @brief Assign the content of a MemeBuffer slice to an initialized stack string.
 *
 * Replaces the current content of @p _out with the byte content of @p _other
 * starting at byte offset @p _offset, stripping any trailing null bytes.
 * The function is the assignment counterpart of MemeStringStack_initByBuffer():
 * it first uninitializes @p _out (releasing any heap-allocated storage) and
 * then delegates to MemeStringStack_initByBuffer() to write the new content.
 * The storage tier of the result is chosen automatically based on the
 * effective byte length after applying @p _offset (see MemeStringStack_initByBuffer()
 * for the tier-selection rules).
 *
 * @par Null-source shortcut
 * If @p _other is @c NULL, @p _out is reset to an empty, initialized string
 * via @c mmstrstk_reset_v0() and the function returns @c 0.  No error is
 * reported for a @c NULL source; the caller's object is merely cleared.
 *
 * @par Self-assignment guard
 * If @p _out and @p _other refer to the same memory address, the function
 * returns @c 0 immediately without modifying @p _out.
 *
 * @par Offset clamping
 * Negative @p _offset values are treated as @c 0 by the underlying
 * MemeStringStack_initByBuffer() call.  If the effective byte length after
 * applying @p _offset is zero or negative, @p _out is initialized to an
 * empty string.
 *
 * @par Error handling and state after failure
 * The uninitialization step is performed before the init step, so on failure
 * @p _out is left in an <b>indeterminate</b> state regardless of which step
 * failed.  Callers must <b>not</b> call MemeStringStack_unInit() on @p _out
 * if a non-zero error code is returned.
 * - If MemeStringStack_unInit() itself fails, its error code is returned
 *   immediately and MemeStringStack_initByBuffer() is never called.
 * - If MemeStringStack_initByBuffer() fails, its error code is returned
 *   (see that function's documentation for the per-failure-mode details).
 *
 * @param[in,out] _out         The stack string object to overwrite.  Must be
 *                             initialized on entry.  Must not be @c NULL
 *                             (enforced by assert).  On success, contains the
 *                             new string content.  On failure, left in an
 *                             indeterminate state; do not call
 *                             MemeStringStack_unInit() on it.
 * @param[in]     _object_size Byte size of the caller's @c mmsstk_t instance
 *                             (typically @c sizeof(*_out) or @c MMSTR__OBJ_SIZE).
 * @param[in]     _other       Source buffer whose byte content is copied.
 *                             If @c NULL, @p _out is reset to an empty string
 *                             and @c 0 is returned.  Must not alias @p _out
 *                             (see the self-assignment guard above).
 * @param[in]     _offset      Byte offset into @p _other at which reading begins.
 *                             Negative values are treated as @c 0.
 *
 * @return @c 0 on success (including the @c NULL-source and self-assignment
 *         short-circuit paths), or a non-zero error code on failure:
 *         - Any error code returned by MemeStringStack_unInit().
 *         - @c MGEC__OPNOTSUPP if @p _other uses an unsupported storage type.
 *         - A non-zero allocation error code from MemeStringStack_initByBuffer().
 *
 * @note @p _out must be in a valid initialized state on entry; passing an
 *       uninitialized object leads to undefined behaviour during the
 *       MemeStringStack_unInit() call.
 *
 * @see MemeStringStack_initByBuffer   The initialization counterpart used internally.
 * @see MemeStringStack_unInit         Called to release existing storage before reassignment.
 * @see MemeStringStack_assign         Assign from another MemeString object.
 * @see MemeStringStack_assignByU8bytes Assign from a raw UTF-8 byte array.
 * @see MMSTR__OBJ_SIZE                Canonical macro for the object byte size.
 */
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

/**
 * @brief Extract a byte-range substring from a string stack object.
 *
 * Computes the substring of @p _str that starts at byte offset @p _offset and
 * spans at most @p _count bytes, then writes the result into @p _out.
 *
 * The function chooses the most efficient internal representation:
 * - **Shared storage (zero-copy reference)** — when the source uses shared /
 *   reference-counted storage and the requested range covers the entire string
 *   (@p _offset == 0 and @p _count == source byte length), the output shares
 *   the same underlying buffer.
 * - **Unsafe view (zero-copy slice)** — when the source is a view-type string,
 *   the output is a lightweight view that points directly into the source memory.
 * - **Deep copy** — in all other cases the selected byte range is copied into a
 *   newly allocated string object.
 *
 * @param[in]  _str      Source string stack object.  Must be initialized and
 *                       non-NULL; behavior is undefined otherwise.
 *
 * @param[in]  _offset   Byte offset (0-based) at which the substring begins.
 *                       - Negative values are clamped to @c 0.
 *                       - If @p _offset is greater than or equal to the source
 *                         byte length, @p _out is initialized to an empty string
 *                         and the function returns @c 0.
 *                       @note The offset is measured in raw **bytes**, not Unicode
 *                         code points.  The caller is responsible for ensuring the
 *                         offset falls on a valid UTF-8 character boundary when the
 *                         string contains multi-byte sequences.
 *
 * @param[in]  _count    Number of bytes to include in the substring.
 *                       - A negative value means "from @p _offset to the end of
 *                         the string" (equivalent to source byte length minus
 *                         @p _offset).
 *                       - Values that would extend beyond the end of the source
 *                         are silently clamped to the remaining byte length.
 *                       @note Like @p _offset, this is a **byte** count.  Callers
 *                         must ensure that @c _offset + _count also falls on a
 *                         valid UTF-8 character boundary.
 *
 * @param[out] _out      Destination string stack object that receives the result.
 *                       Must be non-NULL.
 *                       - When @p _obj_size is **positive** (@p _out is
 *                         uninitialized), the function initializes @p _out before
 *                         writing the substring.  On failure, @p _out is in an
 *                         indeterminate state; do not call MemeStringStack_unInit()
 *                         on it.
 *                       - When @p _obj_size is **non-positive** (@p _out is already
 *                         initialized), the function uninitializes @p _out first,
 *                         then attempts to write the new substring into it.  On
 *                         failure, @p _out has already been uninitialized and is
 *                         left in an indeterminate state; do not call
 *                         MemeStringStack_unInit() on it.
 *
 * @param[in]  _obj_size Object size in bytes of the @p _out instance.
 *                       - Pass a **positive** value (typically @c sizeof(*_out))
 *                         when @p _out has not yet been initialized.
 *                       - Pass a **non-positive** value (e.g. @c -1) when @p _out
 *                         is already initialized; the function will query the
 *                         actual object size from the object itself and
 *                         uninitialize it before writing the result.
 *
 * @return @c 0 on success, or a non-zero ::mgec_t error code on failure.
 *
 * @note This function supersedes the deprecated ::MemeStringStack_mid, which
 *       returns the result by value and can cause ABI issues.
 *
 * @see MemeStringStack_mid
 * @see MemeStringStack_initByU8bytes
 */
MEME_API mgec_t
MEME_STDCALL MemeStringStack_mid_v2(
    const mmstrstk_t* _str, mmint_t _offset, mmint_t _count, mmstrstk_t* _out, mmint_t _obj_size);

//! @param _s The string stack object, must be initialized.
//! @param _other The string stack object, must be initialized.
//! @deprecated May cause ABI issues in the future
MEME_API mmstrstk_t
MEME_STDCALL MemeStringStack_concat(
    const mmstrstk_t* _s, size_t _object_size, const mmstrstk_t* _other);

/**
 * @brief Concatenate two stack-allocated strings into an output string.
 *
 * Appends the byte content of @p _other to the byte content of @p _str and
 * writes the result into @p _out.  The function always leaves @p _out in a
 * valid, initialized state: if an error occurs at any stage, @p _out is reset
 * to an empty string before the error code is returned.
 *
 * @par Initialization contract for @p _out
 * The @p _obj_size parameter controls how @p _out is treated on entry:
 * - <b>Positive value</b> – @p _out is treated as <em>uninitialized</em> raw
 *   storage.  @p _obj_size must equal the byte size of the @c mmstrstk_t
 *   object (typically the macro @c MMSTR__OBJ_SIZE).  The function
 *   initializes @p _out internally before writing the result.
 * - <b>Negative value (< 0)</b> – @p _out is treated as <em>already
 *   initialized</em>.  The function reads the object size from @p _out's
 *   internal metadata, uninitializes it, then writes the new concatenated
 *   content.  Passing a negative value when @p _out is not actually
 *   initialized leads to undefined behaviour.
 *
 * @par Empty-string shortcut
 * If the combined byte length of @p _str and @p _other is zero, @p _out is
 * initialized as an empty string and the function returns @c 0 immediately
 * without performing any heap allocation.
 *
 * @par Error handling
 * On failure the function guarantees that @p _out is left in a valid,
 * empty-initialized state so that callers may safely call the corresponding
 * uninit routine unconditionally regardless of the return value.
 *
 * @param[in]  _str      Left-hand operand.  Must be initialized.  Must not be @c NULL.
 * @param[in]  _other    Right-hand operand appended after @p _str.  Must be
 *                       initialized.  Must not be @c NULL.
 * @param[out] _out      Receives the concatenated result.  See the
 *                       initialization contract above for whether @p _out
 *                       must be pre-initialized on entry.  Always left in a
 *                       valid state on return, even on failure.
 * @param[in]  _obj_size Byte size of the @p _out object (e.g. @c MMSTR__OBJ_SIZE),
 *                       or any negative value if @p _out is already initialized
 *                       (the size is then read from the object itself).
 *
 * @return @c 0 on success, or a non-zero @c mgec_t error code on failure.
 *
 * @note This function supersedes the deprecated MemeStringStack_concat(),
 *       which returns the result by value and may cause ABI issues.
 *
 * @see MemeStringStack_concat   Deprecated predecessor (by-value return).
 * @see MMSTR__OBJ_SIZE          Canonical macro for the object byte size.
 */
MEME_API mgec_t
MEME_STDCALL MemeStringStack_concat_v2(
    const mmstrstk_t* _str, const mmstrstk_t* _other, mmstrstk_t* _out, mmint_t _obj_size);

//! @param _s The string stack object, must be initialized.
//! @deprecated May cause ABI issues in the future
MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_toEnUpper(
	const mmsstk_t* _s, size_t _object_size);

/**
 * @brief Convert all ASCII lowercase letters in a stack string to uppercase.
 *
 * Produces a new string that is a byte-for-byte copy of @p _str with every
 * ASCII byte in the range <tt>'a'</tt>–<tt>'z'</tt> (0x61–0x7A) replaced by
 * its uppercase counterpart <tt>'A'</tt>–<tt>'Z'</tt> (0x41–0x5A) via the
 * C standard library function @c toupper().  All other bytes — including
 * multi-byte UTF-8 continuation bytes and any non-ASCII code point — are
 * copied unchanged.
 *
 * @par Scope limitation (ASCII only)
 * The conversion is intentionally restricted to the 26 Basic Latin letters.
 * Non-ASCII characters such as accented Latin letters (e.g. é, ñ), Cyrillic,
 * or CJK code points are @b not uppercased. 
 *
 * @par Implementation strategy
 * The function first copies the entire byte content of @p _str into @p _out
 * via MemeStringStack_initByU8bytes(), then iterates over every byte of
 * @p _out's writable buffer in-place, applying @c toupper() byte by byte.
 *
 * @par Initialization contract for @p _out
 * The @p _obj_size parameter controls how @p _out is treated on entry:
 * - <b>Positive value</b> – @p _out is treated as <em>uninitialized</em> raw
 *   storage.  @p _obj_size must equal the byte size of the @c mmstrstk_t
 *   object (typically the macro @c MMSTR__OBJ_SIZE).  The function
 *   initializes @p _out internally before writing the result.
 * - <b>Non-positive value (≤ 0)</b> – @p _out is treated as <em>already
 *   initialized</em>.  The function reads the object size from @p _out's
 *   internal metadata, uninitializes it, then writes the converted content.
 *   Passing a non-positive value when @p _out is not actually initialized
 *   leads to undefined behaviour.
 *
 * @par Error handling and state after failure
 * If the internal MemeStringStack_initByU8bytes() call fails, the function
 * explicitly reinitializes @p _out to an empty string via
 * @c mmstrstk_init_v0() before returning the error code.  Therefore, on
 * failure @p _out is left in a <b>valid, empty-initialized</b> state and
 * callers may safely call MemeStringStack_unInit() on it unconditionally.
 *
 * @param[in]  _str      Source string to convert.  Must be initialized.
 *                       Must not be @c NULL (enforced by assert).
 * @param[out] _out      Receives the uppercased result.  See the
 *                       initialization contract above.  Always left in a
 *                       valid initialized state on return, even on failure
 *                       (empty string on failure).
 * @param[in]  _obj_size Byte size of the @p _out object
 *                       (e.g. @c MMSTR__OBJ_SIZE), or any non-positive value
 *                       if @p _out is already initialized (the size is then
 *                       read from the object itself).
 *
 * @return @c 0 on success, or a non-zero @c mgec_t error code on failure
 *         (propagated from MemeStringStack_initByU8bytes()).
 *
 * @note This function supersedes the deprecated MemeStringStack_toEnUpper(),
 *       which returns the result by value and may cause ABI issues.
 *
 * @see MemeStringStack_toEnUpper       Deprecated predecessor (by-value return).
 * @see MemeStringStack_toEnLower_v2    Lowercase counterpart.
 * @see MMSTR__OBJ_SIZE                 Canonical macro for the object byte size.
 */
MEME_API mgec_t
MEME_STDCALL MemeStringStack_toEnUpper_v2(
	const mmstrstk_t* _str, mmstrstk_t* _out, mmint_t _obj_size);

//! @param _s The string stack object, must be initialized.
//! @deprecated May cause ABI issues in the future
MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_toEnLower(
	const mmsstk_t* _s, size_t _object_size);

/**
 * @brief Convert all ASCII uppercase letters in a stack string to lowercase.
 *
 * Produces a new string that is a byte-for-byte copy of @p _str with every
 * ASCII byte in the range <tt>'A'</tt>–<tt>'Z'</tt> (0x41–0x5A) replaced by
 * its lowercase counterpart <tt>'a'</tt>–<tt>'z'</tt> (0x61–0x7A) via the
 * C standard library function @c tolower().  All other bytes — including
 * multi-byte UTF-8 continuation bytes and any non-ASCII code point — are
 * copied unchanged.
 *
 * @par Scope limitation (ASCII only)
 * The conversion is intentionally restricted to the 26 Basic Latin letters.
 * Non-ASCII characters such as accented Latin letters (e.g. É, Ñ), Cyrillic,
 * or CJK code points are @b not lowercased. 
 *
 * @par Implementation strategy
 * The function first copies the entire byte content of @p _str into @p _out
 * via MemeStringStack_initByU8bytes(), then iterates over every byte of
 * @p _out's writable buffer in-place, applying @c tolower() byte by byte.
 *
 * @par Initialization contract for @p _out
 * The @p _obj_size parameter controls how @p _out is treated on entry:
 * - <b>Positive value</b> – @p _out is treated as <em>uninitialized</em> raw
 *   storage.  @p _obj_size must equal the byte size of the @c mmstrstk_t
 *   object (typically the macro @c MMSTR__OBJ_SIZE).  The function
 *   initializes @p _out internally before writing the result.
 * - <b>Non-positive value (≤ 0)</b> – @p _out is treated as <em>already
 *   initialized</em>.  The function reads the object size from @p _out's
 *   internal metadata, uninitializes it, then writes the converted content.
 *   Passing a non-positive value when @p _out is not actually initialized
 *   leads to undefined behaviour.
 *
 * @par Error handling and state after failure
 * If the internal MemeStringStack_initByU8bytes() call fails, the function
 * explicitly reinitializes @p _out to an empty string via
 * @c mmstrstk_init_v0() before returning the error code.  Therefore, on
 * failure @p _out is left in a <b>valid, empty-initialized</b> state and
 * callers may safely call MemeStringStack_unInit() on it unconditionally.
 *
 * @param[in]  _str      Source string to convert.  Must be initialized.
 *                       Must not be @c NULL (enforced by assert).
 * @param[out] _out      Receives the lowercased result.  See the
 *                       initialization contract above.  Always left in a
 *                       valid initialized state on return, even on failure
 *                       (empty string on failure).
 * @param[in]  _obj_size Byte size of the @p _out object
 *                       (e.g. @c MMSTR__OBJ_SIZE), or any non-positive value
 *                       if @p _out is already initialized (the size is then
 *                       read from the object itself).
 *
 * @return @c 0 on success, or a non-zero @c mgec_t error code on failure
 *         (propagated from MemeStringStack_initByU8bytes()).
 *
 * @note This function supersedes the deprecated MemeStringStack_toEnLower(),
 *       which returns the result by value and may cause ABI issues.
 *
 * @see MemeStringStack_toEnLower       Deprecated predecessor (by-value return).
 * @see MemeStringStack_toEnUpper_v2    Uppercase counterpart.
 * @see MMSTR__OBJ_SIZE                 Canonical macro for the object byte size.
 */
MEME_API mgec_t
MEME_STDCALL MemeStringStack_toEnLower_v2(
	const mmstrstk_t* _str, mmstrstk_t* _out, mmint_t _obj_size);

//! @param _s The string stack object, must be initialized.
//! @deprecated May cause ABI issues in the future
MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_trimSpace(
    const mmsstk_t* _s, size_t _object_size);

/**
 * @brief Strip leading and trailing Unicode whitespace from a stack string.
 *
 * Scans the UTF-8 byte content of @p _str from both ends, advancing the left
 * pointer forward past each leading whitespace rune and retreating the right
 * pointer backward past each trailing whitespace rune.  The whitespace
 * predicate is @c MemeRuneIndex_isSpace, which recognises all Unicode
 * whitespace code points (U+0009 HORIZONTAL TAB, U+000A LINE FEED,
 * U+000B VERTICAL TAB, U+000C FORM FEED, U+000D CARRIAGE RETURN,
 * U+0020 SPACE, U+00A0 NO-BREAK SPACE, U+3000 IDEOGRAPHIC SPACE, etc.).
 *
 * The resulting substring (from the first non-space rune to the last
 * non-space rune, inclusive) is written to @p _out via
 * @c MemeStringStack_mid_v2.
 *
 * @par Initialization contract for @p _out
 * The @p _obj_size parameter controls how @p _out is treated on entry:
 * - <b>Positive value</b> – @p _out is treated as <em>uninitialized</em> raw
 *   storage.  @p _obj_size must equal the byte size of the @c mmstrstk_t
 *   object (typically the macro @c MMSTR__OBJ_SIZE).  The function
 *   initializes @p _out internally before writing the result.
 * - <b>Negative value (< 0)</b> – @p _out is treated as <em>already
 *   initialized</em>.  The function reads the object size from @p _out's
 *   internal metadata, uninitializes it, then writes the new trimmed content.
 *   Passing a negative value when @p _out is not actually initialized leads
 *   to undefined behaviour.
 *
 * @par Empty / all-whitespace strings
 * If @p _str is empty, or consists entirely of whitespace, @p _out is
 * initialized as an empty string and the function returns @c 0.
 *
 * @par Error handling
 * This function delegates the final write to @c MemeStringStack_mid_v2, which
 * inherits that function's failure contract: on failure @p _out is left in an
 * <b>indeterminate</b> state.  Do <b>not</b> call @c MemeStringStack_unInit()
 * on @p _out if the function returns a non-zero error code.
 *
 * @par Invalid UTF-8
 * If a negative rune size is returned by the UTF-8 rune-size helper while
 * scanning either end, the scan stops immediately at that position.  The
 * resulting slice is then built from whatever valid prefix/suffix was found
 * up to that point.
 *
 * @param[in]  _str      The source string to trim.  Must be initialized.
 *                       Must not be @c NULL.
 * @param[out] _out      Receives the trimmed result.  See the initialization
 *                       contract above for whether @p _out must be
 *                       pre-initialized on entry.  On failure, @p _out is in
 *                       an indeterminate state; do not call
 *                       @c MemeStringStack_unInit() on it.
 * @param[in]  _obj_size Byte size of the @p _out object
 *                       (e.g. @c MMSTR__OBJ_SIZE), or any negative value if
 *                       @p _out is already initialized (the size is then read
 *                       from the object itself).
 *
 * @return @c 0 on success, or a non-zero @c mgec_t error code on failure.
 *
 * @note This function supersedes the deprecated MemeStringStack_trimSpace(),
 *       which returns the result by value and may cause ABI issues.
 *
 * @see MemeStringStack_trimSpace        Deprecated predecessor (by-value return).
 * @see MemeStringStack_trimLeftSpace_v2 Strip leading whitespace only.
 * @see MemeStringStack_trimRightSpace_v2 Strip trailing whitespace only.
 * @see MemeRuneIndex_isSpace            Whitespace predicate used for scanning.
 * @see MMSTR__OBJ_SIZE                  Canonical macro for the object byte size.
 */
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

/**
 * @brief Return a copy of a stack string with a given byte prefix removed.
 *
 * Checks whether @p _str begins with the byte sequence [@p _prefix,
 * @p _prefix + @p _prefix_len).  The comparison is a raw byte comparison
 * (@c memcmp) and is therefore case-sensitive and encoding-agnostic.
 *
 * - If the prefix is found, @p _out receives the substring that starts
 *   immediately after the prefix and extends to the end of @p _str
 *   (equivalent to @c MemeStringStack_mid_v2 with @p _offset = @p _prefix_len
 *   and @p _count = @c -1).
 * - If the prefix is <b>not</b> found, @p _out receives a full copy of
 *   @p _str unchanged.  No error is reported in this case.
 *
 * @par Prefix-length auto-detection
 * If @p _prefix_len is negative, the function computes the length via
 * @c strlen(@p _prefix).  @p _prefix must then be a valid null-terminated
 * C string; passing @c NULL with a negative length is undefined behaviour.
 * If @p _prefix_len is @c 0, no prefix comparison is performed and the
 * function always returns a copy of @p _str.
 *
 * @par Initialization contract for @p _out
 * The @p _obj_size parameter controls how @p _out is treated on entry:
 * - <b>Positive value</b> – @p _out is treated as <em>uninitialized</em> raw
 *   storage.  @p _obj_size must equal the byte size of the @c mmstrstk_t
 *   object (typically the macro @c MMSTR__OBJ_SIZE).
 * - <b>Non-positive value (≤ 0)</b> – @p _out is treated as <em>already
 *   initialized</em>.  The function reads the object size from @p _out's
 *   internal metadata and uninitializes it before writing the result.
 *   Passing a non-positive value when @p _out is not actually initialized
 *   leads to undefined behaviour.
 *
 * @par Error handling and state after failure
 * On failure, @p _out is left in an <b>indeterminate</b> state.  Do
 * <b>not</b> call MemeStringStack_unInit() on @p _out if a non-zero error
 * code is returned.  The failure contract is inherited from whichever
 * internal function is called:
 * - MemeStringStack_mid_v2() in the prefix-found path.
 * - MemeStringStack_initByOther() or MemeStringViewUnsafeStack_initByOther()
 *   in the no-match path.
 *
 * @param[in]  _str        Source string to inspect.  Must be initialized.
 *                         Must not be @c NULL (enforced by assert).
 * @param[in]  _prefix     Pointer to the byte sequence to look for at the
 *                         start of @p _str.  May be @c NULL only when
 *                         @p _prefix_len is @c 0.
 * @param[in]  _prefix_len Number of bytes in @p _prefix.  A negative value
 *                         causes the length to be computed via
 *                         @c strlen(@p _prefix).
 * @param[out] _out        Receives the result (prefix stripped, or full copy).
 *                         See the initialization contract above.  On failure,
 *                         left in an indeterminate state; do not call
 *                         MemeStringStack_unInit() on it.
 * @param[in]  _obj_size   Byte size of the @p _out object
 *                         (e.g. @c MMSTR__OBJ_SIZE), or any non-positive value
 *                         if @p _out is already initialized.
 *
 * @return @c 0 on success (prefix found or not found), or a non-zero
 *         @c mgec_t error code propagated from the internal copy/slice call.
 *
 * @see MemeStringStack_trimSuffix    Remove a matching suffix instead.
 * @see MemeStringStack_mid_v2        Used internally when the prefix matches.
 * @see MMSTR__OBJ_SIZE               Canonical macro for the object byte size.
 */
MEME_API mgec_t
MEME_STDCALL MemeStringStack_trimPrefix(
    const mmstrstk_t* _str, const char* _prefix, mmint_t _prefix_len, mmstrstk_t* _out, mmint_t _obj_size);

/**
 * @brief Return a copy of a stack string with a given byte suffix removed.
 *
 * Checks whether @p _str ends with the byte sequence [@p _suffix,
 * @p _suffix + @p _suffix_len).  The comparison is a raw byte comparison
 * (@c memcmp) and is therefore case-sensitive and encoding-agnostic.
 *
 * - If the suffix is found, @p _out receives the leading substring of
 *   @p _str up to (but not including) the suffix bytes (equivalent to
 *   @c MemeStringStack_mid_v2 with @p _offset = @c 0 and
 *   @p _count = source byte length − @p _suffix_len).
 * - If the suffix is <b>not</b> found, @p _out receives a full copy of
 *   @p _str unchanged.  No error is reported in this case.
 *
 * @par Suffix-length auto-detection
 * If @p _suffix_len is negative, the function computes the length via
 * @c strlen(@p _suffix).  @p _suffix must then be a valid null-terminated
 * C string; passing @c NULL with a negative length is undefined behaviour.
 * If @p _suffix_len is @c 0, no suffix comparison is performed and the
 * function always returns a copy of @p _str.
 *
 * @par Initialization contract for @p _out
 * The @p _obj_size parameter controls how @p _out is treated on entry:
 * - <b>Positive value</b> – @p _out is treated as <em>uninitialized</em> raw
 *   storage.  @p _obj_size must equal the byte size of the @c mmstrstk_t
 *   object (typically the macro @c MMSTR__OBJ_SIZE).
 * - <b>Non-positive value (≤ 0)</b> – @p _out is treated as <em>already
 *   initialized</em>.  The function reads the object size from @p _out's
 *   internal metadata and uninitializes it before writing the result.
 *   Passing a non-positive value when @p _out is not actually initialized
 *   leads to undefined behaviour.
 *
 * @par Error handling and state after failure
 * On failure, @p _out is left in an <b>indeterminate</b> state.  Do
 * <b>not</b> call MemeStringStack_unInit() on @p _out if a non-zero error
 * code is returned.  The failure contract is inherited from whichever
 * internal function is called:
 * - MemeStringStack_mid_v2() in the suffix-found path.
 * - MemeStringStack_initByOther() or MemeStringViewUnsafeStack_initByOther()
 *   in the no-match path.
 *
 * @param[in]  _str        Source string to inspect.  Must be initialized.
 *                         Must not be @c NULL (enforced by assert).
 * @param[in]  _suffix     Pointer to the byte sequence to look for at the
 *                         end of @p _str.  May be @c NULL only when
 *                         @p _suffix_len is @c 0.
 * @param[in]  _suffix_len Number of bytes in @p _suffix.  A negative value
 *                         causes the length to be computed via
 *                         @c strlen(@p _suffix).
 * @param[out] _out        Receives the result (suffix stripped, or full copy).
 *                         See the initialization contract above.  On failure,
 *                         left in an indeterminate state; do not call
 *                         MemeStringStack_unInit() on it.
 * @param[in]  _obj_size   Byte size of the @p _out object
 *                         (e.g. @c MMSTR__OBJ_SIZE), or any non-positive value
 *                         if @p _out is already initialized.
 *
 * @return @c 0 on success (suffix found or not found), or a non-zero
 *         @c mgec_t error code propagated from the internal copy/slice call.
 *
 * @see MemeStringStack_trimPrefix    Remove a matching prefix instead.
 * @see MemeStringStack_mid_v2        Used internally when the suffix matches.
 * @see MMSTR__OBJ_SIZE               Canonical macro for the object byte size.
 */
MEME_API mgec_t
MEME_STDCALL MemeStringStack_trimSuffix(
    const mmstrstk_t* _str, const char* _suffix, mmint_t _suffix_len, mmstrstk_t* _out, mmint_t _obj_size);

//! \code
//! MemeStringStack_t s = MemeStringStack_getRepeat(MEME_STRING__OBJECT_SIZE, 3, "abc", 3);
//! // s is "abcabcabc";
//! \endcode
//! @param _s The string stack object, must be initialized.
//! @deprecated May cause ABI issues in the future
MEME_API mmsstk_t
MEME_STDCALL MemeStringStack_getRepeat(
	size_t _object_size, mmint_t _count, const char* _s, mmint_t _len);

/**
 * @brief Construct a string by repeating a byte sequence a given number of times.
 *
 * Appends the byte sequence [@p _in, @p _in + @p _in_len) to an internal
 * variable buffer @p _count times, then transfers the accumulated content to
 * @p _out via MemeVariableBuffer_releaseToString() (a move, not a copy).
 *
 * @par Zero or negative repeat count
 * If @p _count is zero or negative the append loop is never entered.
 * @p _out is initialized to an empty string and @c 0 is returned.
 *
 * @par Input-length semantics
 * If @p _in_len is negative, the function calls @c strlen(@p _in) once before
 * the repeat loop (yielding 0 when @p _in is @c NULL) so callers may
 * conveniently pass @c -1 for NUL-terminated strings.
 * If @p _in_len is zero, no bytes are appended and @p _out is an empty string.
 * If @p _in_len is positive, exactly that many bytes are appended per repetition.
 *
 * @par Null input pointer
 * @p _in may be @c NULL when @p _count ≤ 0, @p _in_len == 0, or
 * @p _in_len < 0 (the negative-length path guards the @c strlen() call).
 *
 * @par Initialization contract for @p _out
 * The @p _obj_size parameter controls how @p _out is treated on entry:
 * - <b>Positive value</b> – @p _out is treated as <em>uninitialized</em> raw
 *   storage.  @p _obj_size must equal the byte size of the @c mmstrstk_t
 *   object (typically the macro @c MMSTR__OBJ_SIZE).
 * - <b>Non-positive value (≤ 0)</b> – @p _out is treated as <em>already
 *   initialized</em>.  The function reads the object size from @p _out's
 *   internal metadata and uninitializes it before building the result.
 *   Passing a non-positive value when @p _out is not actually initialized
 *   leads to undefined behaviour.
 *
 * @par Error handling and state after failure
 * Every failure path explicitly calls @c mmstrstk_init_v0() on @p _out before
 * returning.  Therefore on failure @p _out is always left in a <b>valid,
 * empty-initialized</b> state and callers may safely call
 * MemeStringStack_unInit() on it unconditionally.  Possible failures:
 * - Internal variable-buffer initialization fails.
 * - Any MemeVariableBuffer_appendWithBytes() call during the repeat loop fails
 *   (e.g. allocation failure for a large total length).
 * - MemeVariableBuffer_releaseToString() fails after the loop.
 *
 * @param[out] _out      Receives the repeated string.  See the initialization
 *                       contract above.  Always left in a valid initialized
 *                       state on return (empty string on failure).
 *                       Must not be @c NULL (enforced by assert).
 * @param[in]  _obj_size Byte size of the @p _out object
 *                       (e.g. @c MMSTR__OBJ_SIZE), or any non-positive value
 *                       if @p _out is already initialized.
 * @param[in]  _count    Number of times to repeat @p _in.  Values ≤ 0
 *                       produce an empty string.
 * @param[in]  _in       Pointer to the byte sequence to repeat.  May be
 *                       @c NULL when @p _count ≤ 0, @p _in_len == 0, or
 *                       @p _in_len < 0 (NULL is guarded before strlen()).
 * @param[in]  _in_len   Number of bytes in @p _in to append per repetition.
 *                       Pass @c 0 to produce an empty string.  A negative
 *                       value delegates length computation to
 *                       @c strlen() once before the repeat loop.
 *
 * @return @c 0 on success, or a non-zero @c mgec_t error code on failure.
 *         @p _out is always left initialized (empty) on failure.
 *
 * @note This function supersedes the deprecated MemeStringStack_getRepeat(),
 *       which returns the result by value and may cause ABI issues.
 *
 * @par Example
 * @code
 * mmstrstk_t s;
 * MemeStringStack_getRepeat_v2(&s, MMSTR__OBJ_SIZE, 3, "abc", 3);
 * // s contains "abcabcabc"
 * mmstrstk_uninit_v0(&s, MMSTR__OBJ_SIZE);
 * @endcode
 *
 * @see MemeStringStack_getRepeat        Deprecated predecessor (by-value return).
 * @see MemeVariableBuffer_appendWithBytes  Append primitive used in the loop.
 * @see MemeVariableBuffer_releaseToString  Move used to produce the final string.
 * @see MMSTR__OBJ_SIZE                  Canonical macro for the object byte size.
 */
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

/**
 * @brief Replace non-overlapping occurrences of a byte pattern in a string,
 *        writing the result into a caller-supplied stack object.
 *
 * Scans @p _str for non-overlapping occurrences of the byte sequence @p _from
 * and replaces each with @p _to, performing at most @p _max_count substitutions.
 * The result is written to @p _out via MemeVariableBuffer_releaseToString()
 * (a zero-copy move, not a heap copy).
 *
 * @par Empty pattern
 * If @p _from_len resolves to zero (passed as 0 or @p _from is an empty
 * string), the function copies @p _str into @p _out unchanged via
 * MemeStringStack_initByOther() and returns immediately without scanning.
 *
 * @par Replacement count limit
 * @p _max_count controls how many substitutions are performed:
 * - <b>Positive</b> – at most @p _max_count occurrences are replaced; bytes
 *   after the last replacement are appended to @p _out unchanged.
 * - <b>Zero or negative</b> – every occurrence is replaced (unlimited).
 *
 * @par Length semantics
 * Both @p _from_len and @p _to_len are resolved once before scanning begins:
 * - Negative value → @c strlen() is called once on the respective pointer.
 * - Zero → the pattern or replacement contributes no bytes.
 * - Positive → exactly that many bytes are used.
 *
 * @par Initialization contract for @p _out
 * @p _obj_size controls how @p _out is treated on entry:
 * - <b>Positive value</b> – @p _out is treated as <em>uninitialized</em> raw
 *   storage.  @p _obj_size must equal the byte size of the @c mmstrstk_t
 *   object (typically @c MMSTR__OBJ_SIZE).
 * - <b>Non-positive value (≤ 0)</b> – @p _out is treated as <em>already
 *   initialized</em>.  The function reads the object size from @p _out's
 *   internal metadata and uninitializes it before building the result.
 *   Passing a non-positive value when @p _out is not actually initialized
 *   leads to undefined behaviour.
 *
 * @par Error handling and state after failure
 * Every failure path calls @c MemeStringStack_init() on @p _out before
 * returning, leaving it in a valid, empty-initialized state.  Callers may
 * therefore call MemeStringStack_unInit() unconditionally after failure.
 * Possible failures:
 * - MemeVariableBufferStack_init() fails (internal buffer setup).
 * - MemeVariableBuffer_appendWithBytes() fails during the replacement pass
 *   (e.g. allocation failure for a large result).
 * - MemeVariableBuffer_releaseToString() fails after the loop.
 *
 * @param[in]  _str       Input string.  Must be initialized.
 *                        Must not be @c NULL (enforced by assert).
 * @param[in]  _from      Byte pattern to search for.  Must not be @c NULL
 *                        when @p _from_len resolves to a positive value.
 * @param[in]  _from_len  Byte length of @p _from.  Negative → resolved via
 *                        @c strlen().  Zero → @p _str is copied unchanged.
 * @param[in]  _to        Replacement byte sequence.  May be @c NULL when
 *                        @p _to_len resolves to zero.
 * @param[in]  _to_len    Byte length of @p _to.  Negative → resolved via
 *                        @c strlen().  Zero → matched patterns are deleted.
 * @param[in]  _max_count Maximum number of replacements.  ≤ 0 means unlimited.
 * @param[out] _out       Receives the result.  See the initialization contract
 *                        above.  Always left in a valid initialized state on
 *                        return (empty string on failure).
 * @param[in]  _obj_size  Byte size of the @p _out object
 *                        (e.g. @c MMSTR__OBJ_SIZE), or any non-positive value
 *                        if @p _out is already initialized.
 *
 * @return @c 0 on success, or a non-zero @c mgec_t error code on failure.
 *
 * @see MemeStringStack_replace           Deprecated predecessor (by-value return).
 * @see MemeVariableBuffer_appendWithBytes Append primitive used in the build pass.
 * @see MemeVariableBuffer_releaseToString Move used to produce the final string.
 * @see MMSTR__OBJ_SIZE                   Canonical macro for the object byte size.
 */
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

/**
 * @brief Truncate a string to its longest valid UTF-8 prefix and store the
 *        result in a caller-supplied stack object.
 *
 * Scans the raw bytes of @p _str using mmutf_u8valid() to locate the byte
 * offset @c pos of the first ill-formed UTF-8 sequence.  The bytes in the
 * range <tt>[0, pos)</tt> — guaranteed to form a well-formed UTF-8 string —
 * are then extracted into @p _out via MemeStringStack_mid_v2().
 *
 * @par Truncation semantics
 * This function implements a <b>truncation</b> strategy, not a replacement
 * strategy.  Invalid bytes and everything after the first invalid byte are
 * silently discarded.  No replacement character (U+FFFD) is inserted.
 * - If @p _str is entirely valid UTF-8, the full content is copied unchanged
 *   (@c pos equals the byte length, so mid_v2 returns the whole string).
 * - If the very first byte is invalid (@c pos == 0), @p _out is initialized
 *   to an empty string.
 *
 * @par UTF-8 converter
 * The validity scan is performed by the currently active UTF-8 converter,
 * selected via mmutf_get_best_converter().  The default converter validates
 * sequences code-point by code-point using mmutf_u8rune_valid().
 *
 * @par Initialization contract for @p _out
 * @p _obj_size controls how @p _out is treated on entry:
 * - <b>Positive value</b> – @p _out is treated as <em>uninitialized</em> raw
 *   storage.  @p _obj_size must equal the byte size of the @c mmstrstk_t
 *   object (typically @c MMSTR__OBJ_SIZE).
 * - <b>Non-positive value (≤ 0)</b> – @p _out is treated as <em>already
 *   initialized</em>.  The function reads the object size from @p _out's
 *   internal metadata and uninitializes it before writing the result.
 *   Passing a non-positive value when @p _out is not actually initialized
 *   leads to undefined behaviour.
 *
 * @par Error handling and state after failure
 * This function delegates the final write to MemeStringStack_mid_v2(), whose
 * failure contract applies here:
 * - On failure, @p _out is left in an <b>indeterminate</b> state.
 * - <b>Do not</b> call MemeStringStack_unInit() on @p _out if a non-zero
 *   error code is returned.
 *
 * @param[in]  _str      Input string.  Must be initialized.
 *                       Must not be @c NULL (enforced by assert).
 * @param[out] _out      Destination stack object that receives the valid UTF-8
 *                       prefix.  Must not be @c NULL; returns @c MGEC__INVAL
 *                       otherwise.  See the initialization contract above.
 * @param[in]  _obj_size Byte size of the @p _out object
 *                       (e.g. @c MMSTR__OBJ_SIZE), or any non-positive value
 *                       if @p _out is already initialized.
 *
 * @return @c 0 on success, @c MGEC__INVAL if @p _out is @c NULL, or another
 *         non-zero @c mgec_t error code propagated from MemeStringStack_mid_v2().
 *
 * @see MemeStringStack_toValidUtf8    Deprecated predecessor (by-value return).
 * @see mmutf_u8valid                  UTF-8 validity scan used internally.
 * @see MemeStringStack_mid_v2         Substring extraction used to build the result.
 * @see MMSTR__OBJ_SIZE                Canonical macro for the object byte size.
 */
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

/**
 * @brief Convert a stack string by applying a mapping function to each Unicode rune.
 *
 * Iterates over every UTF-8 encoded rune in @p _str. For each rune, @p _mapping_fn
 * is called with a pointer to the decoded rune and @p _user_data. The callback may
 * modify the rune in-place. If the callback returns a value less than 0, the rune
 * is discarded (skipped) in the output; otherwise the (possibly modified) rune is
 * appended to the result. Runes that are invalid after the callback are also
 * silently discarded.
 *
 * On success the result is written to @p _out. On failure @p _out is left in an
 * initialised (empty) state.
 *
 * @param _str        Input stack string to convert. Must not be NULL.
 * @param _mapping_fn Mapping callback invoked for each rune. Must not be NULL.
 *                    Signature: @code int fn(MemeRune_t* rune, void* user_data) @endcode
 *                    A return value < 0 causes the rune to be skipped entirely.
 * @param _user_data  Arbitrary context pointer forwarded to every @p _mapping_fn call.
 * @param _out        Output stack string that receives the converted result.
 *                    Must not be NULL.
 * @param _obj_size   Size in bytes of the stack string object pointed to by @p _out.
 *                    Pass 0 to have the function infer the size from the registered
 *                    default.
 *
 * @return 0 on success, or a negative @c mgec_t error code on failure.
 */
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

/**
 * @brief Format a stack string from a printf-style format string and a @c va_list,
 *        with an optional byte limit and a pre-allocation size hint.
 *
 * Produces a formatted string equivalent to @c vsnprintf and stores the result in
 * the MemeString stack object @p _str. Two formatting strategies are available
 * depending on @p _pre_size:
 *
 * - If @p _pre_size <= 0: the required byte length is first computed by calling
 *   @c vsnprintf(NULL, 0, ...), then a buffer of exactly that size is allocated
 *   and the format string is rendered into it (two-pass approach).
 * - If @p _pre_size > 0: a buffer of @p _pre_size bytes is allocated upfront and
 *   @c vsnprintf is attempted immediately. If the formatted output fits within
 *   @p _pre_size bytes, no second pass is needed. If it does not fit, the function
 *   falls back to the two-pass approach automatically.
 *
 * If @p _size_limit > 0, the stored result is silently truncated to at most
 * @p _size_limit bytes (excluding the NUL terminator). If @p _format is NULL the
 * function returns 0 without writing anything to @p _str.
 *
 * @param _str         Output stack string. Must not be NULL.
 * @param _object_size Size in bytes of the @p _str object. Pass 0 to infer from
 *                     the registered default.
 * @param _size_limit  Maximum number of bytes to store in the result, excluding the
 *                     NUL terminator. Pass <= 0 for no limit.
 * @param _pre_size    Pre-allocation hint in bytes. When > 0 the function reserves
 *                     this many bytes upfront to avoid a two-pass @c vsnprintf call.
 *                     Pass <= 0 to always use the two-pass approach.
 * @param _format      Standard C printf-style format string. May be NULL, in which
 *                     case the function returns 0 immediately.
 * @param _args        Argument list matching the conversion specifiers in @p _format.
 *
 * @return 0 on success, or a negative @c mgec_t error code on failure:
 *         @li @c MGEC__INVAL — @p _str is NULL.
 *         @li @c MGEC__ERR  — @c vsnprintf reported a format error (returned < 0 on
 *             the dry-run pass or <= 0 on the final render pass).
 *         @li Other negative codes — internal buffer allocation or resize failed.
 *
 * @warning On failure the state of @p _str is indeterminate. When @p _object_size
 *          is <= 0 on entry, the object is uninitialised before the error is
 *          detected; it must be re-initialised before further use. When
 *          @p _object_size is > 0, @p _str is left unchanged on most failure
 *          paths (before the final @c releaseToString step), but this is not
 *          guaranteed. Callers should re-initialise @p _str after any failure.
 */
MEME_API mgec_t MEME_STDCALL
MemeStringStack_vformatWithLimitInCstyle_v2(
	mmstrstk_t* _str,
	mmint_t _object_size,
	mmint_t _size_limit,
	mmint_t _pre_size,
	MG_SYM__MSVC_FMT_STR(const char* _format),
	va_list _args);

/**
 * @brief Format a stack string from a printf-style format string with variadic
 *        arguments, an optional byte limit, and a pre-allocation size hint.
 *
 * Variadic wrapper around MemeStringStack_vformatWithLimitInCstyle_v2(). Accepts
 * the same parameters but takes @c ... instead of a @c va_list. Refer to that
 * function for a complete description of the formatting behaviour, the
 * @p _size_limit truncation semantics, and the @p _pre_size optimisation hint.
 *
 * @param _str         Output stack string. Must not be NULL.
 * @param _object_size Size in bytes of the @p _str object. Pass 0 to infer from
 *                     the registered default.
 * @param _size_limit  Maximum number of bytes to store in the result, excluding the
 *                     NUL terminator. Pass <= 0 for no limit.
 * @param _pre_size    Pre-allocation hint in bytes. When > 0 the function reserves
 *                     this many bytes upfront to avoid a two-pass @c vsnprintf call.
 *                     Pass <= 0 to always use the two-pass approach.
 * @param _format      Standard C printf-style format string. Must not be NULL.
 * @param ...          Format arguments corresponding to the conversion specifiers
 *                     in @p _format.
 *
 * @return 0 on success, or a negative @c mgec_t error code on failure.
 *         See MemeStringStack_vformatWithLimitInCstyle_v2() for the full list of
 *         error codes and the state of @p _str on failure.
 */
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

/**
 * @brief Join an array of stack strings into a single stack string, inserting a
 *        separator between consecutive elements.
 *
 * Concatenates @p _item_count strings from the flat array @p _items, placing the
 * @p _separator byte sequence between each adjacent pair. The separator is not
 * prepended before the first element nor appended after the last element. If
 * @p _item_count is 0 the result is an empty string.
 *
 * The array @p _items is traversed as a flat byte array whose element stride is
 * computed from the registered size of the first element
 * (@c MemeStringStack_regSize(_items) * sizeof(mmint_t)), so every element must
 * be a stack string of the same registered size.
 *
 * On success the joined string is written to @p _str. On failure @p _str is left
 * in an initialised (empty) state.
 *
 * @param _str           Output stack string. Must not be NULL.
 * @param _obj_size      Size in bytes of the @p _str object. Pass 0 to infer from
 *                       the registered default.
 * @param _separator     UTF-8 byte sequence to insert between elements. May be NULL,
 *                       which is treated as an empty separator (no bytes inserted).
 * @param _separator_len Byte length of @p _separator. Pass a negative value to have
 *                       the length computed automatically via @c strlen.
 * @param _items         Pointer to the first element of the input array. May be NULL
 *                       only when @p _item_count is 0.
 * @param _item_count    Number of elements in @p _items. Pass 0 to produce an empty
 *                       result string without touching @p _items.
 *
 * @return 0 on success, or a negative @c mgec_t error code on failure.
 */
MEME_API mgec_t
MEME_STDCALL MemeStringStack_join(
	mmstrstk_t* _str, mmint_t _obj_size, const char* _separator, mmint_t _separator_len,
	const mmstrstk_t* _items, mmint_t _item_count);


/**
 * @brief Split a stack string into substrings at every occurrence of a delimiter
 *        key, writing results into a caller-provided output array.
 *
 * Searches @p _s for occurrences of @p _key starting at the byte offset stored in
 * @p *_search_index (or from offset 0 when @p _search_index is NULL). Each
 * substring between consecutive delimiter matches is stored as a newly initialised
 * stack string in @p _out. At most @p *_out_count substrings are written per call;
 * when the string contains more parts than the output array can hold, the function
 * fills @p *_out_count entries, updates @p *_search_index to the byte offset of
 * the next unprocessed position, and returns so the caller can continue with a
 * subsequent call using the same @p _out array.
 *
 * Empty-part handling is governed by @p _behavior:
 * - @c MemeFlag_KeepEmptyParts: empty substrings produced by adjacent delimiters
 *   or a leading/trailing delimiter are written as empty stack strings.
 * - @c MemeFlag_SkipEmptyParts: such zero-length segments are silently discarded.
 *
 * Each successfully written element in @p _out is initialised by this function and
 * must eventually be released by the caller. On failure, all already-written
 * elements are released before returning.
 *
 * @param _s            Input stack string to split. Must not be NULL.
 * @param _key          Delimiter byte sequence (UTF-8). Must not be NULL.
 * @param _key_len      Byte length of @p _key. Pass a negative value to have the
 *                      length computed automatically via @c strlen.
 * @param _behavior     Controls how empty substrings are handled:
 *                      @c MemeFlag_KeepEmptyParts or @c MemeFlag_SkipEmptyParts.
 * @param _sensitivity  Case-sensitivity mode for delimiter matching, e.g.
 *                      @c MemeFlag_CaseSensitive or @c MemeFlag_AllInsensitive.
 * @param _out          Caller-allocated array of stack string objects that receives
 *                      the split substrings. Must not be NULL.
 * @param _obj_size     Size in bytes of each element in @p _out.
 * @param _out_count    In/out parameter. On entry: the capacity of @p _out (maximum
 *                      number of substrings to write). On return: the actual number
 *                      of substrings written. Must not be NULL; @p *_out_count must
 *                      be >= 1 on entry.
 * @param _search_index In/out parameter for incremental splitting. On entry: the
 *                      byte offset in @p _s at which to resume (use 0 for the first
 *                      call). On return: the byte offset of the next unprocessed
 *                      position, or -1 when the entire string has been consumed.
 *                      May be NULL, which is equivalent to always starting from 0
 *                      with no resumption support.
 *
 * @return 0 on success, or a negative @c mgec_t error code on failure
 *         (e.g., @c MGEC__INVAL if @p *_out_count < 1).
 */
MEME_API mmint_t
MEME_STDCALL MemeStringStack_split(
	const mmstrstk_t* _s,
	const char* _key, mmint_t _key_len,
	mmflag_split_behav_t, mmflag_case_sensit_t,
	mmstrstk_t* MEGO_SYMBOL__RESTRICT _out, mmint_t _obj_size,
	mmint_t* MEGO_SYMBOL__RESTRICT _out_count,
	mmint_t* MEGO_SYMBOL__RESTRICT _search_index
);

/**
 * @brief Split a stack string into substrings at every byte that satisfies a
 *        user-supplied predicate, writing results into a caller-provided output array.
 *
 * Searches @p _str byte-by-byte starting from the offset stored in
 * @p *_search_index (or from offset 0 when @p _search_index is NULL). Whenever
 * @p _cond_func returns a non-zero value for a byte, that byte position is treated
 * as a single-byte delimiter. The substring from the end of the previous delimiter
 * up to (but not including) the current delimiter byte is stored as a newly
 * initialised stack string in @p _out. Zero-length segments produced by adjacent
 * delimiter bytes are always skipped (never written to @p _out).
 *
 * At most @p *_out_count substrings are written per call. When the string contains
 * more parts than the output array can hold, the function fills @p *_out_count
 * entries, updates @p *_search_index to the byte offset of the next unprocessed
 * position, and returns so the caller can continue with a subsequent call.
 *
 * @note Unlike MemeStringStack_split(), which matches a fixed multi-byte key, this
 *       function evaluates one byte at a time and is well-suited for splitting on
 *       character classes (e.g., whitespace, punctuation, or any custom criterion).
 *
 * Each successfully written element in @p _out is initialised by this function and
 * must eventually be released by the caller. On failure, all already-written
 * elements are released before returning.
 *
 * @param _str          Input stack string to split. Must not be NULL.
 * @param _cond_func    Byte predicate callback. Must not be NULL.
 *                      Signature: @code int fn(MemeByte_t ch, void* user_data) @endcode
 *                      Return non-zero to treat @p ch as a delimiter byte.
 * @param _user_data    Arbitrary context pointer forwarded to every @p _cond_func call.
 * @param _out          Caller-allocated array of stack string objects that receives
 *                      the split substrings. Must not be NULL.
 * @param _obj_size     Size in bytes of each element in @p _out.
 * @param _out_count    In/out parameter. On entry: the capacity of @p _out (maximum
 *                      number of substrings to write). On return: the actual number
 *                      of substrings written. Must not be NULL; @p *_out_count must
 *                      be >= 1 on entry.
 * @param _search_index In/out parameter for incremental splitting. On entry: the
 *                      byte offset in @p _str at which to resume (use 0 for the
 *                      first call). On return: the byte offset of the next
 *                      unprocessed position, or -1 when the entire string has been
 *                      consumed. May be NULL, which is equivalent to always starting
 *                      from 0 with no resumption support.
 *
 * @return 0 on success, or a negative @c mgec_t error code on failure
 *         (e.g., @c MGEC__INVAL if @p *_out_count < 1).
 */
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
//! @deprecated May cause ABI issues in the future
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
