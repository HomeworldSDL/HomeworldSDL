
#ifndef CPUID_H
#define CPUID_H

#ifdef _MSC_VER

//#include <windows.h>

#ifdef __cplusplus
extern "C"
#endif

#define CPU_FEATURE_MMX    0x0001
#define CPU_FEATURE_SSE    0x0002
#define CPU_FEATURE_SSE2   0x0004
#define CPU_FEATURE_3DNOW  0x0008

/*
check whether various instructions are supported

Input: see below

Output: returns 1 if feature is supported, 0 if not

see also: 
> intel ia32 Architecture Software Developer’s Manual Volume 1; basic architecture.pdf
11.6.2 Checking for SSE/SSE2 Support

> Cracking Open The Pentium III by Rob Wyatt
http://www.gamasutra.com/features/wyatts_world/19990528/pentium3_03.htm

> code search (google, koders, etc)
#define TEST_SSE()  __asm __volatile (".byte 0x0f, 0x57, 0xc0")
*/
int has_feature(int feature)
{
  __try {
      switch (feature) {
      case CPU_FEATURE_SSE:
          __asm {
              xorps xmm0, xmm0
          }
          break;
      case CPU_FEATURE_SSE2:
          __asm {
              xorpd xmm0, xmm0
          }
          break;
      case CPU_FEATURE_3DNOW:
          __asm {
              pfrcp mm0, mm0
              emms
          }
          break;
      case CPU_FEATURE_MMX:
          __asm {
              pxor mm0, mm0
              emms
          }
          break;
      }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
      if (_exception_code() == STATUS_ILLEGAL_INSTRUCTION) {
          return 0;
      }
      return 0;
  }
  return 1;
}

#endif
#endif