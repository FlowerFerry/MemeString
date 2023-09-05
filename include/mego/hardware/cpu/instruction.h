
#ifndef MEGO_HARDWARE_CPU_INSTRUCTION_H_INCLUDED
#define MEGO_HARDWARE_CPU_INSTRUCTION_H_INCLUDED

#include <mego/predef/architecture/arm.h>
#include <mego/predef/architecture/x86.h>

#include "id.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t mghw_simd_instruction_t;
#define MGHW_SIMD_INSTRUCTION__DEFAULT   (0x0000)
#define MGHW_SIMD_INSTRUCTION__SSE2      (0x0001)
#define MGHW_SIMD_INSTRUCTION__SSE42     (0x0002)
#define MGHW_SIMD_INSTRUCTION__AVX2      (0x0008)
#define MGHW_SIMD_INSTRUCTION__AVX512F   (0x0010)
#define MGHW_SIMD_INSTRUCTION__AVX512BW  (0x0020)
#define MGHW_SIMD_INSTRUCTION__AVX512DQ  (0x0040)
#define MGHW_SIMD_INSTRUCTION__BMI1      (0x0200)
#define MGHW_SIMD_INSTRUCTION__BMI2      (0x0400)
#define MGHW_SIMD_INSTRUCTION__ALTIVEC   (0x0800)
#define MGHW_SIMD_INSTRUCTION__PCLMULQDQ (0x1000)
#define MGHW_SIMD_INSTRUCTION__NEON      (0x2000)

#if MEGO_ARCH__ARM && defined(__ARM_NEON)

inline mghw_simd_instruction_t
mghw_detect_supported_simd_instructions() {
    return MGHW_SIMD_INSTRUCTION__NEON;
}

#elif MEGO_ARCH__AMD64

inline mghw_simd_instruction_t
mghw_detect_supported_simd_instructions()
{
    return MGHW_SIMD_INSTRUCTION__DEFAULT;
}

#elif 

inline mghw_simd_instruction_t
mghw_detect_supported_simd_instructions() {
    return MGHW_SIMD_INSTRUCTION__DEFAULT;
}

#endif

#ifdef __cplusplus
}
#endif

#endif // !MEGO_HARDWARE_CPU_INSTRUCTION_H_INCLUDED
