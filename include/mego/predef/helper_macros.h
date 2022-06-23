
#ifndef MEGO_PREDEF_HELPER_MACROS_H_INCLUDED
#define MEGO_PREDEF_HELPER_MACROS_H_INCLUDED

//! @def MEGO__STRINGIZE
//! 
//! Converts the parameter X to a string after macro replacement
//! on X has been performed.
#ifndef MEGO__STRINGIZE
#   define MEGO__STRINGIZE(X) MEGO__DO_STRINGIZE(X)
#   define MEGO__DO_STRINGIZE(X) #X
#endif 

//! @def MEGO__JOIN
//!
//! The following piece of macro magic joins the two
//! arguments together, even when one of the arguments is
//! itself a macro (see 16.3.1 in C++ standard).  The key
//! is that macro expansion of macro arguments does not
//! occur in __MEGO__DO_JOIN but does in NCM_DO_JOIN.
#ifndef MEGO__JOIN
#   define MEGO__JOIN( X, Y ) NCM_DO_JOIN( X, Y )
#   define MEGO__DO_JOIN( X, Y ) __NCM_DO_JOIN(X,Y)
#   define __MEGO__DO_JOIN( X, Y ) X##Y
#endif 

#ifndef MEGO__MAKE_VERSION_NUMBER
#   define MEGO__MAKE_VERSION_NUMBER(major, minor, patch) \
    ( (((major)%100)*10000000) + (((minor)%100)*100000) + ((patch)%100000) )

#   define MEGO__GET_VERSION_MAJOR(value) (((value)/10000000)%100)
#   define MEGO__GET_VERSION_MINOR(value) (((value)/100000)%100)
#   define MEGO__GET_VERSION_PATCH(value)  ((value)%100000)
#endif

#endif // !MEGO_PREDEF_HELPER_MACROS_H_INCLUDED