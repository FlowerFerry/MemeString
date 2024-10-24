
#ifndef MEGO_MEM_CSTR_ALLOC_IF_NO_END_ZERO_H_INCLUDED
#define MEGO_MEM_CSTR_ALLOC_IF_NO_END_ZERO_H_INCLUDED

#include <mego/err/ec.h>
#include <mego/predef/symbol/likely.h>
#include <mego/predef/symbol/inline.h>
#include <meme/string.h>

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <wchar.h>

//! 如果源字符串没有以 '\0' 结尾，分配一个新的字符串并添加 '\0' 结尾。
//! 
//! @param[in] _src 源字符串的指针。
//! @param[in] _slen 源字符串的长度。如果为负数，将自动计算长度。
//! @param[out] _out 输出字符串的指针。如果需要分配新的字符串，将返回该指针。
//! @param[out] _olen 输出字符串的长度指针。如果不为 NULL，将返回字符串的长度。
//! @param[in] _must_alloc 指示是否必须分配新字符串的标志。
//! 
//! @return 返回一个错误码。如果成功则返回 0，如果输入无效则返回 MGEC__INVAL，如果内存不足则返回 MGEC__NOMEM。
MG_CAPI_INLINE mgec_t mgmem__cstr_alloc_if_no_end_zero(
    const char *_src, mmint_t _slen, const char ** _out, mmint_t* _olen, int _must_alloc)
{
    char * str = NULL;

    if (MEGO_SYMBOL__UNLIKELY(_src == NULL))
        return MGEC__INVAL;
    if (MEGO_SYMBOL__UNLIKELY(_out == NULL))
        return MGEC__INVAL;

    if (_slen < 0) {
        if (_must_alloc) {
            _slen = strlen(_src);
        }
        else {
            if (_olen != NULL)
                *_olen = strlen(_src);
            *_out = _src;
            return 0;
        }
    }

    if (_src[_slen] == '\0' && !_must_alloc) 
    {            
        if (_olen != NULL)
            *_olen = _slen;
        *_out = _src;
        return 0;
    }

    str = (char*)malloc(sizeof(char) * (_slen + 1));
    if (str == NULL)
        return MGEC__NOMEM;

    memcpy(str, _src, _slen);
    str[_slen] = '\0';

    if (_olen != NULL)
        *_olen = _slen;
    *_out = str;
    return 0;
}

//! 如果源字符串没有以 '\0' 结尾，分配一个新的字符串并添加 '\0' 结尾。
//! 
//! @param[in] _src 源字符串的指针。
//! @param[in] _slen 源字符串的长度。如果为负数，将自动计算长度。
//! @param[out] _out 输出字符串的指针。如果需要分配新的字符串，将返回该指针。
//! @param[out] _olen 输出字符串的长度指针。如果不为 NULL，将返回字符串的长度。
//! @param[in] _must_alloc 指示是否必须分配新字符串的标志。
//! 
//! @return 返回一个错误码。如果成功则返回 0，如果输入无效则返回 MGEC__INVAL，如果内存不足则返回 MGEC__NOMEM。
MG_CAPI_INLINE mgec_t mgmem__wcstr_alloc_if_no_end_zero(
    const wchar_t *_src, mmint_t _slen, const wchar_t ** _out, mmint_t* _olen, int _must_alloc)
{
    wchar_t * str = NULL;

    if (MEGO_SYMBOL__UNLIKELY(_src == NULL))
        return MGEC__INVAL;
    if (MEGO_SYMBOL__UNLIKELY(_out == NULL))
        return MGEC__INVAL;

    if (_slen < 0) {
        if (_must_alloc) {
            _slen = wcslen(_src);
        }
        else {
            if (_olen != NULL)
                *_olen = wcslen(_src);
            *_out = _src;
            return 0;
        }
    }

    if (_src[_slen] == '\0' && !_must_alloc) 
    {    
        if (_olen != NULL)
            *_olen = _slen;
        *_out = _src;
        return 0;
    }

    str = (wchar_t*)malloc(sizeof(wchar_t) * (_slen + 1));
    if (str == NULL)
        return MGEC__NOMEM;

    memcpy(str, _src, sizeof(wchar_t) * _slen);
    str[_slen] = '\0';

    if (_olen != NULL)
        *_olen = _slen;
    *_out = str;
    return 0;
}

MG_CAPI_INLINE void mgmem__free_if_ptr_not_equal(const void *_src, void * _new)
{
    if (_new != NULL && _new != _src)
        free(_new);
}

#endif // !MEGO_MEM_CSTR_ALLOC_IF_NO_END_ZERO_H_INCLUDED
