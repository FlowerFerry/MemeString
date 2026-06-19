
#ifndef MEMEPP_DLL_HPP_INCLUDED
#define MEMEPP_DLL_HPP_INCLUDED

#include <meme/string_fwd.h>

#include <type_traits>
#include <megopp/help/trivial_create.h>

namespace memepp {

	/*
	 * import_from_dll / export_into_dll are customization-point stubs.
	 * 
	 * Design pattern:
	 *   Data flowing from the plugin into the host process MUST be
	 *   deep-copied (the host cannot trust plugin-allocated memory).
	 *   Data flowing from the host into a plugin MAY be passed by
	 *   reference/shallow-copy — the plugin runs in-process and the
	 *   host-controlled lifetime outlasts the plugin call.
	 * 
	 * Concrete implementations override these templates per-type to
	 * apply the appropriate copy semantics.
	 */
	template<typename _Result, typename _Ty>
	inline _Result import_from_dll(const _Ty& _obj, mmint_t _struct_size)
	{
		return mgpp::help::trivial_create<_Result>();
	}
	
	template<typename _Result, typename _Ty,
		typename = typename std::enable_if<std::is_rvalue_reference<_Ty&&>::value>::type>
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
		typename = typename std::enable_if<std::is_rvalue_reference<_Ty&&>::value>::type>
	inline _Result export_into_dll(_Ty&& _obj, mmint_t _struct_size)
	{
        return mgpp::help::trivial_create<_Result>();
	}

}

#endif // !MEMEPP_DLL_HPP_INCLUDED
