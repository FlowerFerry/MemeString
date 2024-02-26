
#ifndef MEMEPP_VARIANT_SRCIMPL_HPP_INCLUDED
#define MEMEPP_VARIANT_SRCIMPL_HPP_INCLUDED

#include <meme/variant.h>
#include <memepp/variant_def.hpp>
#include <memepp/string_def.hpp>
#include <memepp/buffer_def.hpp>
#include <memepp/variable_buffer_def.hpp>
#include <memepp/rune_def.hpp>

namespace memepp {

    MEMEPP__IMPL_INLINE variant::variant() noexcept
    {
        MemeVariantStack_init(&data_, MMVAR__OBJ_SIZE);
    }

    MEMEPP__IMPL_INLINE variant::variant(mmvar_cptr_t _other)
    {
        MemeVariantStack_initByOther(&data_, MMVAR__OBJ_SIZE, _other);
    }

    MEMEPP__IMPL_INLINE variant::variant(native_handle_type&& _other)
    {
        MemeVariantStack_initByMove(&data_, MMVAR__OBJ_SIZE, &_other);
    }

    MEMEPP__IMPL_INLINE variant::variant(const native_handle_type& _other)
    {
        MemeVariantStack_initByOther(&data_, MMVAR__OBJ_SIZE, memepp::to_pointer(_other));
    }

    MEMEPP__IMPL_INLINE variant::variant(const variant& _other)
    {
        MemeVariantStack_initByOther(&data_, MMVAR__OBJ_SIZE, memepp::to_pointer(_other.data_));
    }

    MEMEPP__IMPL_INLINE variant::variant(variant&& _other) noexcept
    {
        MemeVariantStack_initByMove(&data_, MMVAR__OBJ_SIZE, &(_other.data_));
    }

    MEMEPP__IMPL_INLINE variant::variant(mmbyte_t _v) noexcept
    {
        MemeVariantStack_initByByte(&data_, MMVAR__OBJ_SIZE, _v);
    }

    MEMEPP__IMPL_INLINE variant::variant(char _v) noexcept
    {
        MemeVariantStack_initByChar(&data_, MMVAR__OBJ_SIZE, _v);
    }

    MEMEPP__IMPL_INLINE variant::variant(wchar_t _v) noexcept
    {
        MemeVariantStack_initByWChar(&data_, MMVAR__OBJ_SIZE, _v);
    }

    MEMEPP__IMPL_INLINE variant::variant(int64_t _v) noexcept
    {
        MemeVariantStack_initByInt64(&data_, MMVAR__OBJ_SIZE, _v);
    }

    MEMEPP__IMPL_INLINE variant::variant(uint64_t _v) noexcept
    {
        MemeVariantStack_initByUInt64(&data_, MMVAR__OBJ_SIZE, _v);
    }

    MEMEPP__IMPL_INLINE variant::variant(double _v) noexcept
    {
        MemeVariantStack_initByDouble(&data_, MMVAR__OBJ_SIZE, _v);
    }

    MEMEPP__IMPL_INLINE variant::variant(const string& _v)
    {
        MemeVariantStack_initByString(&data_, MMVAR__OBJ_SIZE, memepp::to_pointer(_v.native_handle()));
    }

    MEMEPP__IMPL_INLINE variant::variant(const buffer& _v)
    {
        MemeVariantStack_initByBuffer(&data_, MMVAR__OBJ_SIZE, memepp::to_pointer(_v.native_handle()));
    }

    MEMEPP__IMPL_INLINE variant::variant(const variable_buffer& _v)
    {
        MemeVariantStack_initByVariableBuffer(&data_, MMVAR__OBJ_SIZE, memepp::to_pointer(_v.native_handle()));
    }

    MEMEPP__IMPL_INLINE variant::variant(const rune& _v)
    {
        MemeVariantStack_initByRune(&data_, MMVAR__OBJ_SIZE, &_v.native_handle());
    }

    MEMEPP__IMPL_INLINE variant::~variant()
    {
        MemeVariantStack_unInit(&data_, MMVAR__OBJ_SIZE);
    }

    MEMEPP__IMPL_INLINE variant& variant::operator=(const variant& _other)
    {
        MemeVariantStack_assign(&data_, MMVAR__OBJ_SIZE, memepp::to_pointer(_other.data_));
        return *this;
    }

    MEMEPP__IMPL_INLINE variant& variant::operator=(variant&& _other) noexcept
    {
        swap(_other);
        return *this;
    }

    MEMEPP__IMPL_INLINE variant& variant::operator=(mmbyte_t _v)
    {
        MemeVariantStack_setByte(&data_, MMVAR__OBJ_SIZE, _v);
        return *this;
    }

    MEMEPP__IMPL_INLINE variant& variant::operator=(char _v)
    {
        MemeVariantStack_setChar(&data_, MMVAR__OBJ_SIZE, _v);
        return *this;
    }

    MEMEPP__IMPL_INLINE variant& variant::operator=(wchar_t _v)
    {
        MemeVariantStack_setWChar(&data_, MMVAR__OBJ_SIZE, _v);
        return *this;
    }

    MEMEPP__IMPL_INLINE variant& variant::operator=(int64_t _v)
    {
        MemeVariantStack_setInt64(&data_, MMVAR__OBJ_SIZE, _v);
        return *this;
    }

    MEMEPP__IMPL_INLINE variant& variant::operator=(uint64_t _v)
    {
        MemeVariantStack_setUInt64(&data_, MMVAR__OBJ_SIZE, _v);
        return *this;
    }

    MEMEPP__IMPL_INLINE variant& variant::operator=(double _v)
    {
        MemeVariantStack_setDouble(&data_, MMVAR__OBJ_SIZE, _v);
        return *this;
    }

    MEMEPP__IMPL_INLINE variant& variant::operator=(const string& _v)
    {
        MemeVariantStack_setString(&data_, MMVAR__OBJ_SIZE, memepp::to_pointer(_v.native_handle()));
        return *this;
    }

    MEMEPP__IMPL_INLINE variant& variant::operator=(const buffer& _v)
    {
        MemeVariantStack_setBuffer(&data_, MMVAR__OBJ_SIZE, memepp::to_pointer(_v.native_handle()));
        return *this;
    }

    MEMEPP__IMPL_INLINE variant& variant::operator=(const variable_buffer& _v)
    {
        MemeVariantStack_setVariableBuffer(&data_, MMVAR__OBJ_SIZE, memepp::to_pointer(_v.native_handle()));
        return *this;
    }

    MEMEPP__IMPL_INLINE variant& variant::operator=(const rune& _v)
    {
        MemeVariantStack_setRune(&data_, MMVAR__OBJ_SIZE, &_v.native_handle());
        return *this;
    }

    MEMEPP__IMPL_INLINE bool variant::is_null() const noexcept
    {
        return MemeVariantStack_isNull(&data_, MMVAR__OBJ_SIZE) == 1 ? true : false;
    }
    
    MEMEPP__IMPL_INLINE bool variant::is_type(meta::typid _type) const noexcept
    {
        return MemeVariantStack_isType(&data_, MMVAR__OBJ_SIZE, (mmmeta_typid_t)(_type)) == 1 ? true : false;
    }

    MEMEPP__IMPL_INLINE meta::typid variant::type_id() const noexcept
    {
        return (meta::typid)MemeVariantStack_typeId(&data_, MMVAR__OBJ_SIZE);
    }
    
    MEMEPP__IMPL_INLINE void variant::reset() noexcept
    {
        MemeVariantStack_reset(&data_, MMVAR__OBJ_SIZE);
    }

    MEMEPP__IMPL_INLINE void variant::swap(variant& _other) noexcept
    {
        MemeVariantStack_swap(&data_, &_other.data_, MMVAR__OBJ_SIZE);
    }

    MEMEPP__IMPL_INLINE const variant::native_handle_type& variant::native_handle() const noexcept
    {
        return data_;
    }

}

#endif // !MEMEPP_VARIANT_SRCIMPL_HPP_INCLUDED
