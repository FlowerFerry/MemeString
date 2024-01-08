
#ifndef MG_ERR_AVEC_CONVERT_H_INCLUDED
#define MG_ERR_AVEC_CONVERT_H_INCLUDED

#include <mego/err/ec.h>
#include <mego/predef/symbol/inline.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <libavutil/error.h>

MG_CAPI_INLINE mgec_t mgec__from_averr(int _err)
{
	if (_err == 0)
		return 0;
	
    switch (_err) {
    default:                return mgec__from_posix_err(AVUNERROR(_err));
    }
}

#ifdef __cplusplus
}
#endif

#endif // !MG_ERR_AVEC_CONVERT_H_INCLUDED
