
#ifndef MEMEPP_VARIANT_DEF_HPP_INCLUDED
#define MEMEPP_VARIANT_DEF_HPP_INCLUDED

#include <meme/string_fwd.h>
#include <meme/variant_fwd.h>

#include <memepp/metatype_fwd.hpp>
#include <memepp/variant_fwd.hpp>
#include <memepp/rune_fwd.hpp>
#include <memepp/string_fwd.hpp>
#include <memepp/buffer_fwd.hpp>
#include <memepp/variable_buffer_fwd.hpp>
#include <memepp/string_view_fwd.hpp>
#include <memepp/buffer_view_fwd.hpp>

namespace memepp {

    class variant 
    {
    public:
        using native_handle_type = mmvarstk_t;

        variant() noexcept;
        variant(mmvar_cptr_t _other);
        variant(native_handle_type&& _other);
        variant(const native_handle_type& _other);

        variant(const variant& _other);
        variant(variant&& _other);

        variant(mmbyte_t _v) noexcept;
        variant(char _v) noexcept;
        variant(wchar_t _v) noexcept;
        variant(int64_t _v) noexcept;
        variant(uint64_t _v) noexcept;
        variant(double _v) noexcept;
        variant(const string& _v);
        variant(const buffer& _v);
        variant(const variable_buffer& _v);
        variant(const rune& _v);

        ~variant();

        variant& operator=(const variant& _other);
        variant& operator=(variant&& _other);

        variant& operator=(mmbyte_t _v);
        variant& operator=(char _v);
        variant& operator=(wchar_t _v);
        variant& operator=(int64_t _v);
        variant& operator=(uint64_t _v);
        variant& operator=(double _v);
        variant& operator=(const string& _v);
        variant& operator=(const buffer& _v);
        variant& operator=(const variable_buffer& _v);
        variant& operator=(const rune& _v);

        template<typename _Ty>
        mgec_t try_get(_Ty& _out) const noexcept;
        
        template<typename _Ty>
        inline _Ty get_or(const _Ty& _default = {}) const noexcept
        {
            _Ty _out = {};
            if (try_get(_out) == 0)
                return _out;
            return _default;
        }
        
        bool is_null() const noexcept;
        bool is_type(meta::typid _type) const noexcept;

        meta::typid type_id() const noexcept;

        template<typename _Ty>
        mgec_t set(const _Ty& _v) noexcept;

        void reset() noexcept;
        void swap(variant& _other) noexcept;

        const native_handle_type& native_handle() const noexcept;
    private:
        native_handle_type data_;
    };

}

#endif // !MEMEPP_VARIANT_DEF_HPP_INCLUDED

#include <memepp/variant_tmpimpl.hpp>
