#ifndef TEST_SHARED_H_INCLUDED
#define TEST_SHARED_H_INCLUDED

#include <meme/string.h>
#include <meme/buffer.h>
#include <meme/variable_buffer.h>
#include <meme/variant.h>
#include <string.h>

/* Cast helpers: stack-struct pointers → opaque pointer types.
   The stack structs (mmstrstk_t, mmbufstk_t, mmvbstk_t, mmvarstk_t) are
   layout-compatible with the opaque pointer types but are formally distinct,
   hence the explicit cast to suppress MSVC C4133. */
#define AS_MMSTR_C(ptr)  ((mmstr_cptr_t)(void*)(ptr))
#define AS_MMSTR(ptr)    ((mmstr_ptr_t)(void*)(ptr))
#define AS_MMBUF_C(ptr)  ((mmbuf_cptr_t)(void*)(ptr))
#define AS_MMBUF(ptr)    ((mmbuf_ptr_t)(void*)(ptr))
#define AS_MMVB_C(ptr)   ((MemeVariableBuffer_Const_t)(void*)(ptr))
#define AS_MMVB(ptr)     ((MemeVariableBuffer_t)(void*)(ptr))
#define AS_MMVAR_C(ptr)  ((mmvar_cptr_t)(void*)(ptr))
#define AS_MMBLDR_C(ptr) ((mmsbldr_const_t)(void*)(ptr))
#define AS_MMBLDR(ptr)   ((mmsbldr_t)(void*)(ptr))

/* Helper: initialize a string from a C string literal */
static void init_cstr(mmstrstk_t* s, const char* cstr)
{
    int rc = mmstrstk_init_by_u8(s, cstr, (mmint_t)strlen(cstr));
    mu_assert(rc == 0, "mmstrstk_init_by_u8 should succeed");
}

#endif /* TEST_SHARED_H_INCLUDED */
