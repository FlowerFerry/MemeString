
#ifndef MEGOPP_PREDEF_ATTRIBUTE_INDETERMINATE_H_INCLUDED
#define MEGOPP_PREDEF_ATTRIBUTE_INDETERMINATE_H_INCLUDED

#include <mego/predef/lang/version.h>

#if MG_LANG__CXX26_AVAIL
#endif

#ifndef MGPP_ATTR__INDETERMINATE
#if MG_LANG__CXX26_AVAIL

//! \def MGPP_ATTR__INDETERMINATE
//! \ref https://en.cppreference.com/w/cpp/language/attributes/indeterminate
#define MGPP_ATTR__INDETERMINATE [[indeterminate]]
#else

//! \def MGPP_ATTR__INDETERMINATE
//! \ref https://en.cppreference.com/w/cpp/language/attributes/indeterminate
#define MGPP_ATTR__INDETERMINATE 
#endif
#endif

#endif // !MEGOPP_PREDEF_ATTRIBUTE_INDETERMINATE_H_INCLUDED
