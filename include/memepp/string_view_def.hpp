
#ifndef MEMEPP_STRING_VIEW_DEF_HPP_INCLUDED
#define MEMEPP_STRING_VIEW_DEF_HPP_INCLUDED

#include "meme/string_fwd.h"
#include "memepp/string_fwd.hpp"
#include "memepp/string_view_fwd.hpp"

namespace memepp {

	class string_view final
	{
	public:
		using value_type = uint8_t;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;

		using native_handle_type = MemeStringStack_t;

		static const size_type npos = static_cast<size_type>(-1);

		string_view() noexcept;
		string_view(const char* _utf8);
		string_view(const char* _utf8, size_t _size);
		string_view(const uint8_t* _utf8);
		string_view(const uint8_t* _utf8, size_t _size);

		string_view(const string& _other);
		string_view(const string_view& _other);
		string_view(string&& _other);
		string_view(string_view&& _other);

		string_view& operator=(const string& _other);
		string_view& operator=(const string_view& _other);
		string_view& operator=(string&& _other);
		string_view& operator=(string_view&& _other);

		const char* data() const noexcept;
		size_t size() const noexcept;

		string to_string() const;

		const native_handle_type& native_handle() const noexcept;

	private:
		native_handle_type data_;
	};

	bool operator==(const string_view& _lhs, const string_view& _rhs);
	bool operator!=(const string_view& _lhs, const string_view& _rhs);
};

#endif // !MEMEPP_STRING_VIEW_DEF_HPP_INCLUDED
