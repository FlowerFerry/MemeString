
#ifndef MEGO_PREDEF_HAS_CXX11_H_INCLUDED
#define MEGO_PREDEF_HAS_CXX11_H_INCLUDED

#if defined(__GNUC__) && (defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus >= 201103L))
#   define MEGO__GCC_CXX11__AVAILABLE (1)
#endif

#if (defined(__GLIBCPP__) || defined(__GLIBCXX__)) \
	&& (defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus >= 201103))
#   define MEGO__LIBSTD_CXX11__AVAILABLE (1)
#endif

#ifndef MEGO__GCC_CXX11__AVAILABLE
#define MEGO__GCC_CXX11__AVAILABLE (0)
#endif

#ifndef MEGO__LIBSTD_CXX11__AVAILABLE
#define MEGO__LIBSTD_CXX11__AVAILABLE (0)
#endif 

#endif // !MEGO_PREDEF_HAS_CXX11_H_INCLUDED
