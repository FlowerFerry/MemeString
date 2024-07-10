
#ifndef MEGOPP_ERR_INTO_YYJSON_H_INCLUDED
#define MEGOPP_ERR_INTO_YYJSON_H_INCLUDED

#include <yyjson.h>

#include "err.h"
#include <memory>

namespace mgpp {
namespace errs {

inline std::unique_ptr<yyjson_mut_doc> into_yyjson_doc(const mgpp::err& _err) 
{
    auto yydoc = yyjson_mut_doc_new(nullptr);
    if (!yydoc) {
        return nullptr;
    }
    std::unique_ptr<yyjson_mut_doc> yydoc_ptr(
        yydoc, [](yyjson_mut_doc* _doc) { yyjson_mut_doc_free(_doc); });
    
    auto yyroot = yyjson_mut_obj(yydoc);
    yyjson_mut_doc_set_root(yydoc, yyroot);

    yyjson_mut_obj_add_sint(yyroot, "code", _err.code());
    yyjson_mut_obj_add_str (yyroot, "message", _err.message().data());

    auto yycategory = yyjson_mut_obj(yydoc);
    yyjson_mut_obj_add_obj(yyroot, yycategory, "category");

    yyjson_mut_obj_add_str(yycategory, "name", _err.category().name().data());

    return std::move(yydoc_ptr);
}

}
}

#endif // !MEGOPP_ERR_INTO_YYJSON_H_INCLUDED
