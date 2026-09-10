
#ifndef MEMEPP_CONVERT_YYJSON_HPP_INCLUDED
#define MEMEPP_CONVERT_YYJSON_HPP_INCLUDED

#include <yyjson.h>
#include <mego/predef/symbol/likely.h>

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>

#include <memepp/convert/common_def.hpp>

namespace memepp  {
namespace convert {

	template<>
	struct from<yyjson_val*>
	{
		static memepp::string from_object(const yyjson_val* & _v)
		{
            if (MEGO_SYMBOL__UNLIKELY(!yyjson_is_str(const_cast<yyjson_val*>(_v))))
            {
                return {};
            }
            return { yyjson_get_str(const_cast<yyjson_val*>(_v)), static_cast<mmint_t>(yyjson_get_len(const_cast<yyjson_val*>(_v))) };
		}
		static memepp::string from_object(yyjson_val* && _v)
		{
            if (MEGO_SYMBOL__UNLIKELY(!yyjson_is_str(_v)))
            {
                return {};
            }
            return { yyjson_get_str(_v), static_cast<mmint_t>(yyjson_get_len(_v)) };
		}
	};

	template<>
	struct view<yyjson_val*>
	{
		static memepp::string_view view_object(const yyjson_val* & _v)
		{
            if (MEGO_SYMBOL__UNLIKELY(!yyjson_is_str(const_cast<yyjson_val*>(_v))))
            {
                return {};
            }
            return { yyjson_get_str(const_cast<yyjson_val*>(_v)), static_cast<mmint_t>(yyjson_get_len(const_cast<yyjson_val*>(_v))) };
		}
	};
    


}

inline memepp::string from_yyjson_value(yyjson_val* value, const memepp::string_view& key, const memepp::string_view& default_value)
{
    yyjson_val* val = yyjson_obj_getn(value, key.data(), static_cast<size_t>(key.size()));
    if (MEGO_SYMBOL__UNLIKELY(!val)) 
    {
        return default_value.to_string();
    }
    if (MEGO_SYMBOL__UNLIKELY(!yyjson_is_str(val))) 
    {
        return default_value.to_string();
    }
    return { yyjson_get_str(val), static_cast<mmint_t>(yyjson_get_len(val)) };
} 

inline yyjson_mut_val* into_yyjson_value(yyjson_mut_doc* _doc, const memepp::string_view& value, bool _copy)
{
    if (_copy) {
        return yyjson_mut_strncpy(_doc, value.data(), static_cast<size_t>(value.size()));
    }
    else {
        return yyjson_mut_strn   (_doc, value.data(), static_cast<size_t>(value.size()));
    }
}

inline bool add_yyjson_value(yyjson_mut_doc* _doc, yyjson_mut_val* _arr,
	const memepp::string_view& _value, bool _copy)
{
	if (!_arr) return false;
	auto len = static_cast<size_t>(_value.size());
	return _copy
		? yyjson_mut_arr_add_strncpy(_doc, _arr, _value.data(), len)
		: yyjson_mut_arr_add_strn   (_doc, _arr, _value.data(), len);
}

inline bool add_yyjson_member(yyjson_mut_doc* _doc, yyjson_mut_val* _obj, const char* _key,
	const memepp::string_view& _value, bool _copy)
{
	if (!_obj) return false;
	auto len = static_cast<size_t>(_value.size());
	return _copy
		? yyjson_mut_obj_add_strncpy(_doc, _obj, _key, _value.data(), len)
		: yyjson_mut_obj_add_strn   (_doc, _obj, _key, _value.data(), len);
}

};

#endif // !MEMEPP_CONVERT_YYJSON_HPP_INCLUDED
