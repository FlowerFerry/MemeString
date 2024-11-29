
#ifndef MEGOPP_ERR_CONVERT_STD_ERROR_CODE_H_INCLUDED
#define MEGOPP_ERR_CONVERT_STD_ERROR_CODE_H_INCLUDED

#include <mego/err/ec_impl.h>

#include <system_error>
#include <megopp/err/err.h>

namespace mgpp {

    inline err into_err(const std::error_code& _ec)
    {
        if (_ec.category() == std::generic_category())
            return { mgec__from_posix_err(_ec.value()) };

        if (_ec.category() == std::system_category())
            return { mgec__from_sys_err(_ec.value()) };
        
        return { MGEC__ERR };
    }

}

#endif // !MEGOPP_ERR_CONVERT_STD_ERROR_CODE_H_INCLUDED
