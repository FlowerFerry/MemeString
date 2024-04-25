
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

        MEMEPP__IMPL_INLINE variant() noexcept;
        MEMEPP__IMPL_INLINE variant(mmvar_cptr_t _other);
        MEMEPP__IMPL_INLINE variant(native_handle_type&& _other);
        MEMEPP__IMPL_INLINE variant(const native_handle_type& _other);

        MEMEPP__IMPL_INLINE variant(const variant& _other);
        MEMEPP__IMPL_INLINE variant(variant&& _other) noexcept;

        MEMEPP__IMPL_INLINE variant(mmbyte_t _v) noexcept;
        MEMEPP__IMPL_INLINE variant(char _v) noexcept;
        MEMEPP__IMPL_INLINE variant(wchar_t _v) noexcept;
        MEMEPP__IMPL_INLINE variant(int64_t _v) noexcept;
        MEMEPP__IMPL_INLINE variant(uint64_t _v) noexcept;
        MEMEPP__IMPL_INLINE variant(double _v) noexcept;
        MEMEPP__IMPL_INLINE variant(const string& _v);
        MEMEPP__IMPL_INLINE variant(const buffer& _v);
        MEMEPP__IMPL_INLINE variant(const variable_buffer& _v);
        MEMEPP__IMPL_INLINE variant(const rune& _v);

        MEMEPP__IMPL_INLINE ~variant();

        MEMEPP__IMPL_INLINE variant& operator=(const variant& _other);
        MEMEPP__IMPL_INLINE variant& operator=(variant&& _other) noexcept;

        MEMEPP__IMPL_INLINE variant& operator=(mmbyte_t _v);
        MEMEPP__IMPL_INLINE variant& operator=(char _v);
        MEMEPP__IMPL_INLINE variant& operator=(wchar_t _v);
        MEMEPP__IMPL_INLINE variant& operator=(int64_t _v);
        MEMEPP__IMPL_INLINE variant& operator=(uint64_t _v);
        MEMEPP__IMPL_INLINE variant& operator=(double _v);
        MEMEPP__IMPL_INLINE variant& operator=(const string& _v);
        MEMEPP__IMPL_INLINE variant& operator=(const buffer& _v);
        MEMEPP__IMPL_INLINE variant& operator=(const variable_buffer& _v);
        MEMEPP__IMPL_INLINE variant& operator=(const rune& _v);

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
        
        template<typename _Ty>
        mgec_t try_convert(_Ty& _out) const noexcept;

        template<typename _Ty>
        inline _Ty convert_or(const _Ty& _default = {}) const noexcept
        {
            _Ty _out = {};
            if (try_convert(_out) == 0)
                return _out;
            return _default;
        }

        //template<typename _Ty>
        //inline _Ty to() const noexcept
        //{
        //    _Ty _out = {};
        //    if (try_convert(_out) == 0)
        //        return _out;
        //    return {};
        //}

        MEMEPP__IMPL_INLINE bool is_null() const noexcept;
        MEMEPP__IMPL_INLINE bool is_type(meta::typid _type) const noexcept;

        MEMEPP__IMPL_INLINE meta::typid type_id() const noexcept;

        template<typename _Ty>
        mgec_t set(const _Ty& _v) noexcept;

        MEMEPP__IMPL_INLINE void reset() noexcept;
        MEMEPP__IMPL_INLINE void swap(variant& _other) noexcept;

        MEMEPP__IMPL_INLINE const native_handle_type& native_handle() const noexcept;
    private:
        native_handle_type data_;
    };

}

#endif // !MEMEPP_VARIANT_DEF_HPP_INCLUDED

#include <memepp/variant_tmpimpl.hpp>
