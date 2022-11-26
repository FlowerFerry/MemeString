
#ifndef MEMEPP_UTIL_RELATIVE_EXEC_PATH_HPP_INCLUDED
#define MEMEPP_UTIL_RELATIVE_EXEC_PATH_HPP_INCLUDED

#include <mego/util/get_exec_path.h>
#include <memepp/string.hpp>
#include <memepp/variable_buffer.hpp>

namespace memepp {
namespace util {

    inline memepp::string exec_directory_path()
    {
        int pos = 0;
        int len = MegoUtil_GetExecutablePath(NULL, 0, NULL);
        if (len <= 0)
        {
            return {};
        }
        memepp::variable_buffer vb{ len, 0 };
        len = MegoUtil_GetExecutablePath(
            reinterpret_cast<char*>(vb.data()), len, &pos);
        if (len <= 0)
        {
            return {};
        }

        return memepp::string{ vb.data(), pos };
    }

    inline memepp::string relative_exec_path(const memepp::string_view& _path)
    {
        auto path = _path.trim_space();

        //if (path.starts_with("/"))
        //    return path.to_string();
        //if (path.find(":/") != memepp::string_view::npos)
        //    return path.to_string();

        auto dir = exec_directory_path();
        
        memepp::variable_buffer vb{ dir.bytes(), dir.size() };
        vb.push_back(uint8_t('/'));
        vb.append(path);

        memepp::string s;
        vb.release(s);
        return s;
    }

} // namespace util
} // namespace memepp


#endif // MEMEPP_UTIL_RELATIVE_EXEC_PATH_HPP_INCLUDED
