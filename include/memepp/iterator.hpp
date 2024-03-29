
#ifndef MEMEPP_ITERATORBASIC_HPP_INCLUDED
#define MEMEPP_ITERATORBASIC_HPP_INCLUDED

#include <meme/string_fwd.h>
#include <iterator>

namespace memepp {

    class iterator
    {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = MemeByte_t;
        using difference_type = MemeInteger_t;
        using pointer = MemeByte_t*;
        using reference = MemeByte_t&;

        iterator() noexcept = default;
        iterator(const iterator&) noexcept = default;
        iterator(iterator&&) noexcept = default;
        iterator& operator=(const iterator&) noexcept = default;
        iterator& operator=(iterator&&) noexcept = default;
        ~iterator() noexcept = default;

        iterator(pointer _ptr) noexcept
            : p_(_ptr)
        {}

        iterator& operator++() noexcept
        {
            ++p_;
            return *this;
        }

        iterator operator++(int) noexcept
        {
            iterator _tmp(*this);
            ++p_;
            return _tmp;
        }

        iterator& operator--() noexcept
        {
            --p_;
            return *this;
        }

        iterator operator--(int) noexcept
        {
            iterator _tmp(*this);
            --p_;
            return _tmp;
        }

        iterator& operator+=(difference_type _n) noexcept
        {
            p_ += _n;
            return *this;
        }

        iterator operator+(difference_type _n) const noexcept
        {
            iterator _tmp(*this);
            _tmp += _n;
            return _tmp;
        }

        iterator& operator-=(difference_type _n) noexcept
        {
            p_ -= _n;
            return *this;
        }

        iterator operator-(difference_type _n) const noexcept
        {
            iterator _tmp(*this);
            _tmp -= _n;
            return _tmp;
        }

        difference_type operator-(const iterator& _rhs) const noexcept
        {
            return p_ - _rhs.p_;
        }

        reference operator*() const noexcept
        {
            return *p_;
        }

        pointer operator->() const noexcept
        {
            return p_;
        }

        reference operator[](difference_type _n) const noexcept
        {
            return p_[_n];
        }

        bool operator==(const iterator& _rhs) const noexcept
        {
            return p_ == _rhs.p_;
        }

        bool operator!=(const iterator& _rhs) const noexcept
        {
            return p_ != _rhs.p_;
        }

        bool operator<(const iterator& _rhs) const noexcept
        {
            return p_ < _rhs.p_;
        }

        bool operator>(const iterator& _rhs) const noexcept
        {
            return p_ > _rhs.p_;
        }

        bool operator<=(const iterator& _rhs) const noexcept
        {
            return p_ <= _rhs.p_;
        }

        bool operator>=(const iterator& _rhs) const noexcept
        {
            return p_ >= _rhs.p_;
        }

    private:
        pointer p_ = nullptr;
    };

    class const_iterator
    {
    public:
        using value_type = MemeByte_t;
        using reference = const MemeByte_t&;
        using pointer = const MemeByte_t*;
        using difference_type = MemeInteger_t;
        using iterator_category = std::random_access_iterator_tag;

        const_iterator() noexcept = default;
        const_iterator(const const_iterator&) noexcept = default;
        const_iterator(const_iterator&&) noexcept = default;
        const_iterator& operator=(const const_iterator&) noexcept = default;
        const_iterator& operator=(const_iterator&&) noexcept = default;
        ~const_iterator() noexcept = default;

        const_iterator(pointer _ptr) noexcept
            : p_(_ptr)
        {}

        const_iterator& operator++() noexcept
        {
            ++p_;
            return *this;
        }

        const_iterator operator++(int) noexcept
        {
            const_iterator tmp(*this);
            ++p_;
            return tmp;
        }

        const_iterator& operator--() noexcept
        {
            --p_;
            return *this;
        }

        const_iterator operator--(int) noexcept
        {
            const_iterator tmp(*this);
            --p_;
            return tmp;
        }

        const_iterator& operator+=(difference_type _n) noexcept
        {
            p_ += _n;
            return *this;
        }

        const_iterator operator+(difference_type _n) const noexcept
        {
            return const_iterator(p_ + _n);
        }

        const_iterator& operator-=(difference_type _n) noexcept
        {
            p_ -= _n;
            return *this;
        }

        const_iterator operator-(difference_type _n) const noexcept
        {
            return const_iterator(p_ - _n);
        }

        difference_type operator-(const const_iterator& _rhs) const noexcept
        {
            return p_ - _rhs.p_;
        }

        reference operator*() const noexcept
        {
            return *p_;
        }

        pointer operator->() const noexcept
        {
            return p_;
        }

        reference operator[](difference_type _n) const noexcept
        {
            return p_[_n];
        }

        bool operator==(const const_iterator& _other) const noexcept
        {
            return p_ == _other.p_;
        }

        bool operator!=(const const_iterator& _other) const noexcept
        {
            return p_ != _other.p_;
        }

        bool operator<(const const_iterator& _other) const noexcept
        {
            return p_ < _other.p_;
        }

        bool operator<=(const const_iterator& _other) const noexcept
        {
            return p_ <= _other.p_;
        }

        bool operator>(const const_iterator& _other) const noexcept
        {
            return p_ > _other.p_;
        }

        bool operator>=(const const_iterator& _other) const noexcept
        {
            return p_ >= _other.p_;
        }

    private:
        pointer p_ = nullptr;
    };

}

#endif // !MEMEPP_ITERATORBASIC_HPP_INCLUDED
