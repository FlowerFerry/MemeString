
#ifndef MEMEPP_DLL_HPP_INCLUDED
#define MEMEPP_DLL_HPP_INCLUDED

#include <meme/string_fwd.h>

#include <type_traits>
#include <megopp/help/trivial_create.h>

namespace memepp {

	template<typename _Result, typename _Ty>
	inline _Result import_from_dll(const _Ty& _obj, mmint_t _struct_size)
	{
		return mgpp::help::trivial_create<_Result>();
	}
	
	template<typename _Result, typename _Ty,
		typename = typename std::enable_if<!std::is_lvalue_reference<_Ty&&>::value>::type>
	inline _Result import_from_dll(_Ty&& _obj, mmint_t _struct_size)
	{
		return mgpp::help::trivial_create<_Result>();
	}

	template<typename _Result, typename _Ty>
	inline _Result export_into_dll(const _Ty& _obj, mmint_t _struct_size)
	{
        return mgpp::help::trivial_create<_Result>();
	}
	
	template<typename _Result, typename _Ty,
		typename = typename std::enable_if<!std::is_lvalue_reference<_Ty&&>::value>::type>
	inline _Result export_into_dll(_Ty&& _obj, mmint_t _struct_size)
	{
        return mgpp::help::trivial_create<_Result>();
	}

}

#endif // !MEMEPP_DLL_HPP_INCLUDED
