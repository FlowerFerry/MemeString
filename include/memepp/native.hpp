
#ifndef MEMEPP_NATIVE_HPP_INCLUDED
#define MEMEPP_NATIVE_HPP_INCLUDED

#include <meme/native.h>

#include <string>

namespace memepp {

#if MG_OS__WIN_AVAIL
    using native_string = std::wstring;
#else
    using native_string = std::string;
#endif

};

#endif // !MEMEPP_NATIVE_HPP_INCLUDED
