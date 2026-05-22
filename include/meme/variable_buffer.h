
#ifndef MEME_VARIABLE_BUFFER_H_INCLUDED
#define MEME_VARIABLE_BUFFER_H_INCLUDED

#include "variable_buffer_fwd.h"

#include <mego/predef/symbol/restrict.h>
#include <mego/err/ec.h>

MEME_EXTERN_C_SCOPE_START

MEME_API int
MEME_STDCALL MemeVariableBufferStack_init(mmvbstk_t* _out, size_t _object_size);

MEME_API int
MEME_STDCALL MemeVariableBufferStack_initByOther(
	mmvbstk_t* _out, size_t _object_size, const mmvbstk_t* _other);

MEME_API int
MEME_STDCALL MemeVariableBufferStack_initByBytes(
	mmvbstk_t* _out, size_t _object_size, const MemeByte_t* _buf, MemeInteger_t _len);

MEME_API int
MEME_STDCALL MemeVariableBufferStack_initWithRepeatBytes(
	mmvbstk_t* _out, size_t _object_size, MemeInteger_t _count, MemeByte_t _byte);

//! @param _out  must be initialized
MEME_API int
MEME_STDCALL MemeVariableBufferStack_unInit(mmvbstk_t* _out, size_t _object_size);

//! @param _out  must be initialized
MEME_API int
MEME_STDCALL MemeVariableBufferStack_reset(
	MemeVariableBufferStack_t* _out, size_t _object_size);

//! @param _s     must be initialized
//! @param _other must be initialized
MEME_API int
MEME_STDCALL MemeVariableBufferStack_assign(
	mmvbstk_t* _s, size_t _object_size, const mmvbstk_t* _other);

MEME_API int
MEME_STDCALL MemeVariableBufferStack_regSize(const mmvbstk_t* _obj);

MEME_API int
MEME_STDCALL MemeVariableBufferStack_objSize(const mmvbstk_t* _obj);

MEME_API mmint_t
MEME_STDCALL MemeVariableBufferStack_split(
	const mmvbstk_t* _buf,
	const mmbyte_t* _key, mmint_t _key_len,
	mmflag_split_behav_t,
	mmvbstk_t* MEGO_SYMBOL__RESTRICT _out, mmint_t _obj_size,
	mmint_t* MEGO_SYMBOL__RESTRICT _out_count,
	mmint_t* MEGO_SYMBOL__RESTRICT _search_index
);

MEME_API MemeVariableBuffer_Storage_t
MEME_STDCALL MemeVariableBuffer_storageType(MemeVariableBuffer_Const_t _s);

MEME_API int
MEME_STDCALL MemeVariableBuffer_swap(MemeVariableBuffer_t _lhs, MemeVariableBuffer_t _rhs);

MEME_API int
MEME_STDCALL MemeVariableBuffer_isNonempty(MemeVariableBuffer_Const_t _s);

MEME_API int
MEME_STDCALL MemeVariableBuffer_isEmpty(MemeVariableBuffer_Const_t _s);

MEME_API const MemeByte_t*
MEME_STDCALL MemeVariableBuffer_data(MemeVariableBuffer_Const_t _s);

MEME_API MemeByte_t*
MEME_STDCALL MemeVariableBuffer_dataWithNotConst(MemeVariableBuffer_t _s);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_size(MemeVariableBuffer_Const_t _s);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_maxByteSize(MemeVariableBuffer_Const_t _s);

MEME_API int
MEME_STDCALL MemeVariableBuffer_isEqual(
	MemeVariableBuffer_Const_t _s, const MemeByte_t* _buf, MemeInteger_t _len, int* _result);

MEME_API int
MEME_STDCALL MemeVariableBuffer_isEqualWithOther(
	MemeVariableBuffer_Const_t _lhs, MemeVariableBuffer_Const_t _rhs, int* _result);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_availableByteCapacity(MemeVariableBuffer_Const_t _s);

MEME_API const MemeByte_t*
MEME_STDCALL MemeVariableBuffer_constBackItem(MemeVariableBuffer_Const_t _s);

MEME_API MemeByte_t*
MEME_STDCALL MemeVariableBuffer_backItem(MemeVariableBuffer_t _s);

MEME_API const MemeByte_t*
MEME_STDCALL MemeVariableBuffer_constAt(MemeVariableBuffer_Const_t _s, MemeInteger_t _pos);

MEME_API MemeByte_t*
MEME_STDCALL MemeVariableBuffer_at(MemeVariableBuffer_t _s, MemeInteger_t _pos);


MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_indexOfWithBytes(
	MemeVariableBuffer_Const_t _s, MemeInteger_t _offset,
	const MemeByte_t* _needle, MemeInteger_t _needle_len);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_indexOfWithOther(
	MemeVariableBuffer_Const_t _s, MemeInteger_t _offset,
	MemeVariableBuffer_Const_t _other);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_indexOfWithByte(
    MemeVariableBuffer_Const_t _s, MemeInteger_t _offset, MemeByte_t _byte);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_appendWithByte(MemeVariableBuffer_t _s, MemeByte_t _byte);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_appendWithOther(
	MemeVariableBuffer_t _s, MemeVariableBuffer_Const_t _other);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_appendWithBytes(
	MemeVariableBuffer_t _s, const MemeByte_t* _buf, MemeInteger_t _len);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_appendWithRepeatBytes(
	MemeVariableBuffer_t _s, MemeInteger_t _count, MemeByte_t _byte);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_insertWithByte(
    MemeVariableBuffer_t _s, MemeInteger_t _pos, MemeByte_t _byte);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_insertWithBytes(
    MemeVariableBuffer_t _s, MemeInteger_t _pos, const MemeByte_t* _buf, MemeInteger_t _len);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_clear(MemeVariableBuffer_t _s);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_resize(MemeVariableBuffer_t _s, MemeInteger_t _size);

MEME_API mgec_t
MEME_STDCALL MemeVariableBuffer_resizeAndOverwrite(mmvb_ptr_t _b, mmint_t _size);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_resizeWithByte(
	MemeVariableBuffer_t _s, MemeInteger_t _size, MemeByte_t _byte);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_remove(
    MemeVariableBuffer_t _s, MemeInteger_t _pos, MemeInteger_t _count);

//! @param _out The string stack object, must be uninitialized.
MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_releaseToBuffer(
	MemeVariableBuffer_t _s, MemeBufferStack_t* _out, MemeInteger_t _objectSize);

/**
 * @brief Transfer the content of a MemeVariableBuffer into a stack string
 *        without copying heap memory where possible.
 *
 * This is a <b>destructive move</b> operation.  On success, the byte content
 * that was owned by @p _s is transferred to @p _out and @p _s is left in a
 * cleared, reset state.  Callers should treat @p _s as empty after a
 * successful call; it remains a valid (empty) buffer object and may be reused
 * or uninitialized normally.
 *
 * @par Storage-type dispatch
 * The behaviour depends on the internal storage type of @p _s:
 *
 * - <b>Small storage</b>: The small-string struct is copied byte-for-byte
 *   into @p _out via @c memcpy, trailing null bytes are stripped
 *   (@c MemeStringSmall_shrinkTailZero), and the source small-string slot
 *   is cleared in place.  This is a shallow struct copy, not a heap transfer.
 *
 * - <b>Medium storage</b>: The internal heap buffer pointer is transferred to
 *   @p _out as a large-string object via @c MemeStringLarge_initAndTakeover —
 *   no heap allocation or data copy occurs.  Trailing null bytes are then
 *   stripped (@c MemeStringLarge_shrinkTailZero).  Only after a successful
 *   takeover is the source pointer nulled and the medium struct reset.
 *
 * - <b>All other storage types</b>: The function returns @c MGEC__OPNOTSUPP
 *   immediately.  @p _s and @p _out are both left untouched.
 *
 * @par Precondition: @p _out must be uninitialized
 * @p _out must point to raw, uninitialized storage of at least
 * @c MMSTR__OBJ_SIZE bytes.  The function writes directly into this
 * memory without calling any init routine first.  Passing an already-
 * initialized @p _out leads to a resource leak or memory corruption.
 *
 * @par Error handling and object states after failure
 * - <b>Small path</b>: cannot fail; always returns @c 0.
 * - <b>Medium path</b>: if @c MemeStringLarge_initAndTakeover fails, the
 *   function returns its error code immediately.  At that point @p _out is in
 *   an <b>indeterminate</b> state (do not call any uninit routine on it) and
 *   @p _s still owns the original heap buffer and retains its content.
 * - <b>Unsupported type</b>: @c MGEC__OPNOTSUPP is returned; neither @p _s
 *   nor @p _out is modified.
 *
 * @par Trailing-null stripping
 * Both the small and medium success paths call the corresponding
 * @c shrinkTailZero helper on @p _out after the transfer.  Any null bytes
 * appended beyond the logical string content (e.g. by resize-with-byte
 * operations) are removed from the reported byte length.
 *
 * @param[in,out] _s          The variable buffer whose content is to be
 *                            released.  Must not be @c NULL (enforced by
 *                            assert).  On success, left in a cleared, empty
 *                            state.  On failure (medium path), retains its
 *                            original content unchanged.
 * @param[out]    _out        Pointer to <em>uninitialized</em> raw storage
 *                            that receives the string result.  Must not be
 *                            @c NULL (enforced by assert).  On success,
 *                            contains the transferred string content.  On
 *                            failure (medium path), left in an indeterminate
 *                            state; do not call any uninit routine on it.
 *                            Untouched on @c MGEC__OPNOTSUPP.
 * @param[in]     _objectSize Byte size of the @p _out object.  Currently
 *                            accepted but not used by the implementation
 *                            (the small path uses @c MMSTR__OBJ_SIZE
 *                            internally).  Pass @c MMSTR__OBJ_SIZE
 *                            or @c sizeof(*_out) for forward compatibility.
 *
 * @return @c 0 on success, @c MGEC__OPNOTSUPP if @p _s uses an unsupported
 *         storage type, or a non-zero error code from
 *         @c MemeStringLarge_initAndTakeover on medium-path failure.
 *
 * @see MemeVariableBuffer_releaseToBuffer  Transfer content to a buffer stack instead.
 * @see MemeStringLarge_initAndTakeover     Zero-copy heap-pointer takeover used internally.
 * @see MEME_STRING__OBJECT_SIZE            Canonical object size constant.
 */
MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_releaseToString(
	MemeVariableBuffer_t _s, MemeStringStack_t* _out, MemeInteger_t _objectSize);

//! @deprecated 将来可能会有ABI问题
MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_split(
	MemeVariableBuffer_Const_t _s,
	const MemeByte_t* _key, MemeInteger_t _key_len,
	MemeFlag_SplitBehavior_t,
	MemeVariableBufferStack_t* _out, MemeInteger_t* _out_count,
	MemeInteger_t* _search_index
);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_reserve(
	MemeVariableBuffer_t _s, MemeInteger_t _size);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_selfChop(
	MemeVariableBuffer_t _s, MemeInteger_t _n);

MEME_API MemeInteger_t
MEME_STDCALL MemeVariableBuffer_capacityCorrectness(
	MemeVariableBuffer_Const_t _s);

MEME_EXTERN_C_SCOPE_ENDED
#endif // !MEME_VARIABLE_BUFFER_H_INCLUDED
