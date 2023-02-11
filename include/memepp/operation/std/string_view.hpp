
#ifndef MEMEPP_OPERATION_STD_STRING_VIEW_HPP_INCLUDED
#define MEMEPP_OPERATION_STD_STRING_VIEW_HPP_INCLUDED

#include "memepp/string.hpp"
#include "memepp/string_view.hpp"
#include "memepp/string_builder.hpp"

#include <string_view>

namespace memepp {

	inline string_builder  operator+(const string& _lhs, const std::string_view& _rhs);
	inline string_builder  operator+(const std::string_view& _lhs, const string& _rhs);

	inline string_builder  operator+(const string_view& _lhs, const std::string_view& _rhs);
	inline string_builder  operator+(const std::string_view& _lhs, const string_view& _rhs);

	inline string_builder& operator+(string_builder& _sb, const std::string_view& _s);
	inline string_builder  operator+(string_builder&& _sb, const std::string_view& _s);

	inline string_builder& operator+(const std::string_view& _s, memepp::string_builder& _sb);
	inline string_builder  operator+(const std::string_view& _s, string_builder&& _sb);

    inline string_builder operator+(const string& _lhs, const std::string_view& _rhs)
    {
		return string_builder{} + _lhs + memepp::string_view{ _rhs.data(), _rhs.size() };
    }

    inline string_builder operator+(const std::string_view& _lhs, const string& _rhs)
    {
        return string_builder{} + memepp::string_view{ _lhs.data(), _lhs.size() } + _rhs;
    }

    inline string_builder operator+(const string_view& _lhs, const std::string_view& _rhs)
    {
        return string_builder{} + _lhs + memepp::string_view{ _rhs.data(), _rhs.size() };
    }

    inline string_builder operator+(const std::string_view& _lhs, const string_view& _rhs)
    {
        return string_builder{} + memepp::string_view{ _lhs.data(), _lhs.size() } + _rhs;
    }

    inline string_builder& operator+(string_builder& _sb, const std::string_view& _s)
    {
        return _sb + memepp::string_view{ _s.data(), _s.size() };
    }

    inline string_builder operator+(string_builder&& _sb, const std::string_view& _s)
    {
        return std::move(_sb + memepp::string_view{ _s.data(), _s.size() });
    }

    inline string_builder& operator+(const std::string_view& _s, memepp::string_builder& _sb)
    {
        return _sb + memepp::string_view{ _s.data(), _s.size() };
    }
	
    inline string_builder operator+(const std::string_view& _s, string_builder&& _sb)
    {
        return std::move(_sb + memepp::string_view{ _s.data(), _s.size() });
    }

    
};

#endif // !MEMEPP_OPERATION_STD_STRING_VIEW_HPP_INCLUDED
