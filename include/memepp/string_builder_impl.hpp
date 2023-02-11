
#ifndef MEMEPP_STRING_EDITOR_IMPL_HPP_INCLUDED
#define MEMEPP_STRING_EDITOR_IMPL_HPP_INCLUDED

#include "string_view_def.hpp"
#include "string_builder_def.hpp"

#include "meme/string_builder.h"

namespace memepp {

	MEMEPP__IMPL_INLINE string_builder::string_builder()
	{
		MemeStringBuilderStack_init(&data_, MMSBUILDER__OBJECT_SIZE);
	}

	MEMEPP__IMPL_INLINE string_builder::string_builder(string_builder& _other)
	{
		MemeStringBuilderStack_init(&data_, MMSBUILDER__OBJECT_SIZE);
		MemeStringBuilderStack_swap(&data_, &(_other.data_), MMSBUILDER__OBJECT_SIZE);
	}
	
	//MEMEPP__IMPL_INLINE string_builder::string_builder(const string& _in)
	//{
	//	MemeStringBuilderStack_initWithString(
	//		&data_, MEME_STRING_BUILDER__OBJECT_SIZE,
	//		memepp::to_pointer(_in.native_handle()), MEME_STRING__OBJECT_SIZE
	//	);
	//}

	//MEMEPP__IMPL_INLINE string_builder::string_builder(const string& _in, string& _out)
	//{
	//	auto pointer = const_cast<MemeStringStack_t*>(&_out.native_handle());
	//	MemeStringBuilderStack_initAssociated(
	//		&data_, MEME_STRING_BUILDER__OBJECT_SIZE,
	//		memepp::to_pointer(_in.native_handle()), 
	//		pointer, MEME_STRING__OBJECT_SIZE
	//	);
	//}

	MEMEPP__IMPL_INLINE string_builder::string_builder(string_builder&& _other)
	{
		MemeStringBuilderStack_init(&data_, MEME_STRING_BUILDER__OBJECT_SIZE);
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

	MEMEPP__IMPL_INLINE string_builder& string_builder::operator=(string_builder& _other)
	{
        if (this == &_other) 
			return *this;
		
		MemeStringBuilderStack_swap(&data_, &(_other.data_), MEME_STRING_BUILDER__OBJECT_SIZE);
		return *this;
	}

	MEMEPP__IMPL_INLINE string_builder& string_builder::operator+(const string& _other)
	{
		MemeStringBuilder_appendArgWithString(
			memepp::to_pointer(data_),
			memepp::to_pointer(_other.native_handle()));
        return *this;
	}

    MEMEPP__IMPL_INLINE string_builder& string_builder::operator+(const string_view& _other)
    {
		MemeStringBuilder_appendArgWithString(
            memepp::to_pointer(data_),
            memepp::to_pointer(_other.native_handle()));
        return *this;
    }

    MEMEPP__IMPL_INLINE string_builder& string_builder::operator+(const char* _other)
    {
		memepp::string_view sv{ _other };
        MemeStringBuilder_appendArgWithString(
            memepp::to_pointer(data_),
            memepp::to_pointer(sv.native_handle()));
        return *this;
    }
    
	//MEMEPP__IMPL_INLINE string_builder string_builder::operator+(const string_builder& _other) const
	//{
 //       string_builder result;
 //       MemeStringBuilderStack_assignByOther(
 //           &result.data_, MMSBUILDER__OBJECT_SIZE, memepp::to_pointer(data_));

 //       MemeStringBuilder_appendArgByOther(
 //           memepp::to_pointer(result.data_),
 //           memepp::to_pointer(_other.data_));
 //       return result;
	//}

	MEMEPP__IMPL_INLINE string_builder& string_builder::operator+=(const string& _other)
	{
		MemeStringBuilder_appendArgWithString(
			memepp::to_pointer(data_),
			memepp::to_pointer(_other.native_handle()));
		return *this;
	}

    MEMEPP__IMPL_INLINE string_builder& string_builder::operator+=(const string_view& _other)
    {
        MemeStringBuilder_appendArgWithString(
            memepp::to_pointer(data_),
            memepp::to_pointer(_other.native_handle()));
        return *this;
    }

    MEMEPP__IMPL_INLINE string_builder& string_builder::operator+=(const char* _other)
    {
        memepp::string_view sv{ _other };
        MemeStringBuilder_appendArgWithString(
            memepp::to_pointer(data_),
            memepp::to_pointer(sv.native_handle()));
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

	MEMEPP__IMPL_INLINE string string_builder::generate() const
	{
        string s;
		MemeStringBuilder_generate(
            memepp::to_pointer(data_), 
            memepp::to_pointer(const_cast<MemeStringStack_t&>(s.native_handle())));
        return s;
	}

	MEMEPP__IMPL_INLINE const string_builder::native_handle_type& string_builder::native_handle() const noexcept
	{
		return data_;
	}

	//MEMEPP__IMPL_INLINE bool operator==(const string_builder& _lhs, const string& _rhs)
	//{
	//	int result = 0;
	//	MemeStringBuilderStack_isEqualWithString(
	//		&(_lhs.native_handle()),
	//		MEME_STRING_BUILDER__OBJECT_SIZE,
	//		to_pointer(_rhs.native_handle()), &result);
	//	return result;
	//}

	//MEMEPP__IMPL_INLINE bool operator==(const string& _lhs, const string_builder& _rhs)
	//{
	//	int result = 0;
	//	MemeStringBuilderStack_isEqualWithString(
	//		&(_rhs.native_handle()),
	//		MEME_STRING_BUILDER__OBJECT_SIZE,
	//		to_pointer(_lhs.native_handle()), &result);
	//	return result;
	//}

	//MEMEPP__IMPL_INLINE bool operator==(const string_builder& _lhs, const string_view& _rhs)
	//{
	//	int result = 0;
	//	MemeStringBuilderStack_isEqualWithString(
	//		&(_lhs.native_handle()),
	//		MEME_STRING_BUILDER__OBJECT_SIZE,
	//		to_pointer(_rhs.native_handle()), &result);
	//	return result;
	//}
	//
	//MEMEPP__IMPL_INLINE bool operator==(const string_view& _lhs, const string_builder& _rhs)
	//{
	//	int result = 0;
	//	MemeStringBuilderStack_isEqualWithString(
	//		&(_rhs.native_handle()),
	//		MEME_STRING_BUILDER__OBJECT_SIZE,
	//		to_pointer(_lhs.native_handle()), &result);
	//	return result;
	//}

	//MEMEPP__IMPL_INLINE bool operator!=(const string_builder& _lhs, const string& _rhs)
	//{
	//	!(_lhs == _rhs);
	//}

	//MEMEPP__IMPL_INLINE bool operator!=(const string& _lhs, const string_builder& _rhs)
	//{
	//	!(_lhs == _rhs);
	//}
	//
	//MEMEPP__IMPL_INLINE bool operator!=(const string_builder& _lhs, const string_view& _rhs)
	//{
	//	!(_lhs == _rhs);
	//}

	//MEMEPP__IMPL_INLINE bool operator!=(const string_view& _lhs, const string_builder& _rhs)
	//{
	//	!(_lhs == _rhs);
	//}
    
	MEMEPP__IMPL_INLINE string_builder& operator+(const string& _lhs, string_builder& _rhs)
	{
        MemeStringBuilder_prependArgWithString(
            memepp::to_pointer(const_cast<MemeStringBuilderStack_t&>(_rhs.native_handle())),
            memepp::to_pointer(_lhs.native_handle()));
        return _rhs;
	}
	
	MEMEPP__IMPL_INLINE string_builder operator+(const string& _lhs, string_builder&& _rhs)
	{
        MemeStringBuilder_prependArgWithString(
            memepp::to_pointer(const_cast<MemeStringBuilderStack_t&>(_rhs.native_handle())),
            memepp::to_pointer(_lhs.native_handle()));
        return std::move(_rhs);
	}

    MEMEPP__IMPL_INLINE string_builder& operator+(const string_view& _lhs, string_builder& _rhs)
    {
        MemeStringBuilder_prependArgWithString(
            memepp::to_pointer(const_cast<MemeStringBuilderStack_t&>(_rhs.native_handle())),
            memepp::to_pointer(_lhs.native_handle()));
        return _rhs;
    }
    
    MEMEPP__IMPL_INLINE string_builder operator+(const string_view& _lhs, string_builder&& _rhs)
    {
        MemeStringBuilder_prependArgWithString(
            memepp::to_pointer(const_cast<MemeStringBuilderStack_t&>(_rhs.native_handle())),
            memepp::to_pointer(_lhs.native_handle()));
        return std::move(_rhs);
    }

	MEMEPP__IMPL_INLINE string_builder& operator+(const char* _lhs, string_builder& _rhs)
	{
		memepp::string_view sv{ _lhs };
        MemeStringBuilder_prependArgWithString(
            memepp::to_pointer(const_cast<MemeStringBuilderStack_t&>(_rhs.native_handle())),
            memepp::to_pointer(sv.native_handle()));
        return _rhs;
	}
        
    MEMEPP__IMPL_INLINE string_builder operator+(const char* _lhs, string_builder&& _rhs)
    {
        memepp::string_view sv{ _lhs };
        MemeStringBuilder_prependArgWithString(
            memepp::to_pointer(const_cast<MemeStringBuilderStack_t&>(_rhs.native_handle())),
            memepp::to_pointer(sv.native_handle()));
        return std::move(_rhs);
    }

};

#endif // !MEMEPP_STRING_EDITOR_IMPL_HPP_INCLUDED
