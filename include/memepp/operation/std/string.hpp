
#ifndef MEMEPP_OPERATION_STD_STRING_HPP_INCLUDED
#define MEMEPP_OPERATION_STD_STRING_HPP_INCLUDED

#include "memepp/string.hpp"
#include "memepp/string_view.hpp"
#include "memepp/string_builder.hpp"
#include "memepp/convert/common_def.hpp"

#include <string>

namespace memepp {
    
	inline string_builder  operator+(const string& _lhs, const std::string& _rhs);
	inline string_builder  operator+(const string& _lhs, std::string&& _rhs);
	inline string_builder  operator+(const std::string& _lhs, const string& _rhs);
	inline string_builder  operator+(std::string&& _lhs, const string& _rhs);
    
	inline string_builder  operator+(const string_view& _lhs, const std::string& _rhs);
	inline string_builder  operator+(const string_view& _lhs, std::string&& _rhs);
	inline string_builder  operator+(const std::string& _lhs, const string_view& _rhs);
	inline string_builder  operator+(std::string&& _lhs, const string_view& _rhs);

	inline string_builder& operator+(string_builder& _sb, const std::string& _s);
	inline string_builder  operator+(string_builder&& _sb, const std::string& _s);
	inline string_builder& operator+(string_builder& _sb, std::string&& _s);
	inline string_builder  operator+(string_builder&& _sb, std::string&& _s);

	inline string_builder& operator+(const std::string& _s, memepp::string_builder& _sb);
	inline string_builder  operator+(const std::string& _s, string_builder&& _sb);
	inline string_builder& operator+(std::string&& _s, string_builder& _sb);
	inline string_builder  operator+(std::string&& _s, string_builder&& _sb);

    inline string_builder operator+(const string& _lhs, const std::string& _rhs)
	{
		return string_builder{} + _lhs + _rhs;
    }

	inline string_builder operator+(const string& _lhs, std::string&& _rhs)
	{
        return string_builder{} + _lhs + std::move(_rhs);
	}

    inline string_builder operator+(const std::string& _lhs, const string& _rhs)
    {
        return string_builder{} + _lhs + _rhs;
    }

    inline string_builder operator+(std::string&& _lhs, const string& _rhs)
    {
        return string_builder{} + std::move(_lhs) + _rhs;
    }

    inline string_builder operator+(const string_view& _lhs, const std::string& _rhs)
    {
        return string_builder{} + _lhs + _rhs;
    }

    inline string_builder operator+(const string_view& _lhs, std::string&& _rhs)
    {
        return string_builder{} + _lhs + std::move(_rhs);
    }

    inline string_builder operator+(const std::string& _lhs, const string_view& _rhs)
    {
        return string_builder{} + _lhs + _rhs;
    }

    inline string_builder operator+(std::string&& _lhs, const string_view& _rhs)
    {
        return string_builder{} + std::move(_lhs) + _rhs;
    }
    
	inline string_builder& operator+(string_builder& _sb, const std::string& _s)
	{
		return _sb + mm_view(_s);
	}

	inline string_builder operator+(string_builder&& _sb, const std::string& _s)
	{
		return _sb + mm_view(_s);
	}

	inline string_builder& operator+(string_builder& _sb, std::string&& _s)
	{
		return _sb + mm_from(_s);
	}

	inline string_builder operator+(string_builder&& _sb, std::string&& _s)
	{
		return _sb + mm_from(_s);
	}

	inline string_builder& operator+(const std::string& _s, memepp::string_builder& _sb)
	{
		return mm_view(_s) + _sb;
	}

	inline string_builder operator+(const std::string& _s, string_builder&& _sb)
	{
		return _s + _sb;
	}

	inline string_builder& operator+(std::string&& _s, string_builder& _sb)
	{
		return mm_from(_s) + _sb;
	}

	inline string_builder operator+(std::string&& _s, string_builder&& _sb)
	{
		return _s + _sb;
	}

}; // namespace memepp

#endif // !MEMEPP_OPERATION_STD_STRING_HPP_INCLUDED