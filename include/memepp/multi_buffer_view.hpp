
#ifndef MEMEPP_MULTI_BUFFER_VIEW_HPP_INCLUDED
#define MEMEPP_MULTI_BUFFER_VIEW_HPP_INCLUDED

#include <assert.h>
#include <mego/predef/lang/version.h>

#include <memepp/buffer.hpp>
#include <memepp/buffer_view.hpp>
#include <memepp/variable_buffer.hpp>
#include <megopp/err/err.h>

#include <vector>
#include <iterator>
#include <stdexcept>
#include <algorithm>
#if MG_LANG__CXX17_AVAIL
#include <string_view>
#endif

namespace memepp {

struct multi_buffer_view 
{
    using value_type = uint8_t;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = mmint_t;
    using difference_type = ptrdiff_t;

#if MG_LANG__CXX17_AVAIL
    inline static constexpr size_type npos = static_cast<size_type>(-1);
#else
    enum : size_type { npos = static_cast<size_type>(-1) };
#endif

    struct position {

        bool operator==(const position& other) const noexcept {
            return index == other.index && offset == other.offset;
        }

        bool operator!=(const position& other) const noexcept {
            return !(*this == other);
        }

        size_type index  = -1; // Index of the buffer_view in the vector
        size_type offset = -1; // Offset within that buffer_view
    };

    struct const_iterator
    {
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = uint8_t;
        using difference_type = ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;

        const_iterator(const multi_buffer_view* _view, position _pos) noexcept
            : view_(_view), pos_(_pos) {}

        const_iterator& operator++() {
            advance_to_next();
            return *this;
        }

        const_iterator& operator--() {
            retreat_to_prev();
            return *this;
        }

        const_iterator operator++(int) {
            auto temp = *this;
            ++(*this);
            return temp;
        }

        const_iterator operator--(int) {
            auto temp = *this;
            --(*this);
            return temp;
        }

        reference operator*() const;

        pointer operator->() const {
            return std::addressof(operator*());
        }

        bool operator==(const const_iterator& other) const noexcept {
            return view_ == other.view_ && pos_.index == other.pos_.index && pos_.offset == other.pos_.offset;
        }

        bool operator!=(const const_iterator& other) const noexcept {
            return !(*this == other);
        }

        position pos() const noexcept {
            return pos_;
        }

    private:
        void advance_to_next();
        void retreat_to_prev();

        const multi_buffer_view* view_;
        position pos_;
    };

    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    multi_buffer_view() = default;
    multi_buffer_view(const multi_buffer_view& _other) = default;

    multi_buffer_view(multi_buffer_view&& _other)
        : views_(std::move(_other.views_))
        , total_size_(_other.total_size_) 
    {
        _other.total_size_ = 0;
    }
    
    multi_buffer_view(const buffer_view& _view)
    {
        append(_view);
    }
    
#if MG_LANG__CXX17_AVAIL
    multi_buffer_view(const std::string_view& _view)
    {
        append(_view);
    }
#endif 

    multi_buffer_view(const uint8_t* _data, size_type _size)
    {
        append(_data, _size);
    }

    multi_buffer_view(const char* _data, size_type _size)
    {
        append(reinterpret_cast<const uint8_t*>(_data), _size);
    }
    
    multi_buffer_view(size_type _count, const buffer_view& _view)
    {
        append(_count, _view);
    }

#if MG_LANG__CXX17_AVAIL
    multi_buffer_view(size_type _count, const std::string_view& _view)
    {
        append(_count, _view);
    }
#endif

    multi_buffer_view(std::initializer_list<buffer_view> _views)
    {
        append(_views);
    }

    template <typename _It>
    multi_buffer_view(_It _begin, _It _end)
    {
        append(_begin, _end);
    }
    
    multi_buffer_view& operator=(const multi_buffer_view& _other) = default;

    multi_buffer_view& operator=(multi_buffer_view&& _other)
    {
        if (this != &_other) {
            views_ = std::move(_other.views_);
            total_size_ = _other.total_size_;
            _other.total_size_ = 0;
        }
        return *this;
    }

    multi_buffer_view& operator=(const buffer_view& _view) 
    {
        clear();
        append(_view);
        return *this;
    }

#if MG_LANG__CXX17_AVAIL
    multi_buffer_view& operator=(const std::string_view& _view) 
    {
        clear();
        append(_view);
        return *this;
    }
#endif

    multi_buffer_view& operator=(std::initializer_list<buffer_view> _views) 
    {
        clear();
        append(_views);
        return *this;
    }
    
    bool operator==(const multi_buffer_view& _other) const 
    {
        if (this == &_other) 
            return true;
        if (size() != _other.size()) 
            return false;

        if (views_.size() == _other.views_.size())
        {
            bool all_equal = true;
            for (size_t idx = 0; idx < views_.size(); ++idx)
            {
                if (views_[idx].first  != _other.views_[idx].first || 
                    views_[idx].second != _other.views_[idx].second)
                {
                    all_equal = false;
                    break;
                }
            }
            if (all_equal)
                return true;
        }

        auto it1 = cbegin();
        auto it2 = _other.cbegin();
        while (it1 != cend() && it2 != _other.cend()) {
            if (*it1 != *it2) 
                return false;
                
            ++it1; ++it2;
        }
        return true;
    }

    bool operator!=(const multi_buffer_view& _other) const 
    {
        return !(*this == _other);
    }

    void assign(const buffer_view& _view) 
    {
        clear();
        append(_view);
    }

#if MG_LANG__CXX17_AVAIL
    void assign(const std::string_view& _view) 
    {
        clear();
        append(_view);
    }
#endif 
    
    void assign(const uint8_t* _data, size_type _size) 
    {
        clear();
        append(_data, _size);
    }

    void assign(const multi_buffer_view& _other) 
    {
        clear();
        append(_other);
    }

    void assign(size_type _count, const buffer_view& _view) 
    {
        clear();
        append(_count, _view);
    }
    
#if MG_LANG__CXX17_AVAIL
    void assign(size_type _count, const std::string_view& _view) 
    {
        clear();
        append(_count, _view);
    }
#endif

    template <typename _It>
    void assign(_It _begin, _It _end)
    {
        clear();
        append(_begin, _end);
    }

    void assign(std::initializer_list<buffer_view> _views) 
    {
        clear();
        append(_views);
    }

    void append(const buffer_view& _view) {
        append(_view.data(), _view.size());
    }

#if MG_LANG__CXX17_AVAIL
    void append(const std::string_view& _view) {
        append(reinterpret_cast<const uint8_t*>(_view.data()), static_cast<size_type>(_view.size()));
    }
#endif

    void append(const uint8_t* _data, size_type _size) 
    {
        if (_data == nullptr)
            _size = 0;
        views_.emplace_back(std::make_pair(_data, _size));
        total_size_ += _size;
    }

    void append(const multi_buffer_view& _other) 
    {
        views_.insert(views_.end(), _other.views_.begin(), _other.views_.end());
        total_size_ += _other.size();
    }

    void append(size_type _count, const buffer_view& _view)
    {
        std::vector<std::pair<const uint8_t*, size_type>> new_views;
        new_views.reserve(_count);
        size_type total_size = 0;
        for (size_type idx = 0; idx < _count; ++idx) {
            new_views.emplace_back(_view.data(), _view.size());
            total_size += _view.size();
        }
        views_.insert(views_.end(), new_views.begin(), new_views.end());
        total_size_ += total_size;
    }

#if MG_LANG__CXX17_AVAIL
    void append(size_type _count, const std::string_view& _view)
    {
        std::vector<std::pair<const uint8_t*, size_type>> new_views;
        new_views.reserve(_count);
        size_type total_size = 0;
        for (size_type idx = 0; idx < _count; ++idx) {
            new_views.emplace_back(reinterpret_cast<const uint8_t*>(_view.data()), static_cast<size_type>(_view.size()));
            total_size += _view.size();
        }
        views_.insert(views_.end(), new_views.begin(), new_views.end());
        total_size_ += total_size;
    }
#endif

    void append(std::initializer_list<buffer_view> _views)
    {
        std::vector<std::pair<const uint8_t*, size_type>> new_views;
        new_views.reserve(_views.size());
        size_type new_size = 0;
        for (const auto& view : _views) {
            new_views.emplace_back(view.data(), view.size());
            new_size += view.size();
        }
        views_.insert(views_.end(), new_views.begin(), new_views.end());
        total_size_ += new_size;
    }

    template <typename _It>
    void append(_It _begin, _It _end)
    {
        std::vector<std::pair<const uint8_t*, size_type>> new_views;
        new_views.reserve(std::distance(_begin, _end));
        size_type total_size = 0;
        for (auto it = _begin; it != _end; ++it) {
            new_views.emplace_back(it->data(), it->size());
            total_size += it->size();
        }
        views_.insert(views_.end(), new_views.begin(), new_views.end());
        total_size_ += total_size;
    }

    void prepend(const buffer_view& _view) {
        prepend(_view.data(), _view.size());
    }

#if MG_LANG__CXX17_AVAIL
    void prepend(const std::string_view& _view) {
        prepend(reinterpret_cast<const uint8_t*>(_view.data()), static_cast<size_type>(_view.size()));
    }
#endif

    void prepend(const uint8_t* _data, size_type _size) 
    {
        if (_data == nullptr)
            _size = 0;
        views_.insert(views_.begin(), std::make_pair(_data, _size));
        total_size_ += _size;
    }

    void prepend(const multi_buffer_view& _other) 
    {
        views_.insert(views_.begin(), _other.views_.begin(), _other.views_.end());
        total_size_ += _other.size();
    }

    void prepend(size_type _count, const buffer_view& _view)
    {
        std::vector<std::pair<const uint8_t*, size_type>> new_views;
        new_views.reserve(_count);
        for (size_type idx = 0; idx < _count; ++idx) {
            new_views.emplace_back(_view.data(), _view.size());
        }
        views_.insert(views_.begin(), new_views.begin(), new_views.end());
        total_size_ += _count * _view.size();
    }

#if MG_LANG__CXX17_AVAIL
    void prepend(size_type _count, const std::string_view& _view)
    {
        std::vector<std::pair<const uint8_t*, size_type>> new_views;
        new_views.reserve(_count);
        for (size_type idx = 0; idx < _count; ++idx) {
            new_views.emplace_back(reinterpret_cast<const uint8_t*>(_view.data()), static_cast<size_type>(_view.size()));
        }
        views_.insert(views_.begin(), new_views.begin(), new_views.end());
        total_size_ += _count * _view.size();
    }
#endif 

    void prepend(std::initializer_list<buffer_view> _views)
    {
        views_.reserve(views_.size() + _views.size());
        std::vector<std::pair<const uint8_t*, size_type>> new_views;
        size_type new_size = 0;
        new_views.reserve(_views.size());
        for (const auto& view : _views) {
            new_views.emplace_back(view.data(), view.size());
            new_size += view.size();
        }
        views_.insert(views_.begin(), new_views.begin(), new_views.end());
        total_size_ += new_size;
    }

    template <typename _It>
    void prepend(_It _begin, _It _end)
    {
        std::vector<std::pair<const uint8_t*, size_type>> new_views;
        size_type new_size = 0;
        new_views.reserve(std::distance(_begin, _end));
        for (auto it = _begin; it != _end; ++it) {
            new_views.emplace_back(it->data(), it->size());
            new_size += it->size();
        }
        views_.insert(views_.begin(), new_views.begin(), new_views.end());
        total_size_ += new_size;
    }

    void pop_back() {
        auto pos = view_last_position(views_.size() - 1);
        if (pos.index >= static_cast<size_type>(views_.size()))
            return;

        total_size_ -= 1;
        views_[pos.index].second -= 1;
    }

    void pop_front() {
        auto pos = view_first_position(0);
        if (pos.index >= static_cast<size_type>(views_.size()))
            return;

        total_size_ -= 1;
        views_[pos.index].second -= 1;
        views_[pos.index].first  += 1;
    }

    void push_back(const buffer_view& _view) {
        append(_view);
    }

#if MG_LANG__CXX17_AVAIL
    void push_back(const std::string_view& _view) {
        push_back(reinterpret_cast<const uint8_t*>(_view.data()), static_cast<size_type>(_view.size()));
    }
#endif
    
    void push_back(const uint8_t* _data, size_type _size) {
        append(_data, _size);
    }

    void push_back(const multi_buffer_view& _other) {
        append(_other);
    }

    void push_front(const buffer_view& _view) {
        prepend(_view);
    }

#if MG_LANG__CXX17_AVAIL
    void push_front(const std::string_view& _view) {
        push_front(reinterpret_cast<const uint8_t*>(_view.data()), static_cast<size_type>(_view.size()));
    }
#endif 

    void push_front(const uint8_t* _data, size_type _size) {
        prepend(_data, _size);
    }

    void push_front(const multi_buffer_view& _other) {
        prepend(_other);
    }

    //! @return @c position pointing to the first element of the @c _view .
    //! If the @c _view is empty, move forward to find the next non-empty view and point to its first element.
    position insert(position _pos, const buffer_view& _view) {
        auto index = split_at(_pos);
        views_.insert(views_.begin() + index, std::make_pair(_view.data(), _view.size()));
        total_size_ += _view.size();
        return view_first_position(index);
    }

#if MG_LANG__CXX17_AVAIL
    //! @return @c position pointing to the first element of the @c _view .
    //! If the @c _view is empty, move forward to find the next non-empty view and point to its first element.
    position insert(position _pos, const std::string_view& _view) 
    {
        return insert(_pos, reinterpret_cast<const uint8_t*>(_view.data()), static_cast<size_type>(_view.size()));
    }
#endif 
    //! @return @c position pointing to the first element of the @c _data .
    //! If the @c _data is empty, move forward to find the next non-empty view and point to its first element.
    position insert(position _pos, const uint8_t* _data, size_type _size) {
        auto index = split_at(_pos);
        if (_data == nullptr)
            _size = 0;
        views_.insert(views_.begin() + index, std::make_pair(_data, _size));
        total_size_ += _size;
        return view_first_position(index);
    }

    //! @return @c position pointing to the first element of the first view in the @c multi_buffer_view .
    //! If the view is empty, move forward to find the next non-empty view and point to its first element.
    position insert(position _pos, const multi_buffer_view& _other) {
        if (this == &_other) {
            multi_buffer_view temp = _other;
            return insert(_pos, temp);
        }
        auto index = split_at(_pos);
        views_.insert(views_.begin() + index, _other.views_.begin(), _other.views_.end());
        total_size_ += _other.size();
        return view_first_position(index);
    }

    position insert(position _pos, size_type _count, const buffer_view& _view) {
        if (_count == 0)
            return _pos;
        auto buffer_index = split_at(_pos);
        views_.reserve(views_.size() + _count);
        for (size_type idx = 0; idx < _count; ++idx) {
            views_.insert(views_.begin() + buffer_index + idx, std::make_pair(_view.data(), _view.size()));
            total_size_ += _view.size();
        }
        return view_first_position(buffer_index);
    }

    //! @return @c position pointing to the first element of the first view in the @c _views .
    //! If the view is empty, move forward to find the next non-empty view and point to its first element.
    position insert(position _pos, std::initializer_list<buffer_view> _views) {
        if (_views.size() == 0)
            return _pos;
        views_.reserve(views_.size() + _views.size());
        auto buffer_index = split_at(_pos);
        std::vector<std::pair<const uint8_t*, size_type>> new_views;
        new_views.reserve(_views.size());
        size_type total_size = 0;
        for (const auto& view : _views) {
            new_views.emplace_back(view.data(), view.size());
            total_size += view.size();
        }
        views_.insert(views_.begin() + buffer_index, new_views.begin(), new_views.end());
        total_size_ += total_size;
        return view_first_position(buffer_index);
    }

    //! @return @c position pointing to the first element of the first view in the range [begin, end).
    //! If the view is empty, move forward to find the next non-empty view and point to its first element.
    template <typename _It>
    position insert(position _pos, _It _begin, _It _end) {
        if (_begin == _end)
            return _pos;
        auto buffer_index = split_at(_pos);
        views_.reserve(views_.size() + std::distance(_begin, _end));
        std::vector<std::pair<const uint8_t*, size_type>> new_views;
        new_views.reserve(std::distance(_begin, _end));
        size_type total_size = 0;
        for (auto it = _begin; it != _end; ++it) {
            new_views.emplace_back(it->data(), it->size());
            total_size += it->size();
        }
        views_.insert(views_.begin() + buffer_index, new_views.begin(), new_views.end());
        total_size_ += total_size;
        return view_first_position(buffer_index);
    }

    //! @return @c size_type pointing to the first element of the @c _view .
    //! If the @c _view is empty, move forward to find the next non-empty view and point to its first element.
    size_type insert(size_type _pos, const buffer_view& _view) 
    {
        auto pos = convert_position(_pos);
        if (pos.index < 0)
            pos = end_position();
        if (pos.offset < 0)
            pos.offset = 0;
        auto index = insert(pos, _view);
        return convert_position(index);
    }

#if MG_LANG__CXX17_AVAIL
    //! @return @c size_type pointing to the first element of the @c _view .
    //! If the @c _view is empty, move forward to find the next non-empty view and point to its first element.
    size_type insert(size_type _pos, const std::string_view& _view) 
    {
        return insert(_pos, reinterpret_cast<const uint8_t*>(_view.data()), static_cast<size_type>(_view.size()));
    }
#endif 

    //! @return @c size_type pointing to the first element of the @c _data .
    //! If the @c _data is empty, move forward to find the next non-empty view and point to its first element.
    size_type insert(size_type _pos, const uint8_t* _data, size_type _size) 
    {
        auto pos = convert_position(_pos);
        if (pos.index < 0)
            pos = end_position();
        if (pos.offset < 0)
            pos.offset = 0;
        auto index = insert(pos, _data, _size);
        return convert_position(index);
    }

    //! @return @c size_type pointing to the first element of the first view in the @c multi_buffer_view .
    //! If the view is empty, move forward to find the next non-empty view and point to its first element.
    size_type insert(size_type _pos, const multi_buffer_view& _other) 
    {
        auto pos = convert_position(_pos);
        if (pos.index < 0)
            pos = end_position();
        if (pos.offset < 0)
            pos.offset = 0;
        auto index = insert(pos, _other);
        return convert_position(index);
    }

    size_type insert(size_type _pos, size_type _count, const buffer_view& _view) 
    {
        auto pos = convert_position(_pos);
        if (pos.index < 0)
            pos = end_position();
        if (pos.offset < 0)
            pos.offset = 0;
        auto index = insert(pos, _count, _view);
        return convert_position(index);
    }

    //! @return @c size_type pointing to the first element of the first view in the @c _views .
    //! If the view is empty, move forward to find the next non-empty view and point to its first element.
    size_type insert(size_type _pos, std::initializer_list<buffer_view> _views) 
    {
        auto pos = convert_position(_pos);
        if (pos.index < 0)
            pos = end_position();
        if (pos.offset < 0)
            pos.offset = 0;
        auto index = insert(pos, _views);
        return convert_position(index);
    }

    //! @return @c size_type pointing to the first element of the first view in the range [begin, end).
    //! If the view is empty, move forward to find the next non-empty view and point to its first element.
    template <typename _It>
    size_type insert(size_type _pos, _It _begin, _It _end) 
    {
        auto pos = convert_position(_pos);
        if (pos.index < 0)
            pos = end_position();
        if (pos.offset < 0)
            pos.offset = 0;
        auto index = insert(pos, _begin, _end);
        return convert_position(index);
    }

    //! @return @c const_iterator pointing to the first element of the @c _view .
    //! If the @c _view is empty, move forward to find the next non-empty view and point to its first element.
    const_iterator insert(const_iterator _pos, const buffer_view& _view) 
    {
        auto pos = insert(_pos.pos(), _view);
        return const_iterator{ this, pos };
    }

#if MG_LANG__CXX17_AVAIL
    //! @return @c const_iterator pointing to the first element of the @c _view .
    //! If the @c _view is empty, move forward to find the next non-empty view and point to its first element.
    const_iterator insert(const_iterator _pos, const std::string_view& _view) 
    {
        return insert(_pos, reinterpret_cast<const uint8_t*>(_view.data()), static_cast<size_type>(_view.size()));
    }
#endif
    
    //! @return @c const_iterator pointing to the first element of the @c _data .
    //! If the @c _data is empty, move forward to find the next non-empty view and point to its first element.
    const_iterator insert(const_iterator _pos, const uint8_t* _data, size_type _size) 
    {
        auto pos = insert(_pos.pos(), _data, _size);
        return const_iterator{ this, pos };
    }

    //! @return @c const_iterator pointing to the first element of the first view in the @c multi_buffer_view .
    //! If the view is empty, move forward to find the next non-empty view and point to its first element.
    const_iterator insert(const_iterator _pos, const multi_buffer_view& _other) 
    {
        auto pos = insert(_pos.pos(), _other);
        return const_iterator{ this, pos };
    }

    const_iterator insert(const_iterator _pos, size_type _count, const buffer_view& _view) 
    {
        auto pos = insert(_pos.pos(), _count, _view);
        return const_iterator{ this, pos };
    }

    //! @return @c const_iterator pointing to the first element of the first view in the @c _views .
    //! If the view is empty, move forward to find the next non-empty view and point to its first element.
    const_iterator insert(const_iterator _pos, std::initializer_list<buffer_view> _views) 
    {
        auto pos = insert(_pos.pos(), _views);
        return const_iterator{ this, pos };
    }

    //! @return @c const_iterator pointing to the first element of the first view in the range [begin, end).
    //! If the view is empty, move forward to find the next non-empty view and point to its first element.
    template <typename _It>
    const_iterator insert(const_iterator _pos, _It _begin, _It _end) 
    {
        auto pos = insert(_pos.pos(), _begin, _end);
        return const_iterator{ this, pos };
    }

    //! @return @c position following the last removed element.
    //! If @c _pos refers to the last element, then the end() iterator is returned.
    position erase(position _pos)
    {
        return remove_byte(_pos);
    }

    //! @return @c position following the last removed element.
    //! 1. If @c _end == end() prior to removal, then the updated end() iterator is returned.
    //! 2. If [ @c _begin, @c _end ) is an empty range, then @c _end is returned.
    position erase(position _begin, position _end)
    {
        return remove_bytes(_begin, _end);
    }

    //! @return @c size_type following the last removed element.
    //! If @c _pos refers to the last element, then npos is returned.
    size_type erase(size_type _pos)
    {
        auto pos = convert_position(_pos);
        if (pos.index < 0 || pos.offset < 0)
            return npos;
        auto new_pos = remove_byte(pos);
        return convert_position(new_pos);
    }

    //! @return @c size_type following the last removed element.
    size_type erase(size_type _pos, size_type _count)
    {
        auto pos = convert_position(_pos);
        if (pos.index < 0 || pos.offset < 0)
            return npos;
        auto new_pos = remove_bytes(pos, _count);
        return convert_position(new_pos);
    }

    //! @return @c const_iterator following the last removed element.
    //! If @c _pos refers to the last element, then the end() iterator is returned.
    const_iterator erase(const_iterator _pos) 
    {
        auto pos = remove_byte(_pos.pos());
        return const_iterator{ this, pos };
    }

    //! @return @c const_iterator following the last removed element.
    //! 1. If @c _end == end() prior to removal, then the updated end() iterator is returned.
    //! 2. If [ @c _begin, @c _end ) is an empty range, then @c _end is returned.
    const_iterator erase(const_iterator _begin, const_iterator _end) 
    {
        auto pos = remove_bytes(_begin.pos(), _end.pos());
        return const_iterator{ this, pos };
    }

    position remove_byte(position _pos)
    {
        return remove_bytes(_pos, 1);
    }

    position remove_bytes(position _pos, size_type _count)
    {
        _pos = normalize(_pos);
        if (_pos.index >= static_cast<size_type>(views_.size()) || _count <= 0)
            return _pos;

        size_type bytes_remove = 0;
        while (_pos.index < static_cast<size_type>(views_.size()) && bytes_remove < _count)
        {
            auto& view = views_[_pos.index];
            if (view.second == 0) {
                ++_pos.index;
                _pos.offset = 0;
                continue;
            }

            auto bytes_to_remove = _count - bytes_remove;
            if (_pos.offset + bytes_to_remove > view.second)
                bytes_to_remove = view.second - _pos.offset;
            if (bytes_to_remove < 1)
                continue;
            
            if (_pos.offset + bytes_to_remove == view.second)
            {
                views_[_pos.index].second -= bytes_to_remove;
                bytes_remove += bytes_to_remove;
                total_size_  -= bytes_to_remove;
                _pos.index++;
                _pos.offset = 0;
                continue;
            }

            _pos.index  = split_at(_pos);
            _pos.offset = 0;
            if (_pos.index >= static_cast<size_type>(views_.size()))
                continue;
                
            if (_pos.offset == 0) {
                if (views_[_pos.index].second == bytes_to_remove)
                {
                    _pos.index++;
                    _pos.offset = 0;
                }
                views_[_pos.index].first  += bytes_to_remove;
                views_[_pos.index].second -= bytes_to_remove;
                bytes_remove += bytes_to_remove;
                total_size_  -= bytes_to_remove;
                continue;
            }

        }

        return _pos;
    }

    position remove_bytes(position _begin, position _end)
    {
        auto first = convert_position(_begin);
        auto last  = convert_position(_end);
        if (first == npos)
            first = 0;
        if (last == npos)
            last = size();
        if (first >= last)
            return _end;
        return remove_bytes(_begin, last - first);
    }

    void remove_view(size_type _index)
    {
        if (_index < 0 || _index >= static_cast<size_type>(views_.size()))
            return;

        total_size_ -= views_[_index].second;
        views_.erase(views_.begin() + _index);
    }

    void reset_view(size_type _index) 
    {
        if (_index < 0 || _index >= static_cast<size_type>(views_.size()))
            return;

        total_size_ -= views_[_index].second;
        views_[_index].second = 0;
        views_[_index].first  = nullptr;
    }

    position replace(position _pos, const buffer_view& _view) 
    {
        return replace(_pos, 1, _view);
    }

    position replace(position _pos, const uint8_t* _data, size_type _size) 
    {
        return replace(_pos, 1, _data, _size);
    }

    position replace(position _pos, size_type _count, const buffer_view& _view) 
    {
        return insert(remove_bytes(_pos, _count), _view);
    }

#if MG_LANG__CXX17_AVAIL
    position replace(position _pos, size_type _count, const std::string_view& _view) 
    {
        return replace(_pos, _count, reinterpret_cast<const uint8_t*>(_view.data()), static_cast<size_type>(_view.size()));
    }
#endif
    
    position replace(position _pos, size_type _count, const uint8_t* _data, size_type _size) 
    {
        return insert(remove_bytes(_pos, _count), _data, _size);
    }

    void compact() {
        // Remove empty views
        views_.erase(std::remove_if(views_.begin(), views_.end(),
            [](const auto& view) { return view.second == 0; }), views_.end());
    }

    void clear() {
        views_.clear();
        total_size_ = 0;
    }

    void swap(multi_buffer_view& _other) noexcept {
        std::swap(views_, _other.views_);
        std::swap(total_size_, _other.total_size_);
    }

    const_iterator begin() const MEGOPP__NOEXCEPT {
        return cbegin();
    }

    const_iterator cbegin() const MEGOPP__NOEXCEPT {
        return const_iterator{ this, view_first_position(0) };
    }

    const_iterator end() const MEGOPP__NOEXCEPT {
        return cend();
    }

    const_iterator cend() const MEGOPP__NOEXCEPT {
        return const_iterator{ this, end_position() };
    }

    const_reverse_iterator rbegin() const MEGOPP__NOEXCEPT {
        return crbegin();
    }

    const_reverse_iterator rend() const MEGOPP__NOEXCEPT {
        return crend();
    }

    const_reverse_iterator crbegin() const MEGOPP__NOEXCEPT {
        auto it = const_iterator{ this, view_last_position(views_.size() - 1) };
        if (it.pos() != end_position()) {
            it = std::next(it);
        }
        return const_reverse_iterator{ it };
    }

    const_reverse_iterator crend() const MEGOPP__NOEXCEPT {
        return const_reverse_iterator{ cbegin() };
    }

    constexpr size_type size() const MEGOPP__NOEXCEPT {
        return total_size_;
    }

    constexpr bool empty() const MEGOPP__NOEXCEPT {
        return total_size_ == 0;
    }

    const_reference at(position _pos) const 
    {
        _pos = view_first_position(_pos);
        if (_pos.index >= static_cast<size_type>(views_.size()))
            throw std::out_of_range("Position out of range");

        return *(views_[_pos.index].first + _pos.offset);
    }

    const_reference at(size_type _pos) const 
    {
        return at(convert_position(_pos));
    }

    const_reference operator[](position _pos) const 
    {
        assert(_pos.index >= 0 && _pos.offset >= 0);
        if (_pos.index < static_cast<size_type>(views_.size()) && _pos.offset >= views_[_pos.index].second)
            _pos = { _pos.index + 1, 0 };
        assert(_pos.index < static_cast<size_type>(views_.size()));

        return *(views_[_pos.index].first + _pos.offset);
    }

    const_reference operator[](size_type _pos) const 
    {
        return at(convert_position(_pos));
    }

    const_reference front() const 
    {
        auto ptr = data();
        if (ptr == nullptr)
            throw std::out_of_range("No elements in multi_buffer_view");
        return *ptr;
    }

    const_reference back() const 
    {
        auto ptr = back_data();
        if (ptr == nullptr)
            throw std::out_of_range("No elements in multi_buffer_view");
        return *ptr;
    }

    const_pointer data() const MEGOPP__NOEXCEPT 
    {
        size_t index = 0;
        while (index < views_.size() && views_[index].second == 0) {
            ++index;
        }
        if (index < views_.size())
            return views_[index].first;
        else
            return nullptr;
    }

    const_pointer back_data() const MEGOPP__NOEXCEPT 
    {
        size_type index = views_.size();
        while (index > 0 && views_[index - 1].second == 0) {
            --index;
        }
        if (index > 0)
            return views_[index - 1].first + views_[index - 1].second - 1;
        else
            return nullptr;
    }

    void reserve_views(size_type _count)
    {
        views_.reserve(_count);
    }

    size_type views_capacity() const MEGOPP__NOEXCEPT
    {
        return static_cast<size_type>(views_.capacity());
    }

    size_type view_count() const MEGOPP__NOEXCEPT 
    {
        return static_cast<size_type>(views_.size());
    }

    std::pair<const uint8_t*, size_type> view(size_type _index) const 
    {
        return views_.at(_index);
    }

    bool is_end(position _pos) const MEGOPP__NOEXCEPT 
    {
        _pos = normalize(_pos);
        return _pos.index >= static_cast<size_type>(views_.size());
    }

    position end_position() const MEGOPP__NOEXCEPT 
    {
        return { static_cast<size_type>(views_.size()), 0 };
    }

    position normalize(position _pos) const
    {        
        if (_pos.index == npos)
            return end_position();
        if (_pos.index < -1)
            _pos = { 0, 0 };
        if (_pos.offset < 0)
            _pos.offset = 0;

        if (_pos.index < static_cast<size_type>(views_.size())) 
        {
            if (_pos.offset >= views_[_pos.index].second)
                _pos = { _pos.index + 1, 0 };
        }
        if (_pos.index >= static_cast<size_type>(views_.size()))
            return end_position();

            return _pos;
    }

    position convert_position(size_type _pos) const MEGOPP__NOEXCEPT 
    {
        if (_pos >= size() || _pos == npos)
            return { npos, npos };
        if (_pos < 0)
            return { 0, 0 };

        size_type curr_offset = 0;
        for (size_t idx = 0; idx < views_.size(); ++idx) 
        {
            const auto& view = views_[idx];
            if (_pos < curr_offset + view.second)
                return { static_cast<size_type>(idx), _pos - curr_offset };

            curr_offset += view.second;
        }

        return { npos, npos };
    }

    size_type convert_position(position _pos) const MEGOPP__NOEXCEPT 
    {
        if (_pos.index < 0 || _pos.offset < 0)
            return npos;
        if (_pos.index < static_cast<size_type>(views_.size()) && _pos.offset >= views_[_pos.index].second)
            _pos = { _pos.index + 1, 0 };
        if (_pos.index >= static_cast<size_type>(views_.size()))
            return npos;

        size_type curr_offset = 0;
        for (size_type idx = 0; idx < _pos.index; ++idx)
            curr_offset += views_[idx].second;

        return curr_offset + _pos.offset;
    }

    position find(const buffer_view& _view, position _pos = { 0, 0 }) const 
    {
        return find(_view.data(), _view.size(), _pos);
    }

#if MG_LANG__CXX17_AVAIL
    position find(const std::string_view& _view, position _pos = { 0, 0 }) const 
    {
        return find(reinterpret_cast<const uint8_t*>(_view.data()), static_cast<size_type>(_view.size()), _pos);
    }
#endif

    position find(const uint8_t* _buf, size_type _size, position _pos = { 0, 0 }) const 
    {
        if (_buf == nullptr || _size < 1)
            return end_position();

        _pos = normalize(_pos);
        if (_pos.index >= static_cast<size_type>(views_.size()))
            return end_position();

        position start = end_position();
        size_type bytes_found = 0;
        while (_pos.index < static_cast<size_type>(views_.size()))
        {
            const auto& view = views_[_pos.index];
            if (view.second == 0) {
                _pos.offset = 0;
                ++_pos.index;
                continue;
            }

            if (start != end_position()) {
                size_type bytes_to_check = 
                    (std::min)(_size - bytes_found, view.second - _pos.offset);
                const uint8_t* data = view.first + _pos.offset;

                if (::memcmp(data, _buf + bytes_found, bytes_to_check) == 0)
                {
                    bytes_found += bytes_to_check;
                    if (bytes_found == _size)
                        return start;
                    _pos.offset += bytes_to_check;
                    if (_pos.offset >= view.second) {
                        ++_pos.index;
                        _pos.offset = 0;
                    }
                    continue;
                }
                else {
                    if (_pos.index == start.index)
                        _pos.offset = start.offset + 1;
                    else
                        _pos.offset = 0;
                    bytes_found = 0;
                    start = end_position();
                    continue;
                }
            }
            else {            
                size_type bytes_to_check = view.second - _pos.offset;
                auto first = memchr(view.first + _pos.offset, _buf[bytes_found], bytes_to_check);
                if (first == nullptr) {
                    ++_pos.index;
                    _pos.offset = 0;
                    continue;
                }
                _pos.offset = static_cast<size_type>(static_cast<const uint8_t*>(first) - view.first);
                start = { _pos.index, _pos.offset };
            }
        }
        
        return end_position();
    }

    mmint_t read_bytes(position _pos, uint8_t* _buffer, size_type _count) const 
    {        
        if (!_buffer)
            return MGEC__INVAL;
        _pos = normalize(_pos);
        if (_pos.index >= static_cast<size_type>(views_.size()))
            return MGEC__RANGE;

        size_type bytes_read = 0;
        while (bytes_read < _count && _pos.index < static_cast<size_type>(views_.size()))
        {
            const auto& view = views_[_pos.index];
            size_type bytes_to_copy = (std::min)(_count - bytes_read, view.second - _pos.offset);
            if (bytes_to_copy > 0) {
                ::memcpy(_buffer + bytes_read, view.first + _pos.offset, bytes_to_copy);
                bytes_read += bytes_to_copy;
            }
            ++_pos.index;
            _pos.offset = 0;
        }

        return bytes_read;
    }

    mgpp::err read_bytes(size_type _pos, uint8_t* _buffer, size_type _count) const 
    {
        auto result = read_bytes(convert_position(_pos), _buffer, _count);
        if (result < 0) {
            return mgpp::err{ static_cast<mgrc_t>(result) };
        }
        return {};
    }

    template <typename _Ty>
    mgpp::err read(position _pos, _Ty& _value, mgpp::endian_t _endian) const
    {
        typename megopp::type_with_size<sizeof(_Ty)>::uint value = 0;
        auto result = read_bytes(_pos, reinterpret_cast<uint8_t*>(&value), sizeof(value));
        if (result < 0) {
            return mgpp::err{ static_cast<mgrc_t>(result) };
        }
        if (result < sizeof(value)) {
            return mgpp::err{ MGEC__RANGE, "Not enough data" };
        }
#if MEGO_ENDIAN__LITTLE_BYTE
        if (_endian == megopp::endian_t::big_byte)
        {
			value = megopp::endian::byte_swap(value);
        }
#elif MEGO_ENDIAN__BIG_BYTE
        if (_endian == megopp::endian_t::little_byte)
        {
            value = megopp::endian::byte_swap(value);
        }
#endif
        ::memcpy(&_value, &value, sizeof(_value));
        return {};
    }

    template <typename _Ty>
    mgpp::err read(size_type _pos, _Ty& _value, mgpp::endian_t _endian) const
    {
        return read(convert_position(_pos), _value, _endian);
    }

    multi_buffer_view slice(position _pos = { 0, 0 }, size_type _count = npos) const
    {
        if (_pos.index < 0 || _pos.offset < 0)
            return {};
        if (_pos.index < static_cast<size_type>(views_.size()) && _pos.offset >= views_[_pos.index].second)
            _pos = { _pos.index + 1, 0 };
        if (_pos.index >= static_cast<size_type>(views_.size()))
            return {};

        if (_count == npos)
            _count = size() - convert_position(_pos);

        if (_count <= 0)
            return {};

        multi_buffer_view result;
        size_type bytes_collected = 0;

        while (bytes_collected < _count && _pos.index < static_cast<size_type>(views_.size()))
        {
            const auto& view = views_[_pos.index];
            size_type bytes_to_copy = (std::min)(_count - bytes_collected, view.second - _pos.offset);
            if (bytes_to_copy > 0) {
                result.append(view.first + _pos.offset, bytes_to_copy);
                bytes_collected += bytes_to_copy;
            }
            ++_pos.index;
            _pos.offset = 0;
        }

        return result;
    }

    multi_buffer_view slice(size_type _pos = 0, size_type _count = npos) const
    {
        return slice(convert_position(_pos), _count);
    }

    memepp::buffer to_buffer() const 
    {
        memepp::variable_buffer buf;
        buf.reserve(size());
        for (const auto& view : views_) {
            buf.append(view.first, view.second);
        }

        memepp::buffer result;
        buf.release(result);
        return result;
    }

    position view_first_position(size_type _index) const MEGOPP__NOEXCEPT
    {
        if (_index == npos)
            return end_position();
        if (_index < 0)
            _index = 0;

        while (_index < static_cast<size_type>(views_.size()) && views_[_index].second == 0) {
            ++_index;
        }
        if (_index >= static_cast<size_type>(views_.size()))
            return end_position();

        return { _index, 0 };
    }

    position view_first_position(position _pos) const MEGOPP__NOEXCEPT
    {
        auto first = view_first_position(_pos.index);
        if (first.index >= static_cast<size_type>(views_.size()))
            return end_position();
        
        if (first.index != _pos.index)
            return first;

        if (_pos.offset < 0 || _pos.offset >= views_[_pos.index].second)
            return end_position();
        
        return _pos;
    }

    position view_last_position(size_type _index) const MEGOPP__NOEXCEPT
    {
        if (_index >= static_cast<size_type>(views_.size()))
            return end_position();

        while (_index >= 0 && views_[_index].second == 0)
            --_index;

        if (_index < 0)
            return end_position();

        return { _index, views_[_index].second - 1 };
    }

    position view_last_position(position _pos) const MEGOPP__NOEXCEPT
    {
        auto last = view_last_position(_pos.index);
        if (last.index >= static_cast<size_type>(views_.size()))
            return end_position();

        if (last.index != _pos.index)
            return last;

        if (_pos.offset < 0 || _pos.offset >= views_[last.index].second)
            return end_position();

        return _pos;
    }

private:
    //! @return Buffer index of the split point
    size_type split_at(position _pos);

    std::vector< std::pair<const uint8_t*, size_type> > views_;
    size_type total_size_ = 0;
};

#if !MG_LANG__CXX17_AVAIL
#endif

inline multi_buffer_view::size_type multi_buffer_view::split_at(position _pos)
{
    if (_pos.index < 0 || _pos.offset < 0)
        return 0;

    if (_pos.index < static_cast<size_type>(views_.size()) && _pos.offset >= views_[_pos.index].second)
    {
        _pos.index += 1;
        _pos.offset = 0;
    }
    if (_pos.index >= static_cast<size_type>(views_.size()))
        return static_cast<size_type>(views_.size());

    if (_pos.offset == 0)
        return _pos.index;

    views_.insert(std::next(views_.begin(), _pos.index + 1), 
        std::make_pair(views_[_pos.index].first + _pos.offset, views_[_pos.index].second - _pos.offset));
    views_[_pos.index].second = _pos.offset;
    return _pos.index + 1;
}

inline multi_buffer_view::const_iterator::reference 
    multi_buffer_view::const_iterator::operator*() const
{
    return view_->at(pos_);
}

inline void multi_buffer_view::const_iterator::advance_to_next()
{
    if (pos_.index >= static_cast<size_type>(view_->views_.size())) 
    {
        pos_ = view_->end_position();
        return;
    }

    bool is_next_view = false;
    while (pos_.index < static_cast<size_type>(view_->views_.size()))
    {
        auto& view = view_->views_[pos_.index];
        if (view.second == 0 || (!is_next_view && pos_.offset + 1 >= view.second))
        {
            ++pos_.index;
            pos_.offset = 0;
            is_next_view = true;
            continue;
        }
        
        if (!is_next_view) {
            ++(pos_.offset);
        }
        return;
    }

    pos_ = view_->end_position();
}

inline void multi_buffer_view::const_iterator::retreat_to_prev()
{
    if (pos_.index == 0 && pos_.offset == 0) {
        return;
    }

    if (pos_.offset > 0) {
        --pos_.offset;
        return;
    } 
    
    while (pos_.index > 0) {
        auto& view = view_->views_[--pos_.index];
        pos_.offset = view.second - 1;
        if (view.second > 0) {
            return;
        }
    }

    pos_ = { 0, 0 };
}

}

#endif // !MEMEPP_MULTI_BUFFER_VIEW_HPP_INCLUDED
