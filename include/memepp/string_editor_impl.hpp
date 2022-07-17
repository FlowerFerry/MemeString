
#ifndef MEMEPP_STRING_EDITOR_IMPL_HPP_INCLUDED
#define MEMEPP_STRING_EDITOR_IMPL_HPP_INCLUDED

#include "string_view_def.hpp"
#include "string_editor_def.hpp"

#include "meme/string_editor.h"

namespace memepp {

	MEMEPP__IMPL_INLINE string_editor::string_editor()
	{
		MemeStringEditorStack_init(&data_, MEME_STRING__EDITOR_SIZE);
	}
	
	MEMEPP__IMPL_INLINE string_editor::string_editor(const string& _in)
	{
		MemeStringEditorStack_initWithString(
			&data_, MEME_STRING__EDITOR_SIZE,
			memepp::to_pointer(_in.native_handle()), MEME_STRING__OBJECT_SIZE
		);
	}

	MEMEPP__IMPL_INLINE string_editor::string_editor(const string& _in, string& _out)
	{
		auto pointer = const_cast<MemeStringStack_t*>(&_out.native_handle());
		MemeStringEditorStack_initAssociated(
			&data_, MEME_STRING__EDITOR_SIZE,
			memepp::to_pointer(_in.native_handle()), 
			pointer, MEME_STRING__OBJECT_SIZE
		);
	}

	MEMEPP__IMPL_INLINE string_editor::string_editor(string_editor&& _other)
	{
		MemeStringEditorStack_swap(&data_, &(_other.data_), MEME_STRING__EDITOR_SIZE);
	}

	MEMEPP__IMPL_INLINE string_editor::~string_editor()
	{
		MemeStringEditorStack_unInit(&data_, MEME_STRING__EDITOR_SIZE);
	}

	MEMEPP__IMPL_INLINE string_editor& string_editor::operator=(string_editor&& _other)
	{
		MemeStringEditorStack_swap(&data_, &(_other.data_), MEME_STRING__EDITOR_SIZE);
		return *this;
	}

	MEMEPP__IMPL_INLINE string string_editor::release()
	{
		string s;
		auto pointer = const_cast<MemeStringStack_t*>(&s.native_handle());
		MemeStringEditorStack_release(
			&data_, MEME_STRING__EDITOR_SIZE, pointer, MEME_STRING__OBJECT_SIZE);
		return s;
	}

	MEMEPP__IMPL_INLINE const string_editor::native_handle_type& string_editor::native_handle() const noexcept
	{
		return data_;
	}

	MEMEPP__IMPL_INLINE bool operator==(const string_editor& _lhs, const string& _rhs)
	{
		int result = 0;
		MemeStringEditorStack_isEqualWithString(
			&(_lhs.native_handle()),
			MEME_STRING__EDITOR_SIZE,
			to_pointer(_rhs.native_handle()), &result);
		return result;
	}

	MEMEPP__IMPL_INLINE bool operator==(const string& _lhs, const string_editor& _rhs)
	{
		int result = 0;
		MemeStringEditorStack_isEqualWithString(
			&(_rhs.native_handle()),
			MEME_STRING__EDITOR_SIZE,
			to_pointer(_lhs.native_handle()), &result);
		return result;
	}

	MEMEPP__IMPL_INLINE bool operator==(const string_editor& _lhs, const string_view& _rhs)
	{
		int result = 0;
		MemeStringEditorStack_isEqualWithString(
			&(_lhs.native_handle()),
			MEME_STRING__EDITOR_SIZE,
			to_pointer(_rhs.native_handle()), &result);
		return result;
	}
	
	MEMEPP__IMPL_INLINE bool operator==(const string_view& _lhs, const string_editor& _rhs)
	{
		int result = 0;
		MemeStringEditorStack_isEqualWithString(
			&(_rhs.native_handle()),
			MEME_STRING__EDITOR_SIZE,
			to_pointer(_lhs.native_handle()), &result);
		return result;
	}

	MEMEPP__IMPL_INLINE bool operator!=(const string_editor& _lhs, const string& _rhs)
	{
		!(_lhs == _rhs);
	}

	MEMEPP__IMPL_INLINE bool operator!=(const string& _lhs, const string_editor& _rhs)
	{
		!(_lhs == _rhs);
	}
	
	MEMEPP__IMPL_INLINE bool operator!=(const string_editor& _lhs, const string_view& _rhs)
	{
		!(_lhs == _rhs);
	}

	MEMEPP__IMPL_INLINE bool operator!=(const string_view& _lhs, const string_editor& _rhs)
	{
		!(_lhs == _rhs);
	}
};

#endif // !MEMEPP_STRING_EDITOR_IMPL_HPP_INCLUDED
