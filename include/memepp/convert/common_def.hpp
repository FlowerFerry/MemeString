
#ifndef MEMEPP_CONVERT_COMMON_DEF_HPP_INCLUDED
#define MEMEPP_CONVERT_COMMON_DEF_HPP_INCLUDED

#include "memepp/string_def.hpp"
#include "memepp/string_view_def.hpp"

#include <utility>
//
//template<typename _Ty>
//_Ty mm_to(const memepp::string& _s);
//
//template<typename _Ty>
//_Ty mm_to(const memepp::string_view& _s);
//
//template<typename _Ty>
//_Ty mm_into(const memepp::string& _s);
//
//template<typename _Ty>
//_Ty mm_into(const memepp::string_view& _s);
//
//template<typename _Ty>
//memepp::string_view mm_view(const _Ty& _s);

namespace memepp {
	
namespace convert {

    template<typename _Ty>
	struct from;
	//{
	//	static memepp::string from_object(const _Ty& _v)
	//	{
	//		// User defined
	//	}
	//	static memepp::string from_object(_Ty&& _v)
	//	{
	//		// User defined
	//	}
	//};

	template<typename _Ty>
	struct view;
	//{
	//	static memepp::string_view view_object(const _Ty& _v)
	//	{
	//		// User defined
	//	}
	//};
	
	template<typename _Ty>
	struct into;
	//{
	//	static _Ty into_object(const memepp::string& _v)
	//	{
	//		// User defined
	//	}
	//	static _Ty into_object(const memepp::string_view& _v)
	//	{
	//		// User defined
	//	}
	//};



}

	template<typename _Ty>
	inline _Ty to(const memepp::string& _s)
	{
		return memepp::convert::into<
			typename std::remove_reference<typename std::remove_cv<_Ty>::type>::type
		>::into_object(_s);
	}

	template<typename _Ty>
	inline _Ty to(const memepp::string_view& _sv)
	{
		return memepp::convert::into<
			typename std::remove_reference<typename std::remove_cv<_Ty>::type>::type
		>::into_object(_sv);
	}
	
	template<typename _Ty>
	inline _Ty into(const memepp::string& _s)
	{
		return memepp::convert::into<
			typename std::remove_reference<typename std::remove_cv<_Ty>::type>::type
		>::into_object(_s);
	}

	template<typename _Ty>
	inline _Ty into(const memepp::string_view& _sv)
	{
		return memepp::convert::into<
			typename std::remove_reference<typename std::remove_cv<_Ty>::type>::type
		>::into_object(_sv);
	}

	template<typename _Ty>
	inline memepp::string_view view(const _Ty& _s)
	{
        return memepp::convert::view<
            typename std::remove_reference<typename std::remove_cv<_Ty>::type>::type
        >::view_object(_s);
	}

    template<typename _Ty>
    inline memepp::string from(const _Ty& _s)
    {
        return memepp::convert::from<
            typename std::remove_reference<typename std::remove_cv<_Ty>::type>::type
        >::from_object(_s);
    }
	
    template<typename _Ty,
		typename = typename std::enable_if<std::is_rvalue_reference<_Ty&&>::value>::type>
    inline memepp::string from(_Ty&& _s)
    {
        return memepp::convert::from<
            typename std::remove_reference<typename std::remove_cv<_Ty>::type>::type
        >::from_object(std::forward<_Ty>(_s));
    }
	
};

template<typename _Ty>
inline _Ty mm_to(const memepp::string& _s)
{
	return memepp::to<_Ty>(_s);
}

template<typename _Ty>
inline _Ty mm_to(const memepp::string_view& _s)
{
	return memepp::to<_Ty>(_s);
}

template<typename _Ty>
inline _Ty mm_into(const memepp::string& _s)
{
	return memepp::to<_Ty>(_s);
}

template<typename _Ty>
inline _Ty mm_into(const memepp::string_view& _s)
{
	return memepp::to<_Ty>(_s);
}

template<typename _Ty>
inline memepp::string_view mm_view(const _Ty& _s)
{
    return memepp::view(_s);
}

template<typename _Ty>
inline memepp::string mm_from(const _Ty& _s)
{
    return memepp::from(_s);
}

template<typename _Ty,
    typename = typename std::enable_if<std::is_rvalue_reference<_Ty&&>::value>::type>
inline memepp::string mm_from(_Ty&& _s)
{
    return memepp::from(std::forward<_Ty>(_s));
}

#endif // !MEMEPP_CONVERT_COMMON_DEF_HPP_INCLUDED
