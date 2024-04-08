
#ifndef MEGOPP_HELP_OBJBUF_H_INCLUDED
#define MEGOPP_HELP_OBJBUF_H_INCLUDED

#include <vector>
#include <type_traits>

namespace mgpp {
namespace help {

template <typename _Ty, typename _Alloc = std::allocator<uint8_t>>
class objbuf
{
public:
    using value_type = std::remove_reference_t<std::remove_cv_t<_Ty>>;
    using alloc_type = _Alloc;
    using item_type  = typename _Alloc::value_type;

    static_assert(std::is_trivially_copyable_v<value_type>, "Invalid type");

    objbuf()
        : buf_(_Alloc())
    {}

    objbuf(const objbuf&) = delete;

    objbuf(std::vector<uint8_t, _Alloc>&& _buf)
        : buf_(std::move(_buf))
    {
    }

    objbuf(size_t _size, const _Alloc& _alloc = _Alloc())
        : buf_(_size / sizeof(item_type) + sizeof(item_type), _alloc)
    {
    }

    objbuf& operator=(const objbuf&) = delete;

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

}
}

#endif // !MEGOPP_HELP_OBJBUF_H_INCLUDED
