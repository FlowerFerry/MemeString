
#ifndef MEME_UNSAFE_BUFFER_VIEW_H_INCLUDED
#define MEME_UNSAFE_BUFFER_VIEW_H_INCLUDED

#include "meme/buffer_fwd.h"
#include "meme/variable_buffer_fwd.h"
#include "meme/string_fwd.h"

MEME_EXTERN_C_SCOPE_START

MEME_API int
MEME_STDCALL MemeBufferViewUnsafeStack_init(
    mmbufstk_t* _self,
    size_t _object_size,
	const MemeByte_t* _buf, MemeInteger_t _len
);

MEME_API int
MEME_STDCALL MemeBufferViewUnsafeStack_initByOther(
    mmbufstk_t* _self,
    size_t _object_size,
    const mmbufstk_t* _other
);

MEME_API int
MEME_STDCALL MemeBufferViewUnsafeStack_initByString(
    mmbufstk_t* _self,
    size_t _object_size,
    const mmsstk_t* _other
);

MEME_API int
MEME_STDCALL MemeBufferViewUnsafeStack_initByVariableBuffer(
    mmbufstk_t* _self,
    size_t _object_size,
    const mmvbstk_t* _other
);

MEME_API int
MEME_STDCALL MemeBufferViewUnsafeStack_assignByOther(
    mmbufstk_t* _self,
    size_t _object_size,
    const mmbufstk_t* _other
);

MEME_API int
MEME_STDCALL MemeBufferViewUnsafeStack_assignByString(
    mmbufstk_t* _self,
    size_t _object_size,
    const mmsstk_t* _other
);

MEME_API int
MEME_STDCALL MemeBufferViewUnsafeStack_assignByVariableBuffer(
    mmbufstk_t* _self,
    size_t _object_size,
    const mmvbstk_t* _other
);

MEME_EXTERN_C_SCOPE_ENDED

#endif // !MEME_UNSAFE_BUFFER_VIEW_H_INCLUDED
