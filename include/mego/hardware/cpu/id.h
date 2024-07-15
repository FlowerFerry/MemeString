
#ifndef MEGO_HARDWARE_CPU_ID_H_INCLUDED
#define MEGO_HARDWARE_CPU_ID_H_INCLUDED

#include <stdint.h>
#include <mego/predef/compiler/visualc.h>
#include <mego/predef/architecture/x86.h>
#include <mego/predef/os/linux.h>
#include <mego/predef/symbol/inline.h>


#if MEGO_COMP__MSVC__AVAILABLE
#include <intrin.h>
#elif defined(__has_include) && __has_include(<cpuid.h>)
#define MGOPT__USE_GCC_GET_CPUID (1)
#include <cpuid.h>
#elif MGOPT__USE_GCC_GET_CPUID
#include <cpuid.h>
#endif

#ifndef MGOPT__USE_GCC_GET_CPUID
#define MGOPT__USE_GCC_GET_CPUID (0)
#endif

#define MGHW_CPUID_BIT__NONE                   ((uint32_t) 0x0000)
#define MGHW_CPUID_BIT__PCLMULQDQ              ((uint32_t)1 <<  1)
#define MGHW_CPUID_BIT__SSE42                  ((uint32_t)1 << 20)
#define MGHW_CPUID_BIT__EBX_BMI1               ((uint32_t)1 <<  3)
#define MGHW_CPUID_BIT__EBX_BMI2               ((uint32_t)1 <<  8)
#define MGHW_CPUID_BIT__EBX_AVX2               ((uint32_t)1 <<  5)
#define MGHW_CPUID_BIT__EBX_AVX512F            ((uint32_t)1 << 16)
#define MGHW_CPUID_BIT__EBX_AVX512BW           ((uint32_t)1 << 30)
#define MGHW_CPUID_BIT__EBX_AVX512DQ           ((uint32_t)1 << 17)
#define MGHW_CPUID_BIT__EBX_AVX512IFMA         ((uint32_t)1 << 21)
#define MGHW_CPUID_BIT__EBX_AVX512CD           ((uint32_t)1 << 28)
#define MGHW_CPUID_BIT__EBX_AVX512VBMI         ((uint32_t)1 << 31)
#define MGHW_CPUID_BIT__ECX_AVX512VBMI         ((uint32_t)1 <<  1)
#define MGHW_CPUID_BIT__ECX_AVX512VBMI2        ((uint32_t)1 <<  6)
#define MGHW_CPUID_BIT__ECX_AVX512VNNI         ((uint32_t)1 << 11)
#define MGHW_CPUID_BIT__ECX_AVX512BITALG       ((uint32_t)1 << 12)
#define MGHW_CPUID_BIT__ECX_AVX512VPOPCNTDQ    ((uint32_t)1 << 14)
#define MGHW_CPUID_BIT__EDX_AVX512VP2INTERSECT ((uint32_t)1 <<  8)

#if MEGO_ARCH__AMD64

//! @brief 执行 CPUID 指令。
//!
//! 该函数执行 x86 的 CPUID 指令，并将结果存储在提供的寄存器变量中。
//!
//! @param[in,out] _eax 指向 eax 寄存器值的指针。在输入时指定 CPUID 的功能号，在输出时存储 CPUID 指令的返回值。
//! @param[in,out] _ebx 指向 ebx 寄存器值的指针。在输出时存储 CPUID 指令的返回值。
//! @param[in,out] _ecx 指向 ecx 寄存器值的指针。在输入时指定 CPUID 的子功能号，在输出时存储 CPUID 指令的返回值。
//! @param[in,out] _edx 指向 edx 寄存器值的指针。在输出时存储 CPUID 指令的返回值。
//!
//! 根据不同的编译器和系统环境，选择合适的方法执行 CPUID 指令。对于 MSVC 编译器，使用 `__cpuid` 内联函数。对于支持 `cpuid.h` 头文件的系统，使用 `__get_cpuid` 函数。否则，使用内联汇编代码执行 CPUID 指令。
MG_CAPI_INLINE void mghw_cpuid(uint32_t *_eax, uint32_t *_ebx, uint32_t *_ecx, uint32_t *_edx) 
{
#if MEGO_COMP__MSVC__AVAILABLE
  int cpu_info[4];
  __cpuid(cpu_info, *_eax);
  *_eax = cpu_info[0];
  *_ebx = cpu_info[1];
  *_ecx = cpu_info[2];
  *_edx = cpu_info[3];
#elif MGOPT__USE_GCC_GET_CPUID
  uint32_t level = *_eax;
  __get_cpuid(level, _eax, _ebx, _ecx, _edx);
#else
  uint32_t a = *_eax, b, c = *_ecx, d;
  __asm volatile("cpuid\n\t" : "+a"(a), "=b"(b), "+c"(c), "=d"(d));
  *_eax = a;
  *_ebx = b;
  *_ecx = c;
  *_edx = d;
#endif
}

#endif

#endif // !MEGO_HARDWARE_CPU_ID_H_INCLUDED
