
#ifndef MEGO_SYMBOL_RESTRICT_H_INCLUDED
#define MEGO_SYMBOL_RESTRICT_H_INCLUDED

//! @def MEGO_SYMBOL__RESTRICT
//! @brief Macro to use in place of 'restrict' keyword variants.
#if !defined(MEGO_SYMBOL__RESTRICT)
#  if defined(_MSC_VER)
#    define MEGO_SYMBOL__RESTRICT __restrict
#    if !defined(MEGO_SYM__RESTRICT__AVAILABLE) && (_MSC_FULL_VER < 190023026)
#      define MEGO_SYM__RESTRICT__AVAILABLE (1)
#    endif
#  elif defined(__GNUC__) && __GNUC__ > 3
// Clang also defines __GNUC__ (as 4)
#    define MEGO_SYMBOL__RESTRICT __restrict__
#  else
#    define MEGO_SYMBOL__RESTRICT
#    if !defined(MEGO_SYM__RESTRICT__AVAILABLE)
#      define MEGO_SYM__RESTRICT__AVAILABLE (1)
#    endif
#  endif
#endif

#ifndef MEGO_SYM__RESTRICT__AVAILABLE
#  define MEGO_SYM__RESTRICT__AVAILABLE (0)
#endif

#ifndef MG_SYM__RESTRICT_AVAIL
#define MG_SYM__RESTRICT_AVAIL MEGO_SYM__RESTRICT__AVAILABLE
#endif

#endif // MEGO_SYMBOL_RESTRICT_H_INCLUDED
