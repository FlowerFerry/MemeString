
#ifndef MEGOPP_ERR_INTO_STRING_H_INCLUDED
#define MEGOPP_ERR_INTO_STRING_H_INCLUDED

#include "into_yyjson.h"
#include <stdlib.h>

#include <megopp/util/scope_cleanup.h>

#include <string>

namespace mgpp {
namespace errs {

inline std::string into_string(const mgpp::err& _err)
{
    auto yydoc_ptr = into_yyjson_doc(_err);
    if (!yydoc_ptr) {
        return "{}";
    }

    size_t len = 0;
    char* str = yyjson_mut_write(yydoc_ptr.get(), 0, &len);
    if (!str) {
        return "{}";
    }
    MEGOPP_UTIL__ON_SCOPE_CLEANUP([str] { free(str); });

    return std::string{ str, len };
}

}
}

#endif // !MEGOPP_ERR_INTO_STRING_H_INCLUDED
