
#ifndef MEMEPP_BUFFER_TMPIMPL_HPP_INCLUDED
#define MEMEPP_BUFFER_TMPIMPL_HPP_INCLUDED

#include <meme/buffer.h>

#include <memepp/dll.hpp>
#include <memepp/buffer_def.hpp>

namespace memepp {
    
	template<>
	inline memepp::buffer import_from_dll(const mmbufstk_t& _obj, mmint_t _struct_size)
	{
		mmbufstk_t buf;
		auto result = MemeBufferStack_initAndConditionalConvert(&buf, _struct_size, memepp::to_pointer(_obj));
		if (result)
			return {};

		return memepp::buffer{ std::move(buf) };
	}

	template<>
	inline memepp::buffer import_from_dll(mmbufstk_t&& _obj, mmint_t _struct_size)
	{
		mmbufstk_t buf;
		auto result = MemeBufferStack_initAndConditionalConvert(&buf, _struct_size, memepp::to_pointer(_obj));
		MemeBufferStack_unInit(&_obj, _struct_size);
		if (result)
			return {};

		return memepp::buffer{ std::move(buf) };
	}

	template<>
	inline mmbufstk_t export_into_dll(const memepp::buffer& _obj, mmint_t _struct_size)
	{
		mmbufstk_t buf;
		MemeBufferStack_initByOther(&buf, _struct_size, &_obj.native_handle());
		return buf;
	}

	template<>
	inline mmbufstk_t export_into_dll(memepp::buffer&& _obj, mmint_t _struct_size)
	{
		mmbufstk_t buf;
		MemeBufferStack_init(&buf, _struct_size);
		MemeBuffer_swap(memepp::to_pointer(buf), const_cast<mmbuf_ptr_t>(memepp::to_pointer(_obj.native_handle())));
		return buf;
	}

}

#endif // !MEMEPP_BUFFER_TMPIMPL_HPP_INCLUDED
