/*=============================================================================
    Name    : asm.c
    Purpose : assembly language versions of some rGL functions

    Created 7/13/1998 by khent
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

//#define _MM_FUNCTIONALITY
#include <xmmintrin.h>
#include <string.h>
#include "asm.h"
#include "_stdint.h"

#define XMM_VB_SIZE (((VB_SIZE + 3) & (~3)) / 4)
#if defined (_MSC_VER)
    #define SELECT(i0,i1,i2,i3) (i0*64 + i1*16 + i2*4 + i3)
#elif defined (__GNUC__) && defined (__i386__)
    #define SELECT(i0,i1,i2,i3) "$(" #i0 "*64 + " #i1 "*16 + " #i2 "*4 + " #i3 ")"
#endif

#ifdef _MSC_VER

_MM_ALIGN16 struct sInVerts
{
    __m128 x[XMM_VB_SIZE];
    __m128 y[XMM_VB_SIZE];
    __m128 z[XMM_VB_SIZE];
} sInputVerts;

_MM_ALIGN16 struct sOutVerts
{
    __m128 x[XMM_VB_SIZE];
    __m128 y[XMM_VB_SIZE];
    __m128 z[XMM_VB_SIZE];
    __m128 w[XMM_VB_SIZE];
} sOutputVerts;

#else

struct sInVerts
{
    __m128 x[XMM_VB_SIZE];
    __m128 y[XMM_VB_SIZE];
    __m128 z[XMM_VB_SIZE];
} sInputVerts __attribute__ ((aligned (16)));

struct sOutVerts
{
    __m128 x[XMM_VB_SIZE];
    __m128 y[XMM_VB_SIZE];
    __m128 z[XMM_VB_SIZE];
    __m128 w[XMM_VB_SIZE];
} sOutputVerts __attribute__ ((aligned (16)));

#endif

void gl_intrin_project(GLfloat* dest, GLfloat* source, GLfloat* matrix, GLuint count);
void gl_xmm_project(GLfloat* dest, GLfloat* source, GLfloat* matrix, GLuint count);
void transform_points4_general(GLuint n, GLfloat* d, GLfloat* m, GLfloat* s);
void transform_points4_perspective(GLuint n, GLfloat* d, GLfloat* m, GLfloat* s);

#define FSIZE      4
#define FSIZE_STR "4"
#if defined (_MSC_VER)
    #define FPTR    dword ptr

    #define S(x)   [esi + FSIZE*x]
    #define D(x)   [edi + FSIZE*x]
    #define M(n)   [edx + FSIZE*n]

    #define SOURCE  esi
    #define DEST    ebx
    #define MATRIX  edi
#elif defined (__GNUC__) && defined (__i386__)
    #define FPTR

    #define S(x)    "4*" #x "(%%esi)"
    #define D(x)    "4*" #x "(%%edi)"
    #define M(n)    "4*" #n "(%%edx)"

    // Remember to change the source registers in
    // gl_wicked_fast_normal_xform() and gl_fairly_fast_scaled_normal_xform()
    // if you change these.
    #define SOURCE  "%%esi"
    #define DEST    "%%edx"
    #define MATRIX  "%%edi"
#endif

static int chkcpubit()
{
    int rval;
#if defined (_MSC_VER)
    _asm
    {
        pusha

        pushfd
        pop eax
        test eax, 0x00200000
        jz set_21
        and eax, 0xffdfffff
        push eax
        popfd
        pushfd
        pop eax
        test eax, 0x00200000
        jz cpu_id_ok
        jmp err

    set_21:
        or eax, 0x00200000
        push eax
        popfd
        pushfd
        pop eax
        test eax, 0x00200000
        jnz cpu_id_ok

    err:
        mov eax, 0
        jmp exit

    cpu_id_ok:
        mov eax, 1

    exit:
        mov [rval], eax

        popa
    }
#elif defined (__GNUC__) && defined (__i386__)
    __asm__ (
        "    pushfl\n"
        "    popl %%eax\n"
        "    testl $0x00200000, %%eax\n"
        "    jz chkcpubit_set_21\n"
        "    andl $0xffdfffff, %%eax\n"
        "    pushl %%eax\n"
        "    popfl\n"
        "    pushfl\n"
        "    popl %%eax\n"
        "    testl $0x00200000, %%eax\n"
        "    jz chkcpubit_cpu_id_ok\n"
        "    jmp chkcpubit_err\n"

        "chkcpubit_set_21:\n"
        "    orl $0x00200000, %%eax\n"
        "    pushl %%eax\n"
        "    popfl\n"
        "    pushfl\n"
        "    popl %%eax\n"
        "    testl $0x00200000, %%eax\n"
        "    jnz chkcpubit_cpu_id_ok\n"

        "chkcpubit_err:\n"
        "    movl $0, %%eax\n"
        "    jmp chkcpubit_exit\n"

        "chkcpubit_cpu_id_ok:\n"
        "    movl $1, %%eax\n"

        "chkcpubit_exit:\n"
        : "=a" (rval) );
#else
    #error chkcpubit(): Not implemented on this platform.
    rval = 0;
#endif
    return rval;
}

static GLuint cpu_eax, cpu_edx;

GLuint get_cputype()
{
    GLuint ptype;

    if (!chkcpubit())
    {
        cpu_edx = cpu_eax = 0;
        return 4;
    }
    else
    {
#if defined (_MSC_VER)
        _asm
        {
            pusha

            mov eax, 1
//            cpuid
            _emit 0x0f
            _emit 0xa2
            mov [cpu_eax], eax
            mov [cpu_edx], edx
            and eax, 0x0f00
            shr eax, 8
            mov [ptype], eax

            popa
        }
        return ptype;
#elif defined (__GNUC__) && defined (__i386__)
        __asm__ (
            "    pushl %%ebx\n"
            "    movl $1, %%eax\n"
            "    cpuid\n"
            "    popl %%ebx\n"
            : "=a" (cpu_eax), "=d" (cpu_edx)
            :
            : "ecx" );

        return (cpu_eax & 0x0f00) >> 8;
#endif
    }
}

//only call after get_cputype()
GLboolean get_cpummx()
{
    //check MMX bit (23)
    if (cpu_edx & 0x400000)
    {
        return GL_TRUE;
    }
    else
    {
        if (((cpu_eax & 0x0f00) >= 0x600) &&
            ((cpu_eax & 0x0f0) >= 0x30))
        {
            //pentium II has MMX
            return GL_TRUE;
        }
        //no MMX
        return GL_FALSE;
    }
}

//only call after get_cputype()
GLboolean get_cpukatmai()
{
    if ((cpu_edx & 0x2000000) &&    //XMM
        (cpu_edx & 0x1000000))      //FXSR
    {
        return GL_TRUE;
    }
    else
    {
        return GL_FALSE;
    }
}

void transform_points4_identity(GLuint n, GLfloat* d, GLfloat* m, GLfloat* s)
{
    MEMCPY(d, s, n*4*sizeof(GLfloat));
}

void asm_project_and_cliptest_general(
    GLuint n, GLfloat* d, GLfloat* m, GLfloat* s,
    GLubyte* clipmask, GLubyte* ormask, GLubyte* andmask)
{
    transform_points4_general(n, d, m, s);
    asm_cliptest(n, d, clipmask, ormask, andmask);
}

void asm_project_and_cliptest_identity(
    GLuint n, GLfloat* d, GLfloat* m, GLfloat* s,
    GLubyte* clipmask, GLubyte* ormask, GLubyte* andmask)
{
    transform_points4_identity(n, d, m, s);
    asm_cliptest(n, d, clipmask, ormask, andmask);
}

void asm_project_and_cliptest_perspective(
    GLuint n, GLfloat* d, GLfloat* m, GLfloat* s,
    GLubyte* clipmask, GLubyte* ormask, GLubyte* andmask)
{
    transform_points4_perspective(n, d, m, s);
    asm_cliptest(n, d, clipmask, ormask, andmask);
}

typedef struct
{
    float x, y, z, w;
} hvector;

#if defined (_MSC_VER)
_MM_ALIGN16 __m128 mat[16], per[16];
#else
__m128 mat[16] __attribute__ ((aligned (16))),
       per[16] __attribute__ ((aligned (16)));
#endif

void intrin_project_and_cliptest_perspective(
    GLuint n, GLfloat* d, GLfloat* m, GLfloat* s,
    GLubyte* clipmask, GLubyte* ormask, GLubyte* andmask)
{
    gl_intrin_project(d, s, m, n);
    asm_cliptest(n, d, clipmask, ormask, andmask);
}

void xmm_project_and_cliptest_perspective(
    GLuint n, GLfloat* d, GLfloat* m, GLfloat* s,
    GLubyte* clipmask, GLubyte* ormask, GLubyte* andmask)
{
    gl_xmm_project(d, s, m, n);
    asm_cliptest(n, d, clipmask, ormask, andmask);
}

void xmm_set_modelview(GLfloat* m)
{
    static float* m0;
    static float* mat0;

    m0 = m;
    mat0 = (float*)mat;

#if defined (_MSC_VER)
    _asm
    {
        push    esi
        push    edi

        mov     esi, [m0]
        mov     edi, [mat0]

        movss   xmm0, DWORD PTR [esi + 4*0]
        movss   xmm1, DWORD PTR [esi + 4*1]
        shufps  xmm0, xmm0, 00h
        movss   xmm2, DWORD PTR [esi + 4*2]
        shufps  xmm1, xmm1, 00h
        movaps  XMMWORD PTR [edi + 4*4*0], xmm0
        movss   xmm3, DWORD PTR [esi + 4*4]
        shufps  xmm2, xmm2, 00h
        movaps  XMMWORD PTR [edi + 4*4*1], xmm1
        movss   xmm4, DWORD PTR [esi + 4*5]
        shufps  xmm3, xmm3, 00h
        movaps  XMMWORD PTR [edi + 4*4*2], xmm2
        movss   xmm5, DWORD PTR [esi + 4*6]
        shufps  xmm4, xmm4, 00h
        movaps  XMMWORD PTR [edi + 4*4*4], xmm3
        movss   xmm6, DWORD PTR [esi + 4*8]
        shufps  xmm5, xmm5, 00h
        movaps  XMMWORD PTR [edi + 4*4*5], xmm4
        movss   xmm7, DWORD PTR [esi + 4*9]
        shufps  xmm6, xmm6, 00h
        movaps  XMMWORD PTR [edi + 4*4*6], xmm5
        shufps  xmm7, xmm7, 00h
        movaps  XMMWORD PTR [edi + 4*4*8], xmm6
        movaps  XMMWORD PTR [edi + 4*4*9], xmm7

        movss   xmm0, DWORD PTR [esi + 4*10]
        movss   xmm1, DWORD PTR [esi + 4*12]
        shufps  xmm0, xmm0, 00h
        movss   xmm2, DWORD PTR [esi + 4*13]
        shufps  xmm1, xmm1, 00h
        movaps  XMMWORD PTR [edi + 4*4*10], xmm0
        movss   xmm3, DWORD PTR [esi + 4*14]
        shufps  xmm2, xmm2, 00h
        movaps  XMMWORD PTR [edi + 4*4*12], xmm1
        shufps  xmm3, xmm3, 00h
        movaps  XMMWORD PTR [edi + 4*4*13], xmm2
        movaps  XMMWORD PTR [edi + 4*4*14], xmm3

        pop     edi
        pop     esi
    }
#elif defined (__GNUC__) && defined (__i386__)
    __asm__ (
        "    movss   4*0(%%esi), %%xmm0\n"
        "    movss   4*1(%%esi), %%xmm1\n"
        "    shufps  $0x00, %%xmm0, %%xmm0\n"
        "    movss   4*2(%%esi), %%xmm2\n"
        "    shufps  $0x00, %%xmm1, %%xmm1\n"
        "    movaps  %%xmm0, 4*4*0(%%edi)\n"
        "    movss   4*4(%%esi), %%xmm3\n"
        "    shufps  $0x00, %%xmm2, %%xmm2\n"
        "    movaps  %%xmm1, 4*4*1(%%edi)\n"
        "    movss   4*5(%%esi), %%xmm4\n"
        "    shufps  $0x00, %%xmm3, %%xmm3\n"
        "    movaps  %%xmm2, 4*4*2(%%edi)\n"
        "    movss   4*6(%%esi), %%xmm5\n"
        "    shufps  $0x00, %%xmm4, %%xmm4\n"
        "    movaps  %%xmm3, 4*4*4(%%edi)\n"
        "    movss   4*8(%%esi), %%xmm6\n"
        "    shufps  $0x00, %%xmm5, %%xmm5\n"
        "    movaps  %%xmm4, 4*4*5(%%edi)\n"
        "    movss   4*9(%%esi), %%xmm7\n"
        "    shufps  $0x00, %%xmm6, %%xmm6\n"
        "    movaps  %%xmm5, 4*4*6(%%edi)\n"
        "    shufps  $0x00, %%xmm7, %%xmm7\n"
        "    movaps  %%xmm6, 4*4*8(%%edi)\n"
        "    movaps  %%xmm7, 4*4*9(%%edi)\n"

        "    movss   4*10(%%esi), %%xmm0\n"
        "    movss   4*12(%%esi), %%xmm1\n"
        "    shufps  $0x00, %%xmm0, %%xmm0\n"
        "    movss   4*13(%%esi), %%xmm2\n"
        "    shufps  $0x00, %%xmm1, %%xmm1\n"
        "    movaps  %%xmm0, 4*4*10(%%edi)\n"
        "    movss   4*14(%%esi), %%xmm3\n"
        "    shufps  $0x00, %%xmm2, %%xmm2\n"
        "    movaps  %%xmm1, 4*4*12(%%edi)\n"
        "    shufps  $0x00, %%xmm3, %%xmm3\n"
        "    movaps  %%xmm2, 4*4*13(%%edi)\n"
        "    movaps  %%xmm3, 4*4*14(%%edi)\n"
        :
        : "S" (m0), "D" (mat0) );
#endif
}

void xmm_set_projection(GLfloat* m)
{
    static float* m1;
    static float* mat1;

    m1 = m;
    mat1 = (float*)per;

#if defined (_MSC_VER)
    _asm
    {
        push    eax
        push    edx

        mov     eax, [m1]
        mov     edx, [mat1]

        movss   xmm0, DWORD PTR [eax + 4*0]
        movss   xmm1, DWORD PTR [eax + 4*5]
        shufps  xmm0, xmm0, 00h
        movss   xmm2, DWORD PTR [eax + 4*8]
        shufps  xmm1, xmm1, 00h
        movaps  XMMWORD PTR [edx + 4*4*0], xmm0
        movss   xmm3, DWORD PTR [eax + 4*9]
        shufps  xmm2, xmm2, 00h
        movaps  XMMWORD PTR [edx + 4*4*5], xmm1
        movss   xmm4, DWORD PTR [eax + 4*10]
        shufps  xmm3, xmm3, 00h
        movaps  XMMWORD PTR [edx + 4*4*8], xmm2
        movss   xmm5, DWORD PTR [eax + 4*14]
        shufps  xmm4, xmm4, 00h
        movaps  XMMWORD PTR [edx + 4*4*9], xmm3
        shufps  xmm5, xmm5, 00h
        movaps  XMMWORD PTR [edx + 4*4*10], xmm4
        movaps  XMMWORD PTR [edx + 4*4*14], xmm5

        pop     edx
        pop     eax
    }
#elif defined (__GNUC__) && defined (__i386__)
    __asm__ (
        "    movss   4*0(%%eax), %%xmm0\n"
        "    movss   4*5(%%eax), %%xmm1\n"
        "    shufps  $0x00, %%xmm0, %%xmm0\n"
        "    movss   4*8(%%eax), %%xmm2\n"
        "    shufps  $0x00, %%xmm1, %%xmm1\n"
        "    movaps  %%xmm0, 4*4*0(%%edx)\n"
        "    movss   4*9(%%eax), %%xmm3\n"
        "    shufps  $0x00, %%xmm2, %%xmm2\n"
        "    movaps  %%xmm1, 4*4*5(%%edx)\n"
        "    movss   4*10(%%eax), %%xmm4\n"
        "    shufps  $0x00, %%xmm3, %%xmm3\n"
        "    movaps  %%xmm2, 4*4*8(%%edx)\n"
        "    movss   4*14(%%eax), %%xmm5\n"
        "    shufps  $0x00, %%xmm4, %%xmm4\n"
        "    movaps  %%xmm3, 4*4*9(%%edx)\n"
        "    shufps  $0x00, %%xmm5, %%xmm5\n"
        "    movaps  %%xmm4, 4*4*10(%%edx)\n"
        "    movaps  %%xmm5, 4*4*14(%%edx)\n"
        :
        : "a" (m1), "d" (mat1) );
#endif
}

void xmm_update_modelview(GLcontext* ctx) {}
void xmm_update_projection(GLcontext* ctx) {}

void gl_xmm_3dtransform(GLfloat* dest, GLfloat* source, GLfloat* matrix, GLuint count)
{
#if defined (_MSC_VER)
    static _MM_ALIGN16 int n;
    static _MM_ALIGN16 __m128 One, nextX;
    static _MM_ALIGN16 float* m;
#else
    static int n __attribute__ ((aligned (16)));
    static __m128 One   __attribute__ ((aligned (16))),
                  nextX __attribute__ ((aligned (16)));
    static float* m __attribute__ ((aligned (16)));
#endif

    n = (count + 3) & (~3);

    One = _mm_set_ps1(1.0f);
    m = (float*)mat;

    xmm_set_modelview(matrix);

#if defined (_MSC_VER)
    _asm
    {
        push    esi

        mov     esi, [m]
        mov     ecx, [n]
        mov     eax, [source]
        mov     edx, [dest]
        shl     ecx, 4

    start:
        sub     ecx, 64

        //swizzle
        movups  xmm7, [eax+ecx]
        movups  xmm2, [eax+ecx+16]
        movups  xmm4, [eax+ecx+32]
        movups  xmm3, [eax+ecx+48]
        prefetchnta [eax+ecx-64]
        movaps  xmm6, xmm7
        movaps  xmm5, xmm4
        unpcklps xmm6, xmm2
        unpcklps xmm5, xmm3
        movaps  xmm0, xmm6
        prefetchnta [eax+ecx+16-64]
        shufps  xmm0, xmm5, SELECT(1,0,1,0) //xxxx in xmm0
        shufps  xmm6, xmm5, SELECT(3,2,3,2) //yyyy in xmm6
        unpckhps xmm7, xmm2
        prefetchnta [eax+ecx+32-64]
        unpckhps xmm4, xmm3

        //transform
        movaps  xmm3, XMMWORD PTR [esi+4*4*0]
        movaps  xmm1, XMMWORD PTR [esi+4*4*4]
        mulps   xmm3, xmm0          //x*m0 in xmm3
        prefetchnta [eax+ecx+48-64]
        shufps  xmm7, xmm4, SELECT(1,0,1,0) //zzzz in xmm7
        mulps   xmm1, xmm6          //y*m4 in xmm1
        movaps  xmm4, XMMWORD PTR [esi+4*4*8]
        movaps  xmm5, XMMWORD PTR [esi+4*4*12]
        mulps   xmm4, xmm7          //z*m8 in xmm4
        addps   xmm1, xmm3          //x*m0 + y*m4 in xmm1
        addps   xmm4, xmm5          //z*m8 + m12 in xmm4
        addps   xmm1, xmm4          //x*m0 + y*m4 + z*m8 + m12 in xmm1
        movaps  XMMWORD PTR [nextX], xmm1

        movaps  xmm3, XMMWORD PTR [esi+4*4*1]
        movaps  xmm2, XMMWORD PTR [esi+4*4*5]
        mulps   xmm3, xmm0          //x*m1 in xmm3
        mulps   xmm2, xmm6          //y*m5 in xmm2
        movaps  xmm4, XMMWORD PTR [esi+4*4*9]
        movaps  xmm5, XMMWORD PTR [esi+4*4*13]
        mulps   xmm4, xmm7          //z*m9 in xmm4
        addps   xmm2, xmm3          //x*m1 + y*m5 in xmm2
        addps   xmm4, xmm5          //z*m9 + m13 in xmm4
        movaps  xmm3, XMMWORD PTR [esi+4*4*2]   //...
        addps   xmm4, xmm2          //x*m1 + y*m5 + z*m9 + m13 in xmm4

        movaps  xmm1, XMMWORD PTR [esi+4*4*6]
        mulps   xmm3, xmm0          //x*m2 in xmm3
        mulps   xmm1, xmm6          //y*m6 in xmm1
        movaps  xmm2, XMMWORD PTR [esi+4*4*10]
        movaps  xmm5, XMMWORD PTR [esi+4*4*14]
        mulps   xmm2, xmm7          //z*m10 in xmm4
        addps   xmm1, xmm3          //x*m2 + y*m6 in xmm1
        addps   xmm2, xmm5          //z*m10 + m14 in xmm4
        addps   xmm1, xmm2          //x*m2 + y*m6 + z*m10 + m14 in xmm1

        //swizzle
        movaps  xmm2, [nextX]       //x
        movaps  xmm6, xmm1          //z
        movaps  xmm3, [One]         //w
        movaps  xmm0, xmm2
        unpcklps xmm0, xmm4                 //y1,x1,y0,x0
        movaps  xmm1, xmm0
        movaps  xmm5, xmm6
        unpcklps xmm5, xmm3                 //w1,z1,w0,z0
        shufps  xmm1, xmm5, SELECT(1,0,1,0) //w0,z0,y0,x0
        movups  [edx+ecx], xmm1
        shufps  xmm0, xmm5, SELECT(3,2,3,2) //w1,z1,y1,x1
        movups  [edx+ecx+16], xmm0
        movaps  xmm0, xmm2
        unpckhps xmm0, xmm4                 //y3,x3,y2,x2
        movaps  xmm1, xmm0
        movaps  xmm5, xmm6
        unpckhps xmm5, xmm3                 //w3,z3,w2,z2
        shufps  xmm1, xmm5, SELECT(1,0,1,0) //w2,z2,y2,x2
        movups  [edx+ecx+32], xmm1
        shufps  xmm0, xmm5, SELECT(3,2,3,2) //w3,z3,y3,x3
        movups  [edx+ecx+48], xmm0

        jnz     start

        pop     esi
    }
#elif defined (__GNUC__) && defined (__i386__)
    __asm__ (
        "    shll $4, %%ecx\n"

        "xmm3dtrans_start:\n"
        "    subl $64, %%ecx\n"

        //swizzle
        "    movups   0(%%eax, %%ecx), %%xmm7\n"
        "    movups  16(%%eax, %%ecx), %%xmm2\n"
        "    movups  32(%%eax, %%ecx), %%xmm4\n"
        "    movups  48(%%eax, %%ecx), %%xmm3\n"
        "    prefetchnta -64(%%eax, %%ecx)\n"
        "    movaps  %%xmm7, %%xmm6\n"
        "    movaps  %%xmm4, %%xmm5\n"
        "    unpcklps %%xmm2, %%xmm6\n"
        "    unpcklps %%xmm3, %%xmm5\n"
        "    movaps  %%xmm6, %%xmm0\n"
        "    prefetchnta 16-64(%%eax, %%ecx)\n"
        "    shufps  " SELECT(1,0,1,0) ", %%xmm5, %%xmm0\n" //xxxx in xmm0
        "    shufps  " SELECT(3,2,3,2) ", %%xmm5, %%xmm6\n" //yyyy in xmm6
        "    unpckhps %%xmm2, %%xmm7\n"
        "    prefetchnta 32-64(%%eax, %%ecx)\n"
        "    unpckhps %%xmm3, %%xmm4\n"

        //transform
        "    movaps  4*4*0(%%esi), %%xmm3\n"
        "    movaps  4*4*4(%%esi), %%xmm1\n"
        "    mulps   %%xmm0, %%xmm3\n"          //x*m0 in xmm3
        "    prefetchnta 48-64(%%eax, %%ecx)\n"
        "    shufps  " SELECT(1,0,1,0) ", %%xmm4, %%xmm7\n" //zzzz in xmm7
        "    mulps   %%xmm6, %%xmm1\n"          //y*m4 in xmm1
        "    movaps  4*4*8(%%esi), %%xmm4\n"
        "    movaps  4*4*12(%%esi), %%xmm5\n"
        "    mulps   %%xmm7, %%xmm4\n"          //z*m8 in xmm4
        "    addps   %%xmm3, %%xmm1\n"          //x*m0 + y*m4 in xmm1
        "    addps   %%xmm5, %%xmm4\n"          //z*m8 + m12 in xmm4
        "    addps   %%xmm4, %%xmm1\n"          //x*m0 + y*m4 + z*m8 + m12 in xmm1
        "    movaps  %%xmm1, %5\n"

        "    movaps  4*4*1(%%esi), %%xmm3\n"
        "    movaps  4*4*5(%%esi), %%xmm2\n"
        "    mulps   %%xmm0, %%xmm3\n"          //x*m1 in xmm3
        "    mulps   %%xmm6, %%xmm2\n"          //y*m5 in xmm2
        "    movaps  4*4*9(%%esi), %%xmm4\n"
        "    movaps  4*4*13(%%esi), %%xmm5\n"
        "    mulps   %%xmm7, %%xmm4\n"          //z*m9 in xmm4
        "    addps   %%xmm3, %%xmm2\n"          //x*m1 + y*m5 in xmm2
        "    addps   %%xmm5, %%xmm4\n"          //z*m9 + m13 in xmm4
        "    movaps  4*4*2(%%esi), %%xmm3\n"   //...
        "    addps   %%xmm2, %%xmm4\n"          //x*m1 + y*m5 + z*m9 + m13 in xmm4

        "    movaps  4*4*6(%%esi), %%xmm1\n"
        "    mulps   %%xmm0, %%xmm3\n"          //x*m2 in xmm3
        "    mulps   %%xmm6, %%xmm1\n"          //y*m6 in xmm1
        "    movaps  4*4*10(%%esi), %%xmm2\n"
        "    movaps  4*4*14(%%esi), %%xmm5\n"
        "    mulps   %%xmm7, %%xmm2\n"          //z*m10 in xmm4
        "    addps   %%xmm3, %%xmm1\n"          //x*m2 + y*m6 in xmm1
        "    addps   %%xmm5, %%xmm2\n"          //z*m10 + m14 in xmm4
        "    addps   %%xmm2, %%xmm1\n"          //x*m2 + y*m6 + z*m10 + m14 in xmm1

        //swizzle
        "    movaps  %5, %%xmm2\n"              //x
        "    movaps  %%xmm1, %%xmm6\n"          //z
        "    movaps  %4, %%xmm3\n"              //w
        "    movaps  %%xmm2, %%xmm0\n"
        "    unpcklps %%xmm4, %%xmm0\n"                 //y1,x1,y0,x0
        "    movaps  %%xmm0, %%xmm1\n"
        "    movaps  %%xmm6, %%xmm5\n"
        "    unpcklps %%xmm3, %%xmm5\n"                 //w1,z1,w0,z0
        "    shufps  " SELECT(1,0,1,0) ", %%xmm5, %%xmm1\n" //w0,z0,y0,x0
        "    movups  %%xmm1, (%%edx, %%ecx)\n"
        "    shufps  " SELECT(3,2,3,2) ", %%xmm5, %%xmm0\n" //w1,z1,y1,x1
        "    movups  %%xmm0, 16(%%edx, %%ecx)\n"
        "    movaps  %%xmm2, %%xmm0\n"
        "    unpckhps %%xmm4, %%xmm0\n"                 //y3,x3,y2,x2
        "    movaps  %%xmm0, %%xmm1\n"
        "    movaps  %%xmm6, %%xmm5\n"
        "    unpckhps %%xmm3, %%xmm5\n"                 //w3,z3,w2,z2
        "    shufps  " SELECT(1,0,1,0) ", %%xmm5, %%xmm1\n" //w2,z2,y2,x2
        "    movups  %%xmm1, 32(%%edx, %%ecx)\n"
        "    shufps  " SELECT(3,2,3,2) ", %%xmm5, %%xmm0\n" //w3,z3,y3,x3
        "    movups  %%xmm0, 48(%%edx, %%ecx)\n"

        "    jnz     xmm3dtrans_start\n"
        :
        : "S" (m), "c" (n), "a" (source), "d" (dest),
          "m" (One), "m" (nextX) );
#endif
}

void gl_intrin_3dtransform(GLfloat* dest, GLfloat* source, GLfloat* matrix, GLuint count)
{
    int i, j;
    hvector* sp;
    hvector* dp;
    float* xp;

#if defined (_MSC_VER)
    _MM_ALIGN16 __m128 curX, curY, curZ;
    _MM_ALIGN16 __m128 out[4];
#else
    __m128 curX __attribute__ ((aligned (16))),
           curY __attribute__ ((aligned (16))),
           curZ __attribute__ ((aligned (16)));
    __m128 out[4] __attribute__ ((aligned (16)));
#endif

    count = ((count + 3) & (~3)) >> 2;

    xmm_set_modelview(matrix);

    xp = (float*)&out[0];
    dp = (hvector*)dest;
    sp = (hvector*)source;
    for (i = 0; i < count; i++)
    {
        curX = _mm_set_ps((sp+4*i+3)->x, (sp+4*i+2)->x, (sp+4*i+1)->x, (sp+4*i+0)->x);
        curY = _mm_set_ps((sp+4*i+3)->y, (sp+4*i+2)->y, (sp+4*i+1)->y, (sp+4*i+0)->y);
        curZ = _mm_set_ps((sp+4*i+3)->z, (sp+4*i+2)->z, (sp+4*i+1)->z, (sp+4*i+0)->z);

        out[0] = _mm_add_ps(
                     _mm_add_ps(
                         _mm_mul_ps(curX, mat[0]),
                         _mm_mul_ps(curY, mat[4])),
                     _mm_add_ps(
                         _mm_mul_ps(curZ, mat[8]),
                         mat[12]));
        out[1] = _mm_add_ps(
                     _mm_add_ps(
                         _mm_mul_ps(curX, mat[1]),
                         _mm_mul_ps(curY, mat[5])),
                     _mm_add_ps(
                         _mm_mul_ps(curZ, mat[9]),
                         mat[13]));
        out[2] = _mm_add_ps(
                     _mm_add_ps(
                         _mm_mul_ps(curX, mat[2]),
                         _mm_mul_ps(curY, mat[6])),
                     _mm_add_ps(
                         _mm_mul_ps(curZ, mat[10]),
                         mat[14]));

        for (j = 0; j < 4; j++, dp++)
        {
            dp->x = *(xp + j + 0);
            dp->y = *(xp + j + 4);
            dp->z = *(xp + j + 8);
            dp->w = 1.0f;
        }
    }
}

void gl_intrin_project(GLfloat* dest, GLfloat* source, GLfloat* matrix, GLuint count)
{
    GLint i, j;
    hvector* sp;
    hvector* dp;
    float* xp;

#if defined (_MSC_VER)
    _MM_ALIGN16 __m128 curX, curY, curZ;
    _MM_ALIGN16 __m128 out[4];
    _MM_ALIGN16 int mask[4];
#else
    __m128 curX __attribute__ ((aligned (16))),
           curY __attribute__ ((aligned (16))),
           curZ __attribute__ ((aligned (16)));
    __m128 out[4] __attribute__ ((aligned (16)));
    int mask[4] __attribute__ ((aligned (16)));
#endif

    mask[0] = mask[1] = mask[2] = mask[3] = 0x80000000;

    count = ((count + 3) & (~3)) >> 2;

    xmm_set_projection(matrix);

    xp = (float*)&out[0];
    dp = (hvector*)dest;
    sp = (hvector*)source;
    for (i = 0; i < count; i++)
    {
        curX = _mm_set_ps((sp+4*i+3)->x, (sp+4*i+2)->x, (sp+4*i+1)->x, (sp+4*i+0)->x);
        curY = _mm_set_ps((sp+4*i+3)->y, (sp+4*i+2)->y, (sp+4*i+1)->y, (sp+4*i+0)->y);
        curZ = _mm_set_ps((sp+4*i+3)->z, (sp+4*i+2)->z, (sp+4*i+1)->z, (sp+4*i+0)->z);

        out[0] = _mm_add_ps(
                    _mm_mul_ps(curX, per[0]),
                    _mm_mul_ps(curY, per[8]));
        out[1] = _mm_add_ps(
                    _mm_mul_ps(curY, per[5]),
                    _mm_mul_ps(curZ, per[9]));
        out[2] = _mm_add_ps(
                    _mm_mul_ps(curZ, per[10]),
                    per[14]);
        out[3] = _mm_xor_ps(curZ, (*((__m128*)(&(mask[0])))));

        for (j = 0; j < 4; j++, dp++)
        {
            dp->x = *(xp + j + 0);
            dp->y = *(xp + j + 4);
            dp->z = *(xp + j + 8);
            dp->w = *(xp + j + 12);
        }
    }
}

/*
 * transform an array of vertices by a 4x4 matrix, really quickly
 */
void gl_megafast_affine_transform(
        GLfloat* dest, GLfloat* source, GLfloat* matrix, GLuint count)
{
    GLfloat floatOne = 1.0f;

#if defined (_MSC_VER)
    _asm
    {
        mov ecx, count
        mov esi, [source]
        mov ebx, [dest]
        mov edi, [matrix]
        shl ecx, 4
        add esi, ecx
        add ebx, ecx

        neg ecx

        cmp ecx, 0
        jne MEGA0
        ret

    MEGA0:
        fld     FPTR [esi+ecx+0*FSIZE]
        fmul    FPTR [edi+(0*4+0)*FSIZE]
        fld     FPTR [esi+ecx+1*FSIZE]
        fmul    FPTR [edi+(1*4+0)*FSIZE]
        fld     FPTR [esi+ecx+2*FSIZE]
        fmul    FPTR [edi+(2*4+0)*FSIZE]
        fxch    st(1)
        faddp   st(2), st
        fld     FPTR [esi+ecx+0*FSIZE]
        fmul    FPTR [edi+(0*4+1)*FSIZE]
        fxch    st(1)
        faddp   st(2), st
        fld     FPTR [esi+ecx+1*FSIZE]
        fmul    FPTR [edi+(1*4+1)*FSIZE]
        fld     FPTR [esi+ecx+2*FSIZE]
        fmul    FPTR [edi+(2*4+1)*FSIZE]
        fxch    st(1)
        faddp   st(2), st
        fld     FPTR [esi+ecx+0*FSIZE]
        fmul    FPTR [edi+(0*4+2)*FSIZE]
        fxch    st(1)
        faddp   st(2), st
        fld     FPTR [esi+ecx+1*FSIZE]
        fmul    FPTR [edi+(1*4+2)*FSIZE]
        fld     FPTR [esi+ecx+2*FSIZE]
        fmul    FPTR [edi+(2*4+2)*FSIZE]
        fxch    st(1)
        faddp   st(2), st
        fxch    st(3)
        fadd    FPTR [edi+(3*4+0)*FSIZE]
        fxch    st(1)
        faddp   st(3), st
        fxch    st(1)
        fadd    FPTR [edi+(3*4+1)*FSIZE]
        fxch    st(2)
        fadd    FPTR [edi+(3*4+2)*FSIZE]
        fxch    st(1)
        fstp    FPTR [ebx+ecx+0*FSIZE]
        fxch    st(1)
        fstp    FPTR [ebx+ecx+1*FSIZE]
        fstp    FPTR [ebx+ecx+2*FSIZE]

        fld     FPTR [floatOne]
        fstp    FPTR [ebx+ecx+3*FSIZE]

        add     ecx, 4*FSIZE
        jnz     MEGA0
    }
#elif defined (__GNUC__) && defined (__i386__)
    __asm__ (
        "    shll $4, %%ecx\n"
        "    addl %%ecx, %%esi\n"
        "    addl %%ecx, %%eax\n"

        "    negl %%ecx\n"

        "    cmpl $0, %%ecx\n"
        "    je MEGA1\n"  /* ... */

        "MEGA0:\n"
        "    flds    0*"FSIZE_STR"(%%esi, %%ecx)\n"
        "    fmuls   (0*4+0)*"FSIZE_STR"(%%edi)\n"
        "    flds    1*"FSIZE_STR"(%%esi, %%ecx)\n"
        "    fmuls   (1*4+0)*"FSIZE_STR"(%%edi)\n"
        "    flds    2*"FSIZE_STR"(%%esi, %%ecx)\n"
        "    fmuls   (2*4+0)*"FSIZE_STR"(%%edi)\n"
        "    fxch    %%st(1)\n"
        "    faddp   %%st, %%st(2)\n"
        "    flds    0*"FSIZE_STR"(%%esi, %%ecx)\n"
        "    fmuls   (0*4+1)*"FSIZE_STR"(%%edi)\n"
        "    fxch    %%st(1)\n"
        "    faddp   %%st, %%st(2)\n"
        "    flds    1*"FSIZE_STR"(%%esi, %%ecx)\n"
        "    fmuls   (1*4+1)*"FSIZE_STR"(%%edi)\n"
        "    flds    2*"FSIZE_STR"(%%esi, %%ecx)\n"
        "    fmuls   (2*4+1)*"FSIZE_STR"(%%edi)\n"
        "    fxch    %%st(1)\n"
        "    faddp   %%st, %%st(2)\n"
        "    flds    0*"FSIZE_STR"(%%esi, %%ecx)\n"
        "    fmuls   (0*4+2)*"FSIZE_STR"(%%edi)\n"
        "    fxch    %%st(1)\n"
        "    faddp   %%st, %%st(2)\n"
        "    flds    1*"FSIZE_STR"(%%esi, %%ecx)\n"
        "    fmuls   (1*4+2)*"FSIZE_STR"(%%edi)\n"
        "    flds    2*"FSIZE_STR"(%%esi, %%ecx)\n"
        "    fmuls   (2*4+2)*"FSIZE_STR"(%%edi)\n"
        "    fxch    %%st(1)\n"
        "    faddp   %%st, %%st(2)\n"
        "    fxch    %%st(3)\n"
        "    faddl   (3*4+0)*"FSIZE_STR"(%%edi)\n"
        "    fxch    %%st(1)\n"
        "    faddp   %%st, %%st(3)\n"
        "    fxch    %%st(1)\n"
        "    faddl   (3*4+1)*"FSIZE_STR"(%%edi)\n"
        "    fxch    %%st(2)\n"
        "    faddl   (3*4+2)*"FSIZE_STR"(%%edi)\n"
        "    fxch    %%st(1)\n"
        "    fstps   0*"FSIZE_STR"(%%eax, %%ecx)\n"
        "    fxch    %%st(1)\n"
        "    fstps   1*"FSIZE_STR"(%%eax, %%ecx)\n"
        "    fstps   2*"FSIZE_STR"(%%eax, %%ecx)\n"

        "    flds    %4\n"
        "    fstps   3*"FSIZE_STR"(%%eax, %%ecx)\n"

        "    addl    $(4*"FSIZE_STR"), %%ecx\n"
        "    jnz     MEGA0\n"

        "MEGA1:\n"
        :
        : "c" (count), "S" (source), "a" (dest), "D" (matrix),
          "m" (floatOne) );
#endif
}

/*
 * transform normals reasonably quickly, at least quicker than
 * MS or Intel's C compilers can muster
 */
void gl_wicked_fast_normal_xform(
        GLfloat* dest, GLfloat* source, GLfloat* matrix, GLuint n)
{
#if defined (_MSC_VER)
    _asm
    {
        push esi
        push edi
        push ebx

        mov ecx,[n]
        mov SOURCE,[source]
        mov DEST,[dest]
        mov MATRIX,[matrix]

        imul ecx,ecx,3*FSIZE
        add  SOURCE,ecx
        add  DEST,ecx

        neg  ecx

    WICK0:
        fld     FPTR [SOURCE+ecx+0*FSIZE]       //s0
        fmul    FPTR [MATRIX+0*FSIZE]           //M0
        fld     FPTR [SOURCE+ecx+1*FSIZE]       //s1 M0
        fmul    FPTR [MATRIX+1*FSIZE]           //M1 M0
        fld     FPTR [SOURCE+ecx+2*FSIZE]       //s2 M1 M0
        fmul    FPTR [MATRIX+2*FSIZE]           //M2 M1 M0
        fxch    st(1)                           //M1 M2 M0
        faddp   st(2),st                        //M2 M1+M0

        fld     FPTR [SOURCE+ecx+0*FSIZE]       //s0 M2 M1+M0
        fmul    FPTR [MATRIX+4*FSIZE]           //M4 M2 M1+M0
        fxch    st(1)                           //M2 M4 M1+M0
        faddp   st(2),st                        //M4 R0

        fld     FPTR [SOURCE+ecx+1*FSIZE]       //s1 M4 R0
        fmul    FPTR [MATRIX+5*FSIZE]           //M5 M4 R0
        fld     FPTR [SOURCE+ecx+2*FSIZE]       //s2 M5 M4 R0
        fmul    FPTR [MATRIX+6*FSIZE]           //M6 M5 M4 R0
        fxch    st(1)                           //M5 M6 M4 R0
        faddp   st(1),st                        //M5+M6 M4 R0

        fld     FPTR [SOURCE+ecx+0*FSIZE]       //s0 M5+M6 M4 R0
        fmul    FPTR [MATRIX+8*FSIZE]           //M8 M5+M6 M4 R0
        fxch    st(1)                           //M5+M6 M8 M4 R0
        faddp   st(2),st                        //M8 R1 R0

        fld     FPTR [SOURCE+ecx+1*FSIZE]       //s1 M8 R1 R0
        fmul    FPTR [MATRIX+9*FSIZE]           //M9 M8 R1 R0
        fld     FPTR [SOURCE+ecx+2*FSIZE]       //s2 M9 M8 R1 R0
        fmul    FPTR [MATRIX+10*FSIZE]          //M10 M9 M8 R1 R0
        fxch    st(1)                           //M9 M10 M8 R1 R0
        faddp   st(1),st                        //M9+M10 M8 R1 R0

        fxch    st(2)                           //R1 M8 M9+M10 R0
        fstp    FPTR [DEST+ecx+1*FSIZE]         //M8 M9+M10 R0
        fxch    st(1)                           //M9+M10 M8 R0
        faddp   st(1),st                        //R2 R0
        fxch    st(1)                           //R0 R2
        fstp    FPTR [DEST+ecx+0*FSIZE]         //R2
        fstp    FPTR [DEST+ecx+2*FSIZE]

        add ecx, 3*FSIZE
        jnz WICK0

        pop ebx
        pop edi
        pop esi
    }
#elif defined (__GNUC__) && defined (__i386__)
    __asm__ (
        "    imull $(3*"FSIZE_STR"), %%ecx, %%ecx\n"
        "    addl %%ecx, "SOURCE"\n"
        "    addl %%ecx, "DEST"\n"

        "    negl %%ecx\n"

        "normxform_WICK0:\n"
        "    flds    0*"FSIZE_STR"("SOURCE",%%ecx)\n"
        "    fmuls   0*"FSIZE_STR"("MATRIX")\n"
        "    flds    1*"FSIZE_STR"("SOURCE",%%ecx)\n"
        "    fmuls   1*"FSIZE_STR"("MATRIX")\n"
        "    flds    2*"FSIZE_STR"("SOURCE",%%ecx)\n"
        "    fmuls   2*"FSIZE_STR"("MATRIX")\n"
        "    fxch    %%st(1)\n"
        "    faddp   %%st, %%st(2)\n"

        "    flds    0*"FSIZE_STR"("SOURCE",%%ecx)\n"
        "    fmuls   4*"FSIZE_STR"("MATRIX")\n"
        "    fxch    %%st(1)\n"
        "    faddp   %%st, %%st(2)\n"

        "    flds    1*"FSIZE_STR"("SOURCE",%%ecx)\n"
        "    fmuls   5*"FSIZE_STR"("MATRIX")\n"
        "    flds    2*"FSIZE_STR"("SOURCE",%%ecx)\n"
        "    fmuls   6*"FSIZE_STR"("MATRIX")\n"
        "    fxch    %%st(1)\n"
        "    faddp   %%st, %%st(1)\n"

        "    flds    0*"FSIZE_STR"("SOURCE",%%ecx)\n"
        "    fmuls   8*"FSIZE_STR"("MATRIX")\n"
        "    fxch    %%st(1)\n"
        "    faddp   %%st, %%st(2)\n"

        "    flds    1*"FSIZE_STR"("SOURCE",%%ecx)\n"
        "    fmuls   9*"FSIZE_STR"("MATRIX")\n"
        "    flds    2*"FSIZE_STR"("SOURCE",%%ecx)\n"
        "    fmuls   10*"FSIZE_STR"("MATRIX")\n"
        "    fxch    %%st(1)\n"
        "    faddp   %%st, %%st(1)\n"

        "    fxch    %%st(2)\n"
        "    fstps   1*"FSIZE_STR"("DEST",%%ecx)\n"
        "    fxch    %%st(1)\n"
        "    faddp   %%st, %%st(1)\n"
        "    fxch    %%st(1)\n"
        "    fstps   0*"FSIZE_STR"("DEST",%%ecx)\n"
        "    fstps   2*"FSIZE_STR"("DEST",%%ecx)\n"

        "    addl $(3*"FSIZE_STR"), %%ecx\n"
        "    jnz normxform_WICK0\n"
        :
        : "c" (n), "S" (source), "d" (dest), "D" (matrix) );
#endif
}

/*
 * transform and scale normals quicker than compiled C
 */
void gl_fairly_fast_scaled_normal_xform(
        GLfloat* dest, GLfloat* source, GLfloat* matrix, GLuint n, GLfloat scale)
{
#if defined (_MSC_VER)
    _asm
    {
        push esi
        push edi
        push ebx

        mov ecx,[n]
        mov SOURCE,[source]
        mov DEST,[dest]
        mov MATRIX,[matrix]

        imul ecx,ecx,3*FSIZE
        add  SOURCE,ecx
        add  DEST,ecx

        neg  ecx

    WICK0:
        fld     FPTR [SOURCE+ecx+0*FSIZE]       //s0
        fmul    FPTR [MATRIX+0*FSIZE]           //M0
        fld     FPTR [SOURCE+ecx+1*FSIZE]       //s1 M0
        fmul    FPTR [MATRIX+1*FSIZE]           //M1 M0
        fld     FPTR [SOURCE+ecx+2*FSIZE]       //s2 M1 M0
        fmul    FPTR [MATRIX+2*FSIZE]           //M2 M1 M0
        fxch    st(1)                           //M1 M2 M0
        faddp   st(2),st                        //M2 M1+M0

        fld     FPTR [SOURCE+ecx+0*FSIZE]       //s0 M2 M1+M0
        fmul    FPTR [MATRIX+4*FSIZE]           //M4 M2 M1+M0
        fxch    st(1)                           //M2 M4 M1+M0
        faddp   st(2),st                        //M4 R0

        fld     FPTR [SOURCE+ecx+1*FSIZE]       //s1 M4 R0
        fmul    FPTR [MATRIX+5*FSIZE]           //M5 M4 R0
        fld     FPTR [SOURCE+ecx+2*FSIZE]       //s2 M5 M4 R0
        fmul    FPTR [MATRIX+6*FSIZE]           //M6 M5 M4 R0
        fxch    st(1)                           //M5 M6 M4 R0
        faddp   st(1),st                        //M5+M6 M4 R0

        fld     FPTR [SOURCE+ecx+0*FSIZE]       //s0 M5+M6 M4 R0
        fmul    FPTR [MATRIX+8*FSIZE]           //M8 M5+M6 M4 R0
        fxch    st(1)                           //M5+M6 M8 M4 R0
        faddp   st(2),st                        //M8 R1 R0

        fld     FPTR [SOURCE+ecx+1*FSIZE]       //s1 M8 R1 R0
        fmul    FPTR [MATRIX+9*FSIZE]           //M9 M8 R1 R0
        fld     FPTR [SOURCE+ecx+2*FSIZE]       //s2 M9 M8 R1 R0
        fmul    FPTR [MATRIX+10*FSIZE]          //M10 M9 M8 R1 R0
        fxch    st(1)                           //M9 M10 M8 R1 R0
        faddp   st(1),st                        //M9+M10 M8 R1 R0

        fxch    st(2)                           //R1 M8 M9+M10 R0
        fmul    [scale]
        fstp    FPTR [DEST+ecx+1*FSIZE]         //M8 M9+M10 R0
        fxch    st(1)                           //M9+M10 M8 R0
        faddp   st(1),st                        //R2 R0
        fxch    st(1)                           //R0 R2
        fmul    [scale]
        fstp    FPTR [DEST+ecx+0*FSIZE]         //R2
        fmul    [scale]
        fstp    FPTR [DEST+ecx+2*FSIZE]

        add ecx, 3*FSIZE
        jnz WICK0

        pop ebx
        pop edi
        pop esi
    }
#elif defined (__GNUC__) && defined (__i386__)
    __asm__ (
        "    imull $(3*"FSIZE_STR"), %%ecx, %%ecx\n"
        "    addl  "SOURCE", %%ecx\n"
        "    addl  "DEST", %%ecx\n"

        "    negl  %%ecx\n"

        "snormxform_WICK0:\n"
        "    flds    0*"FSIZE_STR"("SOURCE",%%ecx)\n"     //s0
        "    fmuls   0*"FSIZE_STR"("MATRIX")\n"           //M0
        "    flds    1*"FSIZE_STR"("SOURCE",%%ecx)\n"     //s1 M0
        "    fmuls   1*"FSIZE_STR"("MATRIX")\n"           //M1 M0
        "    flds    2*"FSIZE_STR"("SOURCE",%%ecx)\n"     //s2 M1 M0
        "    fmuls   2*"FSIZE_STR"("MATRIX")\n"           //M2 M1 M0
        "    fxch    %%st(1)\n"                           //M1 M2 M0
        "    faddp   %%st, %%st(2)\n"                     //M2 M1+M0

        "    flds    0*"FSIZE_STR"("SOURCE",%%ecx)\n"     //s0 M2 M1+M0
        "    fmuls   4*"FSIZE_STR"("MATRIX")\n"           //M4 M2 M1+M0
        "    fxch    %%st(1)\n"                           //M2 M4 M1+M0
        "    faddp   %%st, %%st(2)\n"                     //M4 R0

        "    flds    1*"FSIZE_STR"("SOURCE",%%ecx)\n"     //s1 M4 R0
        "    fmuls   5*"FSIZE_STR"("MATRIX")\n"           //M5 M4 R0
        "    flds    2*"FSIZE_STR"("SOURCE",%%ecx)\n"     //s2 M5 M4 R0
        "    fmuls   6*"FSIZE_STR"("MATRIX")\n"           //M6 M5 M4 R0
        "    fxch    %%st(1)\n"                           //M5 M6 M4 R0
        "    faddp   %%st, %%st(1)\n"                     //M5+M6 M4 R0

        "    flds    0*"FSIZE_STR"("SOURCE",%%ecx)\n"     //s0 M5+M6 M4 R0
        "    fmuls   8*"FSIZE_STR"("MATRIX")\n"           //M8 M5+M6 M4 R0
        "    fxch    %%st(1)\n"                           //M5+M6 M8 M4 R0
        "    faddp   %%st, %%st(2)\n"                     //M8 R1 R0

        "    flds    1*"FSIZE_STR"("SOURCE",%%ecx)\n"     //s1 M8 R1 R0
        "    fmuls   9*"FSIZE_STR"("MATRIX")\n"           //M9 M8 R1 R0
        "    flds    2*"FSIZE_STR"("SOURCE",%%ecx)\n"     //s2 M9 M8 R1 R0
        "    fmuls   10*"FSIZE_STR"("MATRIX")\n"          //M10 M9 M8 R1 R0
        "    fxch    %%st(1)\n"                           //M9 M10 M8 R1 R0
        "    faddp   %%st, %%st(1)\n"                     //M9+M10 M8 R1 R0

        "    fxch    %%st(2)\n"                           //R1 M8 M9+M10 R0
        "    fmuls   %4\n"
        "    fstps   1*"FSIZE_STR"("DEST",%%ecx)\n"       //M8 M9+M10 R0
        "    fxch    %%st(1)\n"                           //M9+M10 M8 R0
        "    faddp   %%st, %%st(1)\n"                     //R2 R0
        "    fxch    %%st(1)\n"                           //R0 R2
        "    fmuls   %4\n"
        "    fstps   0*"FSIZE_STR"("DEST",%%ecx)\n"       //R2
        "    fmuls   %4\n"
        "    fstps   2*"FSIZE_STR"("DEST",%%ecx)\n"

        "    addl $(3*"FSIZE_STR"), %%ecx\n"
        "    jnz snormxform_WICK0\n"
        :
        : "c" (n), "S" (source), "d" (dest), "D" (matrix), "m" (scale) );
#endif
}

void ablend_565(unsigned char *lpAlpha, unsigned int iAlpPitch,
                unsigned char *lpSrc,   unsigned int iSrcX, unsigned int iSrcY,
                unsigned int iSrcPitch, unsigned char *lpDst,
                unsigned int iDstX,     unsigned int iDstY,
                unsigned int iDstW,     unsigned int iDstH,
                unsigned int iDstPitch);
void ablend_555(unsigned char *lpAlpha, unsigned int iAlpPitch,
                unsigned char *lpSrc,   unsigned int iSrcX, unsigned int iSrcY,
                unsigned int iSrcPitch, unsigned char *lpDst,
                unsigned int iDstX,     unsigned int iDstY,
                unsigned int iDstW,     unsigned int iDstH,
                unsigned int iDstPitch);

void gl_mmx_blend_span(
    GLcontext* ctx, GLuint n, GLubyte* mask, GLubyte rgba[][4], GLushort* dest)
{
    GLushort  srcbuf[MAX_WIDTH+4];
    GLubyte   alphabuf[MAX_WIDTH+8];
    GLushort* src;
    GLubyte*  alpha;
    GLuint    i;

    src = srcbuf;
    while ((GLint)src & 7) src++;
    alpha = alphabuf;
    while ((GLint)alpha & 7) alpha++;

    if (ctx->Buffer.PixelType == GL_RGB565)
    {
        for (i = 0; i < n; i++)
        {
            if (mask[i])
            {
                src[i] = FORM_RGB565(rgba[i][0], rgba[i][1], rgba[i][2]);
                alpha[i] = rgba[i][3];
            }
            else
            {
                alpha[i] = 0;
            }
        }

        ablend_565(alpha, n,
                   (GLubyte*)src, 0, 0, 2*n,
                   (GLubyte*)dest, 0, 0, n, 1, 2*n);
    }
    else
    {
        for (i = 0; i < n; i++)
        {
            if (mask[i])
            {
                src[i] = FORM_RGB555(rgba[i][0], rgba[i][1], rgba[i][2]);
                alpha[i] = rgba[i][3];
            }
            else
            {
                alpha[i] = 0;
            }
        }

        ablend_555(alpha, n,
                   (GLubyte*)src, 0, 0, 2*n,
                   (GLubyte*)dest, 0, 0, n, 1, 2*n);
    }
}
