
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
        using reference = MemeByte_t&;
        using pointer = MemeByte_t*;
        using const_reference = const MemeByte_t&;
        using const_pointer = const MemeByte_t*;
        using difference_type = MemeInteger_t;
        using iterator_category = std::random_access_iterator_tag;

        const_iterator() noexcept = default;
        const_iterator(const const_iterator&) noexcept = default;
        const_iterator(const_iterator&&) noexcept = default;
        const_iterator& operator=(const const_iterator&) noexcept = default;
        const_iterator& operator=(const_iterator&&) noexcept = default;
        ~const_iterator() noexcept = default;

        const_iterator(const_pointer _ptr) noexcept
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

        const_reference operator*() const noexcept
        {
            return *p_;
        }

        const_pointer operator->() const noexcept
        {
            return p_;
        }

        const_reference operator[](difference_type _n) const noexcept
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
        const_pointer p_ = nullptr;
    };

    class reverse_iterator
    {
    public:
        using value_type = MemeByte_t;
        using reference = MemeByte_t&;
        using pointer = MemeByte_t*;
        using difference_type = MemeInteger_t;
        using iterator_category = std::random_access_iterator_tag;

        reverse_iterator() noexcept = default;
        reverse_iterator(const reverse_iterator&) noexcept = default;
        reverse_iterator(reverse_iterator&&) noexcept = default;
        reverse_iterator& operator=(const reverse_iterator&) noexcept = default;
        reverse_iterator& operator=(reverse_iterator&&) noexcept = default;
        ~reverse_iterator() noexcept = default;

        reverse_iterator(pointer _ptr) noexcept
            : p_(_ptr)
        {}

        reverse_iterator& operator++() noexcept
        {
            --p_;
            return *this;
        }

        reverse_iterator operator++(int) noexcept
        {
            reverse_iterator tmp(*this);
            --p_;
            return tmp;
        }

        reverse_iterator& operator--() noexcept
        {
            ++p_;
            return *this;
        }

        reverse_iterator operator--(int) noexcept
        {
            reverse_iterator tmp(*this);
            ++p_;
            return tmp;
        }

        reverse_iterator& operator+=(difference_type _n) noexcept
        {
            p_ -= _n;
            return *this;
        }

        reverse_iterator operator+(difference_type _n) const noexcept
        {
            return reverse_iterator(p_ - _n);
        }

        reverse_iterator& operator-=(difference_type _n) noexcept
        {
            p_ += _n;
            return *this;
        }

        reverse_iterator operator-(difference_type _n) const noexcept
        {
            return reverse_iterator(p_ + _n);
        }

        difference_type operator-(const reverse_iterator& _rhs) const noexcept
        {
            return _rhs.p_ - p_;
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
            return p_[-_n];
        }

        bool operator==(const reverse_iterator& _other) const noexcept
        {
            return p_ == _other.p_;
        }

        bool operator!=(const reverse_iterator& _other) const noexcept
        {
            return p_ != _other.p_;
        }

        bool operator<(const reverse_iterator& _other) const noexcept
        {
            return p_ > _other.p_;
        }
        
        bool operator<=(const reverse_iterator& _other) const noexcept
        {
            return p_ >= _other.p_;
        }

        bool operator>(const reverse_iterator& _other) const noexcept
        {
            return p_ < _other.p_;
        }

        bool operator>=(const reverse_iterator& _other) const noexcept
        {
            return p_ <= _other.p_;
        }

        iterator base() const noexcept
        {
            return iterator(p_ + 1);
        }

    private:
        pointer p_ = nullptr;
    };

    class const_reverse_iterator
    {
    public:
        using value_type = MemeByte_t;
        using reference = MemeByte_t&;
        using pointer = MemeByte_t*;
        using const_reference = const MemeByte_t&;
        using const_pointer = const MemeByte_t*;
        using difference_type = MemeInteger_t;
        using iterator_category = std::random_access_iterator_tag;

        const_reverse_iterator() noexcept = default;
        const_reverse_iterator(const const_reverse_iterator&) noexcept = default;
        const_reverse_iterator(const_reverse_iterator&&) noexcept = default;
        const_reverse_iterator& operator=(const const_reverse_iterator&) noexcept = default;
        const_reverse_iterator& operator=(const_reverse_iterator&&) noexcept = default;
        ~const_reverse_iterator() noexcept = default;

        const_reverse_iterator(const_pointer _ptr) noexcept
            : p_(_ptr)
        {}

        const_reverse_iterator& operator++() noexcept
        {
            --p_;
            return *this;
        }

        const_reverse_iterator operator++(int) noexcept
        {
            const_reverse_iterator tmp(*this);
            --p_;
            return tmp;
        }

        const_reverse_iterator& operator--() noexcept
        {
            ++p_;
            return *this;
        }

        const_reverse_iterator operator--(int) noexcept
        {
            const_reverse_iterator tmp(*this);
            ++p_;
            return tmp;
        }

        const_reverse_iterator& operator+=(difference_type _n) noexcept
        {
            p_ -= _n;
            return *this;
        }

        const_reverse_iterator operator+(difference_type _n) const noexcept
        {
            return const_reverse_iterator(p_ - _n);
        }

        const_reverse_iterator& operator-=(difference_type _n) noexcept
        {
            p_ += _n;
            return *this;
        }

        const_reverse_iterator operator-(difference_type _n) const noexcept
        {
            return const_reverse_iterator(p_ + _n);
        }

        difference_type operator-(const const_reverse_iterator& _rhs) const noexcept
        {
            return _rhs.p_ - p_;
        }

        const_reference operator*() const noexcept
        {
            return *p_;
        }

        const_pointer operator->() const noexcept
        {
            return p_;
        }

        const_reference operator[](difference_type _n) const noexcept
        {
            return p_[-_n];
        }

        bool operator==(const const_reverse_iterator& _other) const noexcept
        {
            return p_ == _other.p_;
        }

        bool operator!=(const const_reverse_iterator& _other) const noexcept
        {
            return p_ != _other.p_;
        }

        bool operator<(const const_reverse_iterator& _other) const noexcept
        {
            return p_ > _other.p_;
        }

        bool operator<=(const const_reverse_iterator& _other) const noexcept
        {
            return p_ >= _other.p_;
        }

        bool operator>(const const_reverse_iterator& _other) const noexcept
        {
            return p_ < _other.p_;
        }

        bool operator>=(const const_reverse_iterator& _other) const noexcept
        {
            return p_ <= _other.p_;
        }

        const_iterator base() const noexcept
        {
            return const_iterator(p_ + 1);
        }
        
    private:
        const_pointer p_ = nullptr;
    };
}

#endif // !MEMEPP_ITERATORBASIC_HPP_INCLUDED
