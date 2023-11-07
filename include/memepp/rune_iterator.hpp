
#ifndef MEMEPP_RUNE_ITERATOR_HPP_INCLUDED
#define MEMEPP_RUNE_ITERATOR_HPP_INCLUDED

#include "rune.hpp"

namespace memepp {

    class const_rune_iterator
    {
    public:
        using value_type = mmbyte_t;
        using size_type = mmint_t;
        using difference_type = ptrdiff_t;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using iterator_category = std::bidirectional_iterator_tag;

        static const int8_t invalid_size =  0;
        static const int8_t invalid_code = -1;

        const_rune_iterator() noexcept
            : begin_(nullptr)
            , end_(nullptr)
            , ptr_(nullptr)
            , prev_size_(invalid_code)
            , curr_size_(invalid_code)
        {
        }

        const_rune_iterator(const_pointer _begin, const_pointer _end) noexcept
            : begin_(_begin)
            , end_(_end)
            , ptr_(_begin)
            , prev_size_(invalid_code)
            , curr_size_(invalid_size)
        {
        }
        
        const_rune_iterator(const_pointer _ptr, const_pointer _begin, const_pointer _end) noexcept
            : begin_(_begin)
            , end_(_end)
            , ptr_(_ptr)
            , prev_size_(invalid_size)
            , curr_size_(_ptr != _end ? invalid_size : invalid_code)
        {
        }

        const_rune_iterator(const const_rune_iterator& _other) noexcept = default;
        const_rune_iterator(const_rune_iterator&& _other) noexcept = default;
        const_rune_iterator& operator=(const const_rune_iterator& _other) noexcept = default;
        const_rune_iterator& operator=(const_rune_iterator&& _other) noexcept = default;
        ~const_rune_iterator() noexcept = default;

        const_rune_iterator& operator++() noexcept
        {
            if (ptr_ >= end_) {
                curr_size_ = invalid_code;
                return *this;
            }

            if (curr_size_ == invalid_size)
                curr_size_ = mmutf_u8rune_valid(ptr_, end_ - ptr_);

            if (curr_size_ == invalid_code) {
                return *this;
            }
            
            ptr_ += curr_size_;
            prev_size_ = curr_size_;
            curr_size_ = mmutf_u8rune_valid(ptr_, end_ - ptr_);

            return *this;
        }

        const_rune_iterator operator++(int) noexcept
        {
            const_rune_iterator tmp(*this);
            ++(*this);
            return tmp;
        }

        const_rune_iterator& operator--() noexcept
        {
            if (ptr_ <= begin_) {
                prev_size_ = invalid_code;
                return *this;
            }
            
            if (prev_size_ == invalid_size) {
                prev_size_ = get_prev_rune_size(begin_, ptr_);
            }

            if (prev_size_ == invalid_code) {
                return *this;
            }

            ptr_ -= prev_size_;
            curr_size_ = prev_size_;
            prev_size_ = get_prev_rune_size(begin_, ptr_);

            return *this;
        }

        const_rune_iterator operator--(int) noexcept
        {
            const_rune_iterator tmp(*this);
            --(*this);
            return tmp;
        }

        const_reference operator*() const noexcept
        {
            return *ptr_;
        }

        const_pointer operator->() const noexcept
        {
            return ptr_;
        }
        
        bool operator==(const const_rune_iterator& _other) const noexcept
        {
            if (curr_size_ == invalid_code) 
            {
                if (_other.curr_size_ == invalid_code)
                    return true;
                else
                    return end_ == _other.ptr_;
            }
            if (prev_size_ == invalid_code)
            {
                if (_other.prev_size_ == invalid_code)
                    return true;
                else
                    return begin_ == _other.ptr_;
            }

            return ptr_ == _other.ptr_;
        }

        bool operator!=(const const_rune_iterator& _other) const noexcept
        {
            return !(*this == _other);
        }

        MemeRuneIndex_t to_index() const noexcept
        {
            if (curr_size_ == invalid_size)
                curr_size_ = mmutf_u8rune_valid(ptr_, end_ - ptr_);
            
            return { curr_size_, ptr_ };
        }

        rune to_rune() const noexcept
        {
            if (curr_size_ == invalid_size)
                curr_size_ = mmutf_u8rune_valid(ptr_, end_ - ptr_);

            return rune{ ptr_, curr_size_ };
        }

    private:
        static int8_t get_prev_rune_size(const_pointer _begin, const_pointer _curr)
        {
            if (_curr <= _begin) {
                return -1;
            }

            for (auto index = _curr - 1; index >= _begin; --index)
            {
                if (mmutf_u8rune_char_size(*index) < 0)
                    continue;
                
                auto byte_size = mmutf_u8rune_valid(index, _curr - index);
                if (byte_size < 0)
                    break;

                return byte_size;
            }

            return -1;
        }
        
        const_pointer begin_;
        const_pointer end_;
        const_pointer ptr_;
        int8_t prev_size_;
        mutable int8_t curr_size_;
    };
};

#endif // !MEMEPP_RUNE_ITERATOR_HPP_INCLUDED
