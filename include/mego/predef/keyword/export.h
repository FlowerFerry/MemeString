
#ifndef MEGO_PREDEF_SYMBOL_EXPORT_H_INCLUDED
#define MEGO_PREDEF_SYMBOL_EXPORT_H_INCLUDED

#include "mego/predef/os/windows.h"

#if MEGO_OS__WINDOWS__AVAILABLE
#  define MEGO_SYMBOL__EXPORT __declspec(dllexport)
#endif

#endif // !MEGO_PREDEF_SYMBOL_EXPORT_H_INCLUDED