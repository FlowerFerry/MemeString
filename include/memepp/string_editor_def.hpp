
#ifndef MEMEPP_STRING_EDITOR_DEF_HPP_INCLUDED
#define MEMEPP_STRING_EDITOR_DEF_HPP_INCLUDED

#include "string_def.hpp"
#include "string_view_fwd.hpp"
#include "string_editor_fwd.hpp"

#include "meme/string_editor_fwd.h"

namespace memepp { 

	class string_editor
	{
		string_editor(const string_editor& _other);
		MEMEPP__IMPL_INLINE string_editor& operator=(const string_editor& _other);
	public:

		using native_handle_type = MemeStringEditorStack_t;

		string_editor();
		string_editor(const string& _in);
		string_editor(const string& _in, string& _out);
		string_editor(string_editor&& _other);

		~string_editor();

		MEMEPP__IMPL_INLINE string_editor& operator=(string_editor&& _other);

		MEMEPP__IMPL_INLINE string release();

		MEMEPP__IMPL_INLINE const native_handle_type& native_handle() const noexcept;

	private:
		native_handle_type data_;
	};

	MEMEPP__IMPL_INLINE bool operator==(const string_editor& _lhs, const string& _rhs);
	MEMEPP__IMPL_INLINE bool operator==(const string& _lhs, const string_editor& _rhs);
	MEMEPP__IMPL_INLINE bool operator==(const string_editor& _lhs, const string_view& _rhs);
	MEMEPP__IMPL_INLINE bool operator==(const string_view& _lhs, const string_editor& _rhs);

	MEMEPP__IMPL_INLINE bool operator!=(const string_editor& _lhs, const string& _rhs);
	MEMEPP__IMPL_INLINE bool operator!=(const string& _lhs, const string_editor& _rhs);
	MEMEPP__IMPL_INLINE bool operator!=(const string_editor& _lhs, const string_view& _rhs);
	MEMEPP__IMPL_INLINE bool operator!=(const string_view& _lhs, const string_editor& _rhs);
};

#endif // !MEMEPP_STRING_EDITOR_DEF_HPP_INCLUDED
