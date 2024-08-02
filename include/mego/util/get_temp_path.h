
#ifndef MEGO_UTIL_GETTEMPPATH_H_INCLUDED
#define MEGO_UTIL_GETTEMPPATH_H_INCLUDED

#include <mego/predef/os/windows.h>
#include <mego/predef/symbol/inline.h>
#include <mego/util/os/windows/windows_simplify.h>
#include <meme/utf/converter.h>
#include <mego/util/std/string.h>

#include <string.h>

#if __cplusplus
extern "C" {
#endif // __cppplusplus
    
    MG_CAPI_INLINE int mgu_get_temp_path(char* _out, int _capacity)
    {
#if MG_OS__WIN_AVAIL
        mmint_t result = 0;
        WCHAR wpath[MAX_PATH];
        if (GetTempPathW(MAX_PATH, wpath) == 0)
        {
            return -1;
        }

        result = mmutf_char_size_u8from16((const uint16_t*)wpath, wcslen(wpath));
        if (result < 0)
            return -1;

        if (result == 0) {
            if (_capacity < 1)
                return -1;
            _out[0] = '\0';
            return 0;
        }
        
        if (result + 1 > _capacity)
            return -1;
        
        result = mmutf_convert_u16to8((const uint16_t*)wpath, wcslen(wpath), (mmbyte_t*)_out);
        if (result < 0)
            return -1;
        
        for (int i = 0; i < result; ++i)
        {
            if (_out[i] == '\\')
                _out[i] = '/';
        }
        _out[result] = '\0';
#else
        const char* tmp_path = "/tmp/";
        if (_capacity < strlen(tmp_path) + 1)
        {
            return -1;
        }

        mgu_strncpy_s(_out, _capacity, tmp_path, _capacity - 1);
#endif
        return 0;
    }


#if __cplusplus
}
#endif // __cppplusplus

#endif // !MEGO_UTIL_GETTEMPPATH_H_INCLUDED
