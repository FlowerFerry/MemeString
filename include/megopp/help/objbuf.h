
#ifndef MEGOPP_HELP_OBJBUF_H_INCLUDED
#define MEGOPP_HELP_OBJBUF_H_INCLUDED

#include <vector>
#include <type_traits>

namespace mgpp {
namespace help {

template <typename _Ty, typename _Alloc = std::allocator<uint8_t>>
class object_buffer
{
public:
    using value_type = std::remove_reference_t<std::remove_cv_t<_Ty>>;
    using alloc_type = _Alloc;
    using item_type  = typename _Alloc::value_type;

    static_assert(std::is_trivially_copyable_v<value_type>, "Invalid type");

    object_buffer()
        : buf_(_Alloc())
    {}

    object_buffer(const object_buffer&) = delete;

    object_buffer(std::vector<uint8_t, _Alloc>&& _buf)
        : buf_(std::move(_buf))
    {
    }

    object_buffer(size_t _size, const _Alloc& _alloc = _Alloc())
        : buf_(_size / sizeof(item_type) + sizeof(item_type), _alloc)
    {
    }

    object_buffer& operator=(const object_buffer&) = delete;

    inline const value_type* get() const noexcept
    {
        if (buf_.size() * sizeof(item_type) < sizeof(value_type))
            return nullptr;
        return reinterpret_cast<const value_type*>(buf_.data());
    }

    inline constexpr std::vector<uint8_t>& src() noexcept
    {
        return buf_;
    }

    inline constexpr const std::vector<uint8_t>& src() const noexcept
    {
        return buf_;
    }

private:
    std::vector<uint8_t, _Alloc> buf_;
};

template<typename _Ty, typename _Alloc = std::allocator<uint8_t>>
using objbuf = object_buffer<_Ty, _Alloc>;

}
}

#endif // !MEGOPP_HELP_OBJBUF_H_INCLUDED
