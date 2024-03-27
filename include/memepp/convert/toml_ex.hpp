
#ifndef MEMEPP_CONVERT_TOML_EX_HPP_INCLUDED
#define MEMEPP_CONVERT_TOML_EX_HPP_INCLUDED

#include <memepp/string_def.hpp>
#include <memepp/string_view_def.hpp>
#include <toml/value.hpp>
#include "./std/string.hpp"

namespace memepp  {
namespace convert {

    

}

    inline string from_toml11_table(
        const toml::table& _table, 
        const ::std::string& _key, const string_view& _default)
    {
        auto it = _table.find(_key);
        if (it != _table.end() && it->second.is_string())
            return mm_from(it->second.as_string());
        return _default.to_string();
    }

}

#endif // !MEMEPP_CONVERT_TOML_EX_HPP_INCLUDED
