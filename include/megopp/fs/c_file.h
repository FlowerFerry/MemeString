
#ifndef MEGOPP_FS_C_FILE_H_INCLUDED
#define MEGOPP_FS_C_FILE_H_INCLUDED

#include <stdio.h>
#include <mego/util/std/file.h>
#include <mego/common.h>

#include <memepp/string_view.hpp>

namespace mgpp {
namespace fs {

class c_file 
{
    FILE* file_;
public:
    inline c_file() noexcept
        : file_(nullptr)
    {}

#if MGOPT__USE_MEMESTR
    inline c_file(const memepp::string_view& _path, const memepp::string_view& _mode) noexcept
        : file_(mgu_fopen(_path.data(), _path.size(), _mode.data(), _mode.size()))
    {}
#else
    inline c_file(const char* _path, const char* _mode) noexcept
        : file_(fopen(_path, _mode))
    {}
#endif

    c_file(const c_file&) = delete;
    c_file& operator=(const c_file&) = delete;

    inline c_file(c_file&& _other) noexcept
        : file_(_other.file_)
    {
        _other.file_ = nullptr;
    }

    inline c_file& operator=(c_file&& _other) noexcept
    {
        if (this != &_other) {
            close();
            file_ = _other.file_;
            _other.file_ = nullptr;
        }
        return *this;
    }

    ~c_file()
    {
        close();
    }

    inline FILE* raw() const noexcept
    {
        return file_;
    }

    inline bool is_open() const noexcept
    {
        return file_ != nullptr;
    }

#if MGOPT__USE_MEMESTR
    inline void open(const memepp::string_view& _path, const memepp::string_view& _mode) noexcept
    {
        close();
        file_ = mgu_fopen(_path.data(), _path.size(), _mode.data(), _mode.size());
    }
#else
    inline void open(const char* _path, const char* _mode) noexcept
    {
        close();
        file_ = fopen(_path, _mode);
    }
#endif

    inline void close() noexcept
    {
        if (file_ != nullptr) {
            fclose(file_);
            file_ = nullptr;
        }
    }

};

}}

#endif // !MEGOPP_FS_C_FILE_H_INCLUDED
