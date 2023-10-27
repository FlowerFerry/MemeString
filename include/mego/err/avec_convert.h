
#ifndef MG_ERR_AVEC_CONVERT_H_INCLUDED
#define MG_ERR_AVEC_CONVERT_H_INCLUDED

#include <mego/err/ec.h>

#ifdef __cplusplus
extern "C" {
#endif
#include <libavutil/error.h>
#ifdef __cplusplus
}
#endif

inline mgec_t mgec__from_averr(int _err)
{
	if (_err == 0)
		return 0;
	
    switch (_err) {
    default:                return mgec__from_posix_err(AVUNERROR(_err));
    }
}

#endif // !MG_ERR_AVEC_CONVERT_H_INCLUDED
