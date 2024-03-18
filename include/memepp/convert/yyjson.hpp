
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
            if (MEGO_SYMBOL__UNLIKELY(!yyjson_is_str(_v)))
            {
                return {};
            }
            return { yyjson_get_str(_v), static_cast<mmint_t>(yyjson_get_len(_v)) };
		}
		static memepp::string from_object(yyjson_val* && _v)
		{
            if (MEGO_SYMBOL__UNLIKELY(!yyjson_is_str(value)))
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
            if (MEGO_SYMBOL__UNLIKELY(!yyjson_is_str(_v)))
            {
                return {};
            }
            return { yyjson_get_str(_v), static_cast<mmint_t>(yyjson_get_len(_v)) };
		}
	};
    


}

//inline memepp::string from(yyjson_val *value) 
//{
//    if (MEGO_SYMBOL__UNLIKELY(!yyjson_is_str(value))) 
//    {
//        return {};
//    }
//    return { yyjson_get_str(value), static_cast<mmint_t>(yyjson_get_len(value)) };
//}
//
//inline memepp::string_view view(yyjson_val *value) 
//{
//    if (MEGO_SYMBOL__UNLIKELY(!yyjson_is_str(value))) 
//    {
//        return {};
//    }
//    return { yyjson_get_str(value), static_cast<mmint_t>(yyjson_get_len(value)) };
//}

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

};


#endif // !MEMEPP_CONVERT_YYJSON_HPP_INCLUDED
