
#ifndef MEMEPP_VARTS_TMPIMPL_HPP_INCLUDED
#define MEMEPP_VARTS_TMPIMPL_HPP_INCLUDED

#include <memepp/dll.hpp>
#include <memepp/varts_def.hpp>

namespace memepp {

	template<>
	inline memepp::varts import_from_dll(const mmvtsstk_t& _obj, mmint_t _struct_size)
	{
        return memepp::varts{ _obj };
	}

	template<>
	inline memepp::varts import_from_dll(mmvtsstk_t&& _obj, mmint_t _struct_size)
	{
        return memepp::varts{ std::move(_obj) };
	}

	template<>
	inline mmvtsstk_t export_into_dll(const memepp::varts& _obj, mmint_t _struct_size)
	{
		mmvtsstk_t vts;
		vts.ts = (uint64_t)_obj.timestamp();
        vts.userdata = (uint8_t)_obj.userdata();
        vts.var = memepp::export_into_dll<mmvarstk_t>(_obj.var(), MMVAR__OBJ_SIZE);
		return vts;
	}

	template<>
	inline mmvtsstk_t export_into_dll(memepp::varts&& _obj, mmint_t _struct_size)
	{
		mmvtsstk_t vts;
        vts.ts = (uint64_t)_obj.timestamp();
        vts.userdata = (uint8_t)_obj.userdata();
        vts.var = memepp::export_into_dll<mmvarstk_t>(std::move(_obj.var()), MMVAR__OBJ_SIZE);
		return vts;
	}
    
}

#endif // !MEMEPP_VARTS_TMPIMPL_HPP_INCLUDED
