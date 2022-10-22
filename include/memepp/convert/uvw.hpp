
#ifndef MEMEPP_CONVERT_UVW_HPP_INCLUDED
#define MEMEPP_CONVERT_UVW_HPP_INCLUDED

#include "memepp/string_def.hpp"
#include "memepp/buffer_def.hpp"
#include "memepp/variable_buffer_def.hpp"

#include "uvw/stream.h"
#include <memory>

namespace memepp {
namespace uvw {

	template<typename _Ty>
	class buf_deleter
	{
	public:
		buf_deleter() :
			obj(std::make_unique<_Ty>())
		{}

		inline void operator() (char*) {}

		std::unique_ptr< _Ty > obj;
	};

	template<>
	class buf_deleter<memepp::buffer>
	{
	public:
		buf_deleter()
		{}

		inline void operator() (char*) {}

		memepp::buffer obj;
	};

};

	template<typename T, typename U>
	inline void write(const memepp::buffer& _buf, 
		::uvw::StreamHandle<T, U>& _stream)
	{
		uvw::buf_deleter< memepp::buffer > d;
		d.obj = _buf;

		auto ptr = d.obj.data();
		auto len = d.obj.size();
		_stream.write(
			std::unique_ptr<char[],
				uvw::buf_deleter< memepp::buffer > >(
					(char*)(ptr), std::move(d)), static_cast<uint32_t>(len));
	}

	template<typename T, typename U>
	inline void write(const memepp::string& _buf,
		::uvw::StreamHandle<T, U>& _stream)
	{
		uvw::buf_deleter< memepp::string > d;
		*(d.obj) = _buf;

		auto ptr = d.obj->data();
		auto len = d.obj->size();
		_stream.write(
			std::unique_ptr<char[],
			uvw::buf_deleter< memepp::string > >(
				(char*)(ptr), std::move(d)), static_cast<uint32_t>(len));
	}


	template<typename T, typename U>
	inline void write(memepp::variable_buffer&& _buf,
		::uvw::StreamHandle<T, U>& _stream)
	{
		uvw::buf_deleter< memepp::variable_buffer > d;
		d.obj->swap(_buf);

		auto ptr = d.obj->data();
		auto len = d.obj->size();
		_stream.write(
			std::unique_ptr<char[],
			uvw::buf_deleter< memepp::variable_buffer > >(
				(char*)(ptr), std::move(d)), static_cast<uint32_t>(len));
	}

};

#endif // !MEMEPP_CONVERT_UVW_HPP_INCLUDED
