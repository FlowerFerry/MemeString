
#ifndef MEMEPP_DLL_HPP_INCLUDED
#define MEMEPP_DLL_HPP_INCLUDED

#include <type_traits>
#include <megopp/help/trivial_create.h>

namespace memepp {

	template<typename _Ty, typename _Result>
	inline _Result import_from_dll(_Ty _obj, mmint_t _struct_size)
	{
		return mgpp::help::trivial_create<_Result>();
	}

	template<typename _Ty, typename _Result>
	inline _Result export_into_dll(_Ty _obj, mmint_t _struct_size)
	{
        return mgpp::help::trivial_create<_Result>();
	}

}

#endif // !MEMEPP_DLL_HPP_INCLUDED
