
#ifndef MEMEPP_VARIANT_TMPIMPL_HPP_INCLUDED
#define MEMEPP_VARIANT_TMPIMPL_HPP_INCLUDED

#include <meme/variant.h>

#include <memepp/convert/common_def.hpp>
#include <memepp/variant_def.hpp>
#include <memepp/rune_def.hpp>
#include <memepp/string_def.hpp>
#include <memepp/buffer_def.hpp>
#include <memepp/variable_buffer_def.hpp>
#include <memepp/string_view_def.hpp>
#include <memepp/buffer_view_def.hpp>

namespace memepp {

    template<typename _Ty>
    inline mgec_t variant::try_get(_Ty& _out) const noexcept
    {
        return MGEC__OPNOTSUPP;
    }
    
    template<>
    inline mgec_t variant::try_get<mmbyte_t>(mmbyte_t& _out) const noexcept
    {
        return MemeVariantStack_getByte(&data_, MMVAR__OBJ_SIZE, &_out);
    }

    template<>
    inline mgec_t variant::try_get<char>(char& _out) const noexcept
    {
        return MemeVariantStack_getChar(&data_, MMVAR__OBJ_SIZE, &_out);
    }

    template<>
    inline mgec_t variant::try_get<wchar_t>(wchar_t& _out) const noexcept
    {
        return MemeVariantStack_getWChar(&data_, MMVAR__OBJ_SIZE, &_out);
    }

    template<>
    inline mgec_t variant::try_get<int64_t>(int64_t& _out) const noexcept
    {
        return MemeVariantStack_getInt64(&data_, MMVAR__OBJ_SIZE, &_out);
    }

    template<>
    inline mgec_t variant::try_get<uint64_t>(uint64_t& _out) const noexcept
    {
        return MemeVariantStack_getUInt64(&data_, MMVAR__OBJ_SIZE, &_out);
    }

    template<>
    inline mgec_t variant::try_get<double>(double& _out) const noexcept
    {
        return MemeVariantStack_getDouble(&data_, MMVAR__OBJ_SIZE, &_out);
    }

    template<>
    inline mgec_t variant::try_get<string>(string& _out) const noexcept
    {
        return MemeVariantStack_getString(
            &data_, MMVAR__OBJ_SIZE, &const_cast<mmstrstk_t&>(_out.native_handle()), 1);
    }
    
    template<>
    inline mgec_t variant::try_get<buffer>(buffer& _out) const noexcept
    {
        return MemeVariantStack_getBuffer(
            &data_, MMVAR__OBJ_SIZE, &const_cast<mmbufstk_t&>(_out.native_handle()), 1);
    }

    template<>
    inline mgec_t variant::try_get<variable_buffer>(variable_buffer& _out) const noexcept
    {
        return MemeVariantStack_getVariableBuffer(
            &data_, MMVAR__OBJ_SIZE, &const_cast<mmvbstk_t&>(_out.native_handle()), 1);
    }

    template<>
    inline mgec_t variant::try_get<rune>(rune& _out) const noexcept
    {
        return MemeVariantStack_getRune(&data_, MMVAR__OBJ_SIZE, &_out.native_handle());
    }

    template<>
    inline mgec_t variant::try_get<string_view>(string_view& _out) const noexcept
    {
        return MemeVariantStack_getStringView(
            &data_, MMVAR__OBJ_SIZE, &const_cast<mmstrstk_t&>(_out.native_handle()), 1);
    }

    template<>
    inline mgec_t variant::try_get<buffer_view>(buffer_view& _out) const noexcept
    {
        return MemeVariantStack_getBufferView(
            &data_, MMVAR__OBJ_SIZE, &const_cast<mmbufstk_t&>(_out.native_handle()), 1);
    }

    template<typename _Ty>
    inline mgec_t variant::set(const _Ty& _v) noexcept
    {
        return MGEC__OPNOTSUPP;
    }
    
    template<>
    inline mgec_t variant::set(const mmbyte_t& _v) noexcept
    {
        return MemeVariantStack_setByte(&data_, MMVAR__OBJ_SIZE, _v);
    }

    template<>
    inline mgec_t variant::set(const char& _v) noexcept
    {
        return MemeVariantStack_setChar(&data_, MMVAR__OBJ_SIZE, _v);
    }

    template<>
    inline mgec_t variant::set(const wchar_t& _v) noexcept
    {
        return MemeVariantStack_setWChar(&data_, MMVAR__OBJ_SIZE, _v);
    }

    template<>
    inline mgec_t variant::set(const int64_t& _v) noexcept
    {
        return MemeVariantStack_setInt64(&data_, MMVAR__OBJ_SIZE, _v);
    }

    template<>
    inline mgec_t variant::set(const uint64_t& _v) noexcept
    {
        return MemeVariantStack_setUInt64(&data_, MMVAR__OBJ_SIZE, _v);
    }

    template<>
    inline mgec_t variant::set(const double& _v) noexcept
    {
        return MemeVariantStack_setDouble(&data_, MMVAR__OBJ_SIZE, _v);
    }

    template<>
    inline mgec_t variant::set(const string& _v) noexcept
    {
        return MemeVariantStack_setString(
            &data_, MMVAR__OBJ_SIZE, memepp::to_pointer(_v.native_handle()));
    }

    template<>
    inline mgec_t variant::set(const buffer& _v) noexcept
    {
        return MemeVariantStack_setBuffer(
            &data_, MMVAR__OBJ_SIZE, memepp::to_pointer(_v.native_handle()));
    }

    template<>
    inline mgec_t variant::set(const variable_buffer& _v) noexcept
    {
        return MemeVariantStack_setVariableBuffer(
            &data_, MMVAR__OBJ_SIZE, memepp::to_pointer(_v.native_handle()));
    }

    template<>
    inline mgec_t variant::set(const rune& _v) noexcept
    {
        return MemeVariantStack_setRune(&data_, MMVAR__OBJ_SIZE, &_v.native_handle());
    }
    
    //template<>
    //inline variant import_from_dll(const mmvarstk_t& _obj)
    //{
    //    ;
    //}

    template<>
    inline mmvarstk_t export_into_dll(const variant& _obj)
    {
        mmvarstk_t result;
        MemeVariantStack_init  (&result, MMVAR__OBJ_SIZE);
        MemeVariantStack_assign(&result, MMVAR__OBJ_SIZE, memepp::to_pointer(_obj.native_handle()));
        return result;
    }
    
    template<>
    inline mmvarstk_t export_into_dll(variant&& _obj)
    {
        mmvarstk_t result;
        MemeVariantStack_initByMove(&result, MMVAR__OBJ_SIZE, const_cast<mmvarstk_t*>(&_obj.native_handle()));
        return result;
    }
}

#endif // !MEMEPP_VARIANT_TMPIMPL_HPP_INCLUDED
