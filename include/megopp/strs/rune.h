
#ifndef MEGOPP_STRS_RUNE_H_INCLUDED
#define MEGOPP_STRS_RUNE_H_INCLUDED

#include <mego/predef/lang/version.h>
#include <mego/predef/symbol/visibility_hidden.h>
#include <mego/strs/rune.h>

#include <string.h>

#include <type_traits>

namespace mgpp {
namespace strs {

class MG_SYM__OPT_VIS_HIDDEN rune
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

    rune() noexcept {
        clear();
    }

    rune(char _ch) noexcept {
        clear();
        data_.byte[0] = static_cast<uint8_t>(_ch);
        data_.attr.capacity = static_cast<uint8_t>(MGS_RUNE__MAX_CHAR_SIZE - 1);
    }

    rune(const_pointer _u8, size_type _size) noexcept {
        clear();
        if (_u8 != nullptr && _size > 0 && _size <= MGS_RUNE__MAX_CHAR_SIZE) {
            memcpy(data_.byte, _u8, _size);
            data_.attr.capacity = static_cast<uint8_t>(MGS_RUNE__MAX_CHAR_SIZE - _size);
        }
    }

    rune(const mgs_rune_t& other) noexcept : data_(other) {}
    rune(mgs_rune_t&& other) noexcept : data_(std::move(other)) {}

    rune(const rune& other) noexcept : data_(other.data_) {}
    rune(rune&& other) noexcept : data_(std::move(other.data_)) {}

    rune& operator=(char _ch) noexcept {
        clear();
        data_.byte[0] = static_cast<uint8_t>(_ch);
        data_.attr.capacity = static_cast<uint8_t>(MGS_RUNE__MAX_CHAR_SIZE - 1);
        return *this;
    }

    rune& operator=(const char* _u8) noexcept {
        clear();
        if (_u8 != nullptr) {
            size_type len = strlen(_u8);
            if (len > 0 && len <= MGS_RUNE__MAX_CHAR_SIZE) {
                memcpy(data_.byte, _u8, len);
                data_.attr.capacity = static_cast<uint8_t>(MGS_RUNE__MAX_CHAR_SIZE - len);
            }
        }
        return *this;
    }

    rune& operator=(const mgs_rune_t& other) noexcept {
        data_ = other;
        return *this;
    }

    rune& operator=(mgs_rune_t&& other) noexcept {
        data_ = std::move(other);
        return *this;
    }

    rune& operator=(const rune& other) noexcept {
        if (this != &other) {
            data_ = other.data_;
        }
        return *this;
    }

    rune& operator=(rune&& other) noexcept {
        if (this != &other) {
            data_ = std::move(other.data_);
        }
        return *this;
    }

    void clear() noexcept {
        memset(&data_, 0, sizeof(data_));
        data_.attr.capacity = MGS_RUNE__MAX_CHAR_SIZE;
    }

    bool operator==(const rune& _r) const noexcept {
        if (size() != _r.size())
            return false;
        return memcmp(data(), _r.data(), size()) == 0;
    }

    bool operator!=(const rune& _r) const noexcept { return !(*this == _r); }

    bool operator==(char _ch) const noexcept {
        if (size() != 1)
            return false;
        return data()[0] == static_cast<uint8_t>(_ch);
    }

    bool operator!=(char _ch) const noexcept { return !(*this == _ch); }

    bool operator==(const char* _u8) const noexcept {
        size_t _size = strlen(_u8);
        if (static_cast<size_t>(size()) != _size)
            return false;
        return memcmp(data(), _u8, _size) == 0;
    }

    bool operator!=(const char* _u8) const noexcept { return !(*this == _u8); }

    bool operator< (const rune& _r) const noexcept { return compare(_r) <  0; }
    bool operator<=(const rune& _r) const noexcept { return compare(_r) <= 0; }
    bool operator> (const rune& _r) const noexcept { return compare(_r) >  0; }
    bool operator>=(const rune& _r) const noexcept { return compare(_r) >= 0; }

    inline explicit operator bool() const noexcept { return valid() && !empty(); }

    const_pointer data() const noexcept { return data_.byte; }
    pointer data() noexcept { return data_.byte; }

    size_type size() const noexcept { return MGS_RUNE__MAX_CHAR_SIZE - data_.attr.capacity; }

    bool empty() const noexcept { return size() == 0; }
    bool valid() const noexcept { return data_.attr.invalid == 0; }

    bool is_multi() const noexcept { return (data_.byte[0] & 0x80) && (size() > 1); }
    inline bool is_en_char() const noexcept { return !is_multi(); }

    int compare(const rune& _other) const noexcept {
        size_type min_sz = (size() < _other.size()) ? size() : _other.size();
        int cmp = memcmp(data(), _other.data(), min_sz);
        if (cmp == 0) {
            if (size() < _other.size()) return -1;
            if (size() > _other.size()) return  1;
            return 0;
        }
        return cmp;
    }

    inline mgs_rune_t& native_handle() noexcept { return data_; }
    inline const mgs_rune_t& native_handle() const noexcept { return data_; }
private:
    mgs_rune_t data_;
};

}
}

#endif // !MEGOPP_STRS_RUNE_H_INCLUDED
