
#ifndef MEMEPP_STRING_BUILDER_DEF_HPP_INCLUDED
#define MEMEPP_STRING_BUILDER_DEF_HPP_INCLUDED

#include "string_def.hpp"
#include "string_view_fwd.hpp"
#include "string_builder_fwd.hpp"

#include "meme/string_builder_fwd.h"

namespace memepp { 

	class string_builder
	{
		string_builder(const string_builder& _other);
		MEMEPP__IMPL_INLINE string_builder& operator=(const string_builder& _other);
	public:

		using native_handle_type = ::MemeStringBuilderStack_t;

		MEMEPP__IMPL_INLINE string_builder();
		string_builder(const string& _in);
		string_builder(const string& _in, string& _out);
		string_builder(string_builder&& _other);

		~string_builder();

		MEMEPP__IMPL_INLINE string_builder& operator=(string_builder&& _other);

		MEMEPP__IMPL_INLINE string release();

		MEMEPP__IMPL_INLINE const native_handle_type& native_handle() const noexcept;

	private:
		native_handle_type data_;
	};

	MEMEPP__IMPL_INLINE bool operator==(const string_builder& _lhs, const string& _rhs);
	MEMEPP__IMPL_INLINE bool operator==(const string& _lhs, const string_builder& _rhs);
	MEMEPP__IMPL_INLINE bool operator==(const string_builder& _lhs, const string_view& _rhs);
	MEMEPP__IMPL_INLINE bool operator==(const string_view& _lhs, const string_builder& _rhs);

	MEMEPP__IMPL_INLINE bool operator!=(const string_builder& _lhs, const string& _rhs);
	MEMEPP__IMPL_INLINE bool operator!=(const string& _lhs, const string_builder& _rhs);
	MEMEPP__IMPL_INLINE bool operator!=(const string_builder& _lhs, const string_view& _rhs);
	MEMEPP__IMPL_INLINE bool operator!=(const string_view& _lhs, const string_builder& _rhs);
};

#endif // !MEMEPP_STRING_BUILDER_DEF_HPP_INCLUDED
