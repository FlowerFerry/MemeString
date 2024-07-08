

#ifndef MEGO_HARDWARE_CPU_INSTRUCTION_H_INCLUDED
#define MEGO_HARDWARE_CPU_INSTRUCTION_H_INCLUDED

#include <mego/predef/architecture/arm.h>
#include <mego/predef/architecture/x86.h>
#include <mego/predef/symbol/inline.h>

#include "id.h"

#ifdef __cplusplus
extern "C" {
#endif

//! @typedef mghw_simd_instruction_t
//! @brief 定义 SIMD 指令集支持标志的类型。
typedef uint64_t mghw_simd_instruction_t;
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

#if defined(__PPC64__)

MG_CAPI_INLINE mghw_simd_instruction_t
mghw_detect_supported_simd_instructions() {
    return MGHW_SIMD_INSTRUCTION__ALTIVEC;
}

#elif MEGO_ARCH__ARM && defined(__ARM_NEON)

MG_CAPI_INLINE mghw_simd_instruction_t
mghw_detect_supported_simd_instructions() {
    return MGHW_SIMD_INSTRUCTION__NEON;
}

#elif MEGO_ARCH__AMD64

//! @brief 检测并返回当前系统支持的 SIMD 指令集。
//!
//! 适用于 AMD64 架构的实现，使用 CPUID 指令检测支持的 SIMD 指令集合。
//!
//! @return 返回支持的 SIMD 指令集合。
MG_CAPI_INLINE mghw_simd_instruction_t
mghw_detect_supported_simd_instructions()
{ 
    uint32_t eax;
    uint32_t ebx = 0;
    uint32_t ecx = 0;
    uint32_t edx = 0;
    uint64_t val = 0x0;

  // EBX for EAX=0x1
    eax = 0x1;
    mghw_cpuid(&eax, &ebx, &ecx, &edx);

    if (ecx & MGHW_CPUID_BIT__SSE42) {
        val |= MGHW_SIMD_INSTRUCTION__SSE42;
    }

    if (ecx & MGHW_CPUID_BIT__PCLMULQDQ) {
        val |= MGHW_SIMD_INSTRUCTION__PCLMULQDQ;
    }

    // ECX for EAX=0x7
    eax = 0x7;
    ecx = 0x0; // Sub-leaf = 0
    mghw_cpuid(&eax, &ebx, &ecx, &edx);
    if (ebx & MGHW_CPUID_BIT__EBX_AVX2) {
        val |= MGHW_SIMD_INSTRUCTION__AVX2;
    }
    if (ebx & MGHW_CPUID_BIT__EBX_BMI1) {
        val |= MGHW_SIMD_INSTRUCTION__BMI1;
    }
    if (ebx & MGHW_CPUID_BIT__EBX_BMI2) {
        val |= MGHW_SIMD_INSTRUCTION__BMI2;
    }
    if (ebx & MGHW_CPUID_BIT__EBX_AVX512F) {
        val |= MGHW_SIMD_INSTRUCTION__AVX512F;
    }
    if (ebx & MGHW_CPUID_BIT__EBX_AVX512BW) {
        val |= MGHW_SIMD_INSTRUCTION__AVX512BW;
    }
    if (ebx & MGHW_CPUID_BIT__EBX_AVX512DQ) {
        val |= MGHW_SIMD_INSTRUCTION__AVX512DQ;
    }

    return val;
}

#else

//! @brief 检测并返回当前系统支持的 SIMD 指令集。
//!
//! 默认实现，不支持任何 SIMD 指令集。
//!
//! @return 返回支持的 SIMD 指令集合。
MG_CAPI_INLINE mghw_simd_instruction_t
mghw_detect_supported_simd_instructions() {
    return MGHW_SIMD_INSTRUCTION__DEFAULT;
}

#endif

#ifdef __cplusplus
}
#endif


#endif // !MEGO_HARDWARE_CPU_INSTRUCTION_H_INCLUDED

