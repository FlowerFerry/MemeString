
#ifndef MEGO_STOOL_OS_WINDOWS_SIMPLE_H_INCLUDED
#define MEGO_STOOL_OS_WINDOWS_SIMPLE_H_INCLUDED

#include "mego/predef/os/windows.h"

#if MEGO_OS__WINDOWS__AVAILABLE
#	ifndef WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN
#		define __UNDEF_LEAN_AND_MEAN
#	endif
#	include <windows.h>
#	ifdef __UNDEF_LEAN_AND_MEAN
#		undef WIN32_LEAN_AND_MEAN
#		undef __UNDEF_LEAN_AND_MEAN
#	endif

// Very hate the pollution of this macro !!
#	undef max
#	undef min

#endif

#endif // !MEGO_STOOL_OS_WINDOWS_SIMPLE_H_INCLUDED
