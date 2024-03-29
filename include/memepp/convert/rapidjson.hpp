
#ifndef MEMEPP_CONVERT_RAPIDJSON_HPP_INCLUDED
#define MEMEPP_CONVERT_RAPIDJSON_HPP_INCLUDED

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/pointer.h"

#include "memepp/convert/common_def.hpp"
#include "memepp/string.hpp"

namespace memepp {

	inline memepp::string from(const rapidjson::GenericStringRef<char>& _s)
	{
		return memepp::string { _s.s, static_cast<MemeInteger_t>(_s.length) };
	}

	inline memepp::string from(const rapidjson::StringBuffer& _s)
	{
		return memepp::string{ _s.GetString(), static_cast<MemeInteger_t>(_s.GetLength())};
	}

	inline memepp::string from(const rapidjson::Value& _v)
	{
		if (_v.IsString())
			return memepp::string{ _v.GetString(), static_cast<MemeInteger_t>(_v.GetStringLength()) };
		else
			return {};
	}

    inline string from(const rapidjson::Value& _value, const memepp::string_view& _path, const memepp::string_view& _default)
    {
        rapidjson::Pointer p(_path.data(), _path.length());
		auto value = p.Get(_value);
        if (value && value->IsString())
            return memepp::string{ value->GetString(), static_cast<MemeInteger_t>(value->GetStringLength()) };
        else
            return _default.to_string();
    }

	inline memepp::string from(rapidjson::StringBuffer&& _buf)
	{
		static const auto destruct_func = [](void* _object) { 
			delete reinterpret_cast<rapidjson::StringBuffer*>(_object); };
		static const auto data_func = [](const void* _object) { 
			return reinterpret_cast<const rapidjson::StringBuffer*>(_object)->GetString(); };
		static const auto size_func = [](const void* _object) { 
			return reinterpret_cast<const rapidjson::StringBuffer*>(_object)->GetLength(); };

		if (_buf.GetLength() == 0)
			return{};
        else if (_buf.GetLength() < MEME_STRING__OBJECT_SIZE)
		{
			return memepp::string(_buf.GetString(), static_cast<MemeInteger_t>(_buf.GetLength()));
		}
		else if (_buf.GetLength() < 16 * sizeof(intptr_t))
		{
			return memepp::string(_buf.GetString(), static_cast<MemeInteger_t>(_buf.GetLength()),
				memepp::string_storage_t::medium);
		}

        memepp::string out;
        auto obj = new rapidjson::StringBuffer(std::move(_buf));
        auto ret = MemeStringStack_initTakeOverUserObject(
            const_cast<MemeStringStack_t*>(&(out.native_handle())), MEME_STRING__OBJECT_SIZE,
            obj, destruct_func, data_func, size_func);
        if (ret) {
            destruct_func(obj);
            return {};
        }
        return out;
	}

	inline memepp::string_view view(const rapidjson::GenericStringRef<char>& _s)
	{
		return memepp::string_view { _s.s, static_cast<MemeInteger_t>(_s.length) };
	}

	inline memepp::string_view view(const rapidjson::StringBuffer& _s)
	{
		return memepp::string_view { _s.GetString(), static_cast<MemeInteger_t>(_s.GetLength()) };
	}

	inline memepp::string_view view(const rapidjson::Value& _v)
	{
		if (_v.IsString())
			return memepp::string_view { _v.GetString(), static_cast<MemeInteger_t>(_v.GetStringLength()) };
		else
			return {};
	}

	template<>
	inline rapidjson::GenericStringRef<char> to< rapidjson::GenericStringRef<char> >(const memepp::string& _s)
	{
		return rapidjson::GenericStringRef<char>{ _s.data(), static_cast<rapidjson::SizeType>(_s.size()) };
	}

	template<>
	inline rapidjson::GenericStringRef<char> to< rapidjson::GenericStringRef<char> >(const memepp::string_view& _sv)
	{
		return rapidjson::GenericStringRef<char>{ _sv.data(), static_cast<rapidjson::SizeType>(_sv.size()) };
	}


};

inline memepp::string mm_from(const rapidjson::GenericStringRef<char>& _s)
{
    return memepp::from(_s);
}

inline memepp::string mm_from(const rapidjson::StringBuffer& _s)
{
    return memepp::from(_s);
}

inline memepp::string mm_from(const rapidjson::Value& _v)
{
    return memepp::from(_v);
}

inline memepp::string mm_from(rapidjson::StringBuffer&& _buf)
{
    return memepp::from(std::move(_buf));
}

namespace rapidjson
{
	inline rapidjson::GenericStringRef<char> toStringRef(const memepp::string_view& _s)
	{
		return rapidjson::GenericStringRef<char>{ _s.data(), static_cast<rapidjson::SizeType>(_s.size()) };
	}

};

#endif // !MEMEPP_CONVERT_RAPIDJSON_HPP_INCLUDED
