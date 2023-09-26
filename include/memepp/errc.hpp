
#ifndef MEMEPP_ERRC_HPP_INCLUDED
#define MEMEPP_ERRC_HPP_INCLUDED

#include <mego/predef/symbol/likely.h>

#include <memepp/predef/macro_option.hpp>

#if !MMOPT__EXCEPTION_DISABLED
#include <stdexcept>
#endif

namespace memepp {

    inline int* errc() noexcept
    {
        thread_local static int errc = 0;
        return &errc;
    }

    inline void set_errc(int _code) noexcept { *errc() = _code; }
    inline int  get_errc() noexcept { return *errc(); }

#if !MMOPT__EXCEPTION_DISABLED
    class errc_except : public std::runtime_error
    {
    public:
        errc_except(int _code) noexcept : std::runtime_error("errc_exception"), m_code(_code) {}
        errc_except(int _code, const char* _what) noexcept : std::runtime_error(_what), m_code(_code) {}
        virtual ~errc_except() noexcept {}
        
        int code() const noexcept { return m_code; }
    
    private:
        int m_code;
    };

    class bad_alloc : public errc_except
    {
    public:
        bad_alloc() noexcept : errc_except(MGEC__NOMEM, "bad_alloc") {}
        bad_alloc(const char* _what) noexcept : errc_except(MGEC__NOMEM, _what) {}
        virtual ~bad_alloc() noexcept {}

    };

    inline void throw_errc(int _code) 
    { 
        if (MEGO_SYMBOL__UNLIKELY(_code != 0))
        {
            switch (_code) {
            case MGEC__NOMEM:
                throw bad_alloc();
            default:
                throw errc_except(_code);
            }
        }
    }

    inline void throw_errc(int _code, const char* _what)
    {
        if (MEGO_SYMBOL__UNLIKELY(_code != 0))
        {
            switch (_code) {
            case MGEC__NOMEM:
                throw bad_alloc(_what);
            default:
                throw errc_except(_code, _what);
            }
        }
    }
#endif

};

#endif // !MEMEPP_ERRC_HPP_INCLUDED
