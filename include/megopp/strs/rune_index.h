
#ifndef MEGOPP_STRS_RUNE_INDEX_H_INCLUDED
#define MEGOPP_STRS_RUNE_INDEX_H_INCLUDED

#include <mego/predef/lang/version.h>
#include <mego/strs/rune_index.h>

namespace mgpp {
namespace strs {

class rune_index
{
public:
    using value_type = uint8_t;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

#if MG_LANG__CXX17_AVAIL
    inline static constexpr size_type npos = static_cast<size_type>(-1);
#else
    enum : size_type { npos = static_cast<size_type>(-1) };
#endif

    rune_index(const_pointer _u8, size_type _size) {
        data_.data = _u8;
        data_.size = (int8_t)_size;
    }

    rune_index(const mgs_rune_index_t& other) noexcept : data_(other) {}
    rune_index(mgs_rune_index_t&& other) noexcept : data_(std::move(other)) {}
    rune_index(const rune_index& other) noexcept : data_(other.data_) {}
    rune_index(rune_index&& other) noexcept : data_(std::move(other.data_)) {}

    const_pointer data() const noexcept { return data_.data; }
    size_type size() const noexcept { return data_.size; }

    inline mgs_rune_index_t& native_handle() noexcept { return data_; }
    inline const mgs_rune_index_t& native_handle() const noexcept { return data_; }
private:
    mgs_rune_index_t data_;
};

}
}

#endif // !MEGOPP_STRS_RUNE_INDEX_H_INCLUDED

