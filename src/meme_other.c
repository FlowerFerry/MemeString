
#include <meme/version.h>

MEME_EXTERN_C MEME_API mmver_t MEME_STDCALL mmver_num()
{
    return MMVER_NUM;
}

MEME_EXTERN_C MEME_API const char* MEME_STDCALL mmver_str()
{
    static const char str[] = MMVER_STR;
    return str;
}
