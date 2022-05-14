
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
		using size_type = MemeInteger_t;
		using difference_type = ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;

		using native_handle_type = MemeStringStack_t;

		static const size_type npos = static_cast<size_type>(-1);

		MEMEPP__IMPL_INLINE string_view() noexcept;
		MEMEPP__IMPL_INLINE string_view(const char* _utf8);
		MEMEPP__IMPL_INLINE string_view(const char* _utf8, size_type _size);
		MEMEPP__IMPL_INLINE string_view(const_pointer _utf8);
		MEMEPP__IMPL_INLINE string_view(const_pointer _utf8, size_type _size);

		MEMEPP__IMPL_INLINE string_view(const string& _other);
		MEMEPP__IMPL_INLINE string_view(const string_view& _other);
		MEMEPP__IMPL_INLINE string_view(string&& _other);
		MEMEPP__IMPL_INLINE string_view(string_view&& _other);

		MEMEPP__IMPL_INLINE ~string_view();

		string_view& operator=(const string& _other);
		string_view& operator=(const string_view& _other);
		string_view& operator=(string&& _other);
		string_view& operator=(string_view&& _other);


		MEMEPP__IMPL_INLINE const char* data() const noexcept;
		MEMEPP__IMPL_INLINE size_type size() const noexcept;

		MEMEPP__IMPL_INLINE string to_string() const;

		MEMEPP__IMPL_INLINE size_type index_of(const char* _utf8,
			MemeFlag_CaseSensitivity_t _cs = MemeFlag_AllSensitive) const noexcept;

		MEMEPP__IMPL_INLINE const native_handle_type& native_handle() const noexcept;

	private:
		native_handle_type data_;
	};

	MEMEPP__IMPL_INLINE bool operator==(const string_view& _lhs, const string_view& _rhs);
	MEMEPP__IMPL_INLINE bool operator!=(const string_view& _lhs, const string_view& _rhs);

	MEMEPP__IMPL_INLINE bool operator==(const string& _lhs, const string_view& _rhs);
	MEMEPP__IMPL_INLINE bool operator!=(const string& _lhs, const string_view& _rhs);

	MEMEPP__IMPL_INLINE bool operator==(const string_view& _lhs, const string& _rhs);
	MEMEPP__IMPL_INLINE bool operator!=(const string_view& _lhs, const string& _rhs);

	MEMEPP__IMPL_INLINE bool operator==(const char* _lhs, const string_view& _rhs);
	MEMEPP__IMPL_INLINE bool operator!=(const char* _lhs, const string_view& _rhs);

	MEMEPP__IMPL_INLINE bool operator==(const string_view& _lhs, const char* _rhs);
	MEMEPP__IMPL_INLINE bool operator!=(const string_view& _lhs, const char* _rhs);
};

	memepp::string_view operator""_meme_sv(const char* _str, size_t _len);


#endif // !MEMEPP_STRING_VIEW_DEF_HPP_INCLUDED
