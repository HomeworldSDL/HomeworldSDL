/*=============================================================================
    Name    : transformer-asm.c
    Purpose : ASM functions that the compiler should not inline in
              Transformer.c (due to line labels in inline assembly).
=============================================================================*/

#include "glcaps.h"
#include "Transformer.h"
#include "Memory.h"
#include "main.h"

#define FPTR dword ptr
#define FSIZE 4
#define FSIZE_STR "4"

#if defined (_MSC_VER)
#define S(x) [esi + FSIZE*x]
#define D(x) [edi + FSIZE*x]
#define M(x) [edx + FSIZE*x]
#elif defined (__GNUC__) && defined (__i386__)
#define S(x) FSIZE_STR "*" #x "(%%esi)"
#define D(x) FSIZE_STR "*" #x "(%%edi)"
#define M(x) FSIZE_STR "*" #x "(%%edx)"
#endif

//x87 3D transform
void transTransformVertexList_asm(sdword n, hvector* dest, vertexentry* source, hmatrix* m)
{
#if defined (_MSC_VER)
    _asm
    {
        mov ecx, n
        mov esi, [source]
        mov ebx, [dest]
        mov edi, [m]
        shl ecx, 4
        add esi, ecx
        add ebx, ecx

        neg ecx

        cmp ecx, 0
        jne MEGA0
        ret

        align 16

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

        add     ecx, 4*FSIZE
        jnz     MEGA0
    }
#elif defined (__GNUC__) && defined (__i386__)
    __asm__ (
        "    shll $4, %%ecx\n"
        "    addl %%ecx, %%esi\n"
        "    addl %%ecx, %%ebx\n"

        "    negl %%ecx\n"

        "    cmpl $0, %%ecx\n"
        "    je MEGA1\n"

        ".align 16\n"

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
        "    fstps   0*"FSIZE_STR"(%%ebx, %%ecx)\n"
        "    fxch    %%st(1)\n"
        "    fstps   1*"FSIZE_STR"(%%ebx, %%ecx)\n"
        "    fstps   2*"FSIZE_STR"(%%ebx, %%ecx)\n"

        "    addl    $(4*"FSIZE_STR"), %%ecx\n"
        "    jnz     MEGA0\n"

        "MEGA1:\n"
        :
        : "c" (n), "S" (source), "b" (dest), "D" (m) );
#endif
}

//x87 perspective transform
void transPerspectiveTransform_asm(sdword n, hvector* dest, hvector* source, hmatrix* m)
{
#if defined (_MSC_VER)
    _asm
    {
        push esi
        push edi

        mov ecx, [n]
        mov esi, [source]
        mov edi, [dest]
        mov edx, [m]

        test ecx, ecx
        jz two

        align 16

      one:
        fld FPTR S(0)
        fmul FPTR M(0)
        fld FPTR S(1)
        fmul FPTR M(5)
        fld FPTR S(2)
        fmul FPTR M(10)

        fld FPTR S(2)
        fmul FPTR M(8)
        fld FPTR S(2)
        fmul FPTR M(9)
        fld FPTR M(14)

        mov eax, FPTR S(2)
        lea esi, FPTR S(4)
        xor eax, 0x80000000
        dec ecx

        faddp st(3),st
        faddp st(3),st
        faddp st(3),st

        fstp FPTR D(2)
        fstp FPTR D(1)
        fstp FPTR D(0)

        mov FPTR D(3), eax
        lea edi, FPTR D(4)
        jnz one

      two:
        pop edi
        pop esi
    }
#elif defined (__GNUC__) && defined (__i386__)
    __asm__ (
        "    testl %%ecx, %%ecx\n"
        "    jz perspxform_two\n"

        "    .align 16\n"

        "perspxform_one:\n"
        "    flds "S(0)"\n"
        "    fmuls "M(0)"\n"
        "    flds "S(1)"\n"
        "    fmuls "M(5)"\n"
        "    flds "S(2)"\n"
        "    fmuls "M(10)"\n"

        "    flds "S(2)"\n"
        "    fmuls "M(8)"\n"
        "    flds "S(2)"\n"
        "    fmuls "M(9)"\n"
        "    flds "M(14)"\n"

        "    movl "S(2)", %%eax\n"
        "    leal "S(4)", %%esi\n"
        "    xorl $0x80000000, %%eax\n"
        "    decl %%ecx\n"

        "    faddp %%st, %%st(3)\n"
        "    faddp %%st, %%st(3)\n"
        "    faddp %%st, %%st(3)\n"

        "    fstps "D(2)"\n"
        "    fstps "D(1)"\n"
        "    fstps "D(0)"\n"

        "    movl %%eax, "D(3)"\n"
        "    leal "D(4)", %%edi\n"
        "    jnz perspxform_one\n"

        "perspxform_two:\n"
        :
        : "c" (n), "S" (source), "D" (dest), "d" (m)
        : "eax" );
#endif
}
