
#ifndef MEGOPP_ERR_INTO_YYJSON_H_INCLUDED
#define MEGOPP_ERR_INTO_YYJSON_H_INCLUDED

#include <yyjson.h>

#include "err.h"
#include <memory>

namespace mgpp {
namespace errs {

inline std::unique_ptr<yyjson_mut_doc, void(*)(yyjson_mut_doc*)> into_yyjson_doc(const mgpp::err& _err)
{
    auto yydoc = yyjson_mut_doc_new(nullptr);
    if (!yydoc) {
        return { nullptr, yyjson_mut_doc_free };
    }
    
    std::unique_ptr<yyjson_mut_doc, void(*)(yyjson_mut_doc*)> 
        yydoc_ptr(yydoc, yyjson_mut_doc_free);
    
    auto yyroot = yyjson_mut_obj(yydoc);
    yyjson_mut_doc_set_root(yydoc, yyroot);

    yyjson_mut_obj_add_sint(yydoc, yyroot, "code", _err.code());
    yyjson_mut_obj_add_str (yydoc, yyroot, "message", _err.message().data());

    auto yycategory = yyjson_mut_obj_add_obj(yydoc, yyroot, "category");

    yyjson_mut_obj_add_str(yydoc, yycategory, "name", _err.category()->name());

    return std::move(yydoc_ptr);
}

}
}

#endif // !MEGOPP_ERR_INTO_YYJSON_H_INCLUDED
