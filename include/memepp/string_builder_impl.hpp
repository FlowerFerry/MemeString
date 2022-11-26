
#ifndef MEMEPP_STRING_EDITOR_IMPL_HPP_INCLUDED
#define MEMEPP_STRING_EDITOR_IMPL_HPP_INCLUDED

#include "string_view_def.hpp"
#include "string_builder_def.hpp"

#include "meme/string_builder.h"

namespace memepp {

	MEMEPP__IMPL_INLINE string_builder::string_builder()
	{
		MemeStringBuilderStack_init(&data_, MEME_STRING_BUILDER__OBJECT_SIZE);
	}
	
	MEMEPP__IMPL_INLINE string_builder::string_builder(const string& _in)
	{
		MemeStringBuilderStack_initWithString(
			&data_, MEME_STRING_BUILDER__OBJECT_SIZE,
			memepp::to_pointer(_in.native_handle()), MEME_STRING__OBJECT_SIZE
		);
	}

	MEMEPP__IMPL_INLINE string_builder::string_builder(const string& _in, string& _out)
	{
		auto pointer = const_cast<MemeStringStack_t*>(&_out.native_handle());
		MemeStringBuilderStack_initAssociated(
			&data_, MEME_STRING_BUILDER__OBJECT_SIZE,
			memepp::to_pointer(_in.native_handle()), 
			pointer, MEME_STRING__OBJECT_SIZE
		);
	}

	MEMEPP__IMPL_INLINE string_builder::string_builder(string_builder&& _other)
	{
		MemeStringBuilderStack_swap(&data_, &(_other.data_), MEME_STRING_BUILDER__OBJECT_SIZE);
	}

	MEMEPP__IMPL_INLINE string_builder::~string_builder()
	{
		MemeStringBuilderStack_unInit(&data_, MEME_STRING_BUILDER__OBJECT_SIZE);
	}

	MEMEPP__IMPL_INLINE string_builder& string_builder::operator=(string_builder&& _other)
	{
		MemeStringBuilderStack_swap(&data_, &(_other.data_), MEME_STRING_BUILDER__OBJECT_SIZE);
		return *this;
	}

	MEMEPP__IMPL_INLINE string string_builder::release()
	{
		string s;
		auto pointer = const_cast<MemeStringStack_t*>(&s.native_handle());
		MemeStringBuilderStack_release(
			&data_, MEME_STRING_BUILDER__OBJECT_SIZE, pointer, MEME_STRING__OBJECT_SIZE);
		return s;
	}

	MEMEPP__IMPL_INLINE const string_builder::native_handle_type& string_builder::native_handle() const noexcept
	{
		return data_;
	}

	MEMEPP__IMPL_INLINE bool operator==(const string_builder& _lhs, const string& _rhs)
	{
		int result = 0;
		MemeStringBuilderStack_isEqualWithString(
			&(_lhs.native_handle()),
			MEME_STRING_BUILDER__OBJECT_SIZE,
			to_pointer(_rhs.native_handle()), &result);
		return result;
	}

	MEMEPP__IMPL_INLINE bool operator==(const string& _lhs, const string_builder& _rhs)
	{
		int result = 0;
		MemeStringBuilderStack_isEqualWithString(
			&(_rhs.native_handle()),
			MEME_STRING_BUILDER__OBJECT_SIZE,
			to_pointer(_lhs.native_handle()), &result);
		return result;
	}

	MEMEPP__IMPL_INLINE bool operator==(const string_builder& _lhs, const string_view& _rhs)
	{
		int result = 0;
		MemeStringBuilderStack_isEqualWithString(
			&(_lhs.native_handle()),
			MEME_STRING_BUILDER__OBJECT_SIZE,
			to_pointer(_rhs.native_handle()), &result);
		return result;
	}
	
	MEMEPP__IMPL_INLINE bool operator==(const string_view& _lhs, const string_builder& _rhs)
	{
		int result = 0;
		MemeStringBuilderStack_isEqualWithString(
			&(_rhs.native_handle()),
			MEME_STRING_BUILDER__OBJECT_SIZE,
			to_pointer(_lhs.native_handle()), &result);
		return result;
	}

	MEMEPP__IMPL_INLINE bool operator!=(const string_builder& _lhs, const string& _rhs)
	{
		!(_lhs == _rhs);
	}

	MEMEPP__IMPL_INLINE bool operator!=(const string& _lhs, const string_builder& _rhs)
	{
		!(_lhs == _rhs);
	}
	
	MEMEPP__IMPL_INLINE bool operator!=(const string_builder& _lhs, const string_view& _rhs)
	{
		!(_lhs == _rhs);
	}

	MEMEPP__IMPL_INLINE bool operator!=(const string_view& _lhs, const string_builder& _rhs)
	{
		!(_lhs == _rhs);
	}
};

#endif // !MEMEPP_STRING_EDITOR_IMPL_HPP_INCLUDED
