
#ifndef MEMEPP_STRING_DEF_HPP_INCLUDED
#define MEMEPP_STRING_DEF_HPP_INCLUDED

#include "meme/string_fwd.h"
#include "memepp/string_fwd.hpp"

namespace std {
	template<class _Elem>
	class initializer_list;

	template<class CharT, class Traits, class Allocator> 
	class basic_string;
};

namespace memepp {

	class string final
	{
	public:
		using value_type = uint8_t;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using reference = value_type & ;
		using const_reference = const value_type&;
		using pointer = value_type * ;
		using const_pointer = const value_type*;

		using native_handle_type = MemeStringStack_t;

		static const size_type npos = static_cast<size_type>(-1);

		string() noexcept;
		string(native_handle_type&& _other);
		//string(const string& _other, string_storage_type _suggest);
		string(string&& _other);
		string(const string& _other);
		string(const string& _other, size_t _pos);
		string(const string& _other, size_t _pos, size_t _count);

		string(const char* _utf8);
		string(const char* _utf8, size_t _size);
		string(const uint8_t* _utf8);
		string(const uint8_t* _utf8, size_t _size);

		string(size_t _count, char _ch);

		string(const std::initializer_list<char>& _ilist);
		string(const std::initializer_list<uint8_t>& _ilist);

		template< class InputIt >
		string(InputIt _first, InputIt _last);

		string(std::nullptr_t) = delete;

		~string();

		string& operator=(string && _other);
		string& operator=(const string& _other);

		string_storage_type storage_type() const noexcept;

		const char* data() const noexcept;
		const char* c_str() const noexcept;


		size_t size() const noexcept;
		bool empty() const noexcept;
		size_t max_size() const noexcept;
		size_t capacity() const noexcept;

		template<class Traits, class Allocator>
		int compare(const std::basic_string<char, Traits, Allocator>& _str) const noexcept;
		void swap(string& _other) noexcept;

		size_type find(string& _other, size_type _pos = 0) const noexcept;

		const native_handle_type& native_handle() const noexcept;

	private:
		native_handle_type data_;
	};

	bool operator==(const string& _lhs, const string& _rhs);
	bool operator==(const char* _lhs, const string& _rhs);
	bool operator==(const string& _lhs, const char* _rhs);

	bool operator!=(const string& _lhs, const string& _rhs);
	bool operator!=(const char* _lhs, const string& _rhs);
	bool operator!=(const string& _lhs, const char* _rhs);

}; // namespace memepp

	inline memepp::string operator""_meme(const char* _str, size_t _len);

#endif // !MEMEPP_STRING_DEF_HPP_INCLUDED
