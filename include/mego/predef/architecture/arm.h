
#if !defined(MEGO_ARCH__STRING)

#if \
    defined(__ARM_ARCH) || defined(__TARGET_ARCH_ARM) || \
    defined(__TARGET_ARCH_THUMB) || defined(_M_ARM) || \
    defined(__arm__) || defined(__arm64) || defined(__thumb__) || \
    defined(_M_ARM64) || defined(__aarch64__) || defined(__AARCH64EL__) || \
    defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || \
    defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || \
    defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || \
    defined(__ARM_ARCH_6KZ__) || defined(__ARM_ARCH_6T2__) || \
    defined(__ARM_ARCH_5TE__) || defined(__ARM_ARCH_5TEJ__) || \
    defined(__ARM_ARCH_4T__) || defined(__ARM_ARCH_4__)

#   if !defined(MEGO_ARCH__ARM) && defined(__ARM_ARCH)
#       define MEGO_ARCH__ARM MEGO__MAKE_VERSION_NUMBER(__ARM_ARCH, 0, 0)
#   endif
#   if !defined(MEGO_ARCH__ARM) && defined(__TARGET_ARCH_ARM)
#       define MEGO_ARCH__ARM MEGO__MAKE_VERSION_NUMBER(__TARGET_ARCH_ARM, 0, 0)
#   endif
#   if !defined(MEGO_ARCH__ARM) && defined(__TARGET_ARCH_THUMB)
#       define MEGO_ARCH__ARM MEGO__MAKE_VERSION_NUMBER(__TARGET_ARCH_THUMB, 0, 0)
#   endif
#   if !defined(MEGO_ARCH__ARM) && defined(_M_ARM)
#       define MEGO_ARCH__ARM MEGO__MAKE_VERSION_NUMBER(_M_ARM, 0, 0)
#   endif
#   if !defined(MEGO_ARCH__ARM) && ( \
        defined(__arm64) || defined(_M_ARM64) || defined(__aarch64__) || \
        defined(__AARCH64EL__) )
#       define MEGO_ARCH__ARM MEGO__MAKE_VERSION_NUMBER(8, 0, 0)
#   endif
#   if !defined(MEGO_ARCH__ARM) && ( \
    defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || \
    defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) )
#       define MEGO_ARCH__ARM MEGO__MAKE_VERSION_NUMBER(7, 0, 0)
#   endif
#   if !defined(MEGO_ARCH__ARM) && ( \
    defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || \
    defined(__ARM_ARCH_6KZ__) || defined(__ARM_ARCH_6T2__) )
#       define MEGO_ARCH__ARM MEGO__MAKE_VERSION_NUMBER(6, 0, 0)
#   endif
#   if !defined(MEGO_ARCH__ARM) && ( \
    defined(__ARM_ARCH_5TE__) || defined(__ARM_ARCH_5TEJ__) )
#       define MEGO_ARCH__ARM MEGO__MAKE_VERSION_NUMBER(5, 0, 0)
#   endif
#   if !defined(MEGO_ARCH__ARM) && ( \
    defined(__ARM_ARCH_4T__) || defined(__ARM_ARCH_4__) )
#       define MEGO_ARCH__ARM MEGO__MAKE_VERSION_NUMBER(4, 0, 0)
#   endif

#   define MEGO_ARCH__STRING "arm"

#endif

#ifndef MEGO_ARCH__ARM
#define MEGO_ARCH__ARM (0)
#endif

#endif