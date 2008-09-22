// Copyright (c) 1998-99 Relic Entertainment Inc.
// Written by Janik Joire
//
// $History: $
//
// Version 1.6a

#ifndef ___FQUANT_H
#define ___FQUANT_H

#include "Types.h"

// General constants
#define FQ_LEN		260			// Block length (bytes)
#define FQ_SCALE	10.0F		// Quantization scale
#define FQ_EXP		6			// # of exponents
#define FQ_FMASK	1.0F		// Frequency masking factor
#define FQ_TPMASK	2.0F		// Primary time masking factor
#define FQ_TSMASK	4.0F		// Secondary time masking factor
#define	FQ_THRES	aTBlock		// Hearing thresholds
#define FQ_STAT		pSStream	// Statistics stream

// Scale constants
#define FQ_SDATA	gSDBlock	// Scale data

// Encoder flags
#define FQ_ENORM	0x0000	// Normal
#define FQ_ERESET	0x0001	// Reset exponents
#define FQ_EMIX		0x0002	// Mix primary and secondary blocks

// Quantization limits (# of bits)
#define FQ_NBMAX	6		// Maximum
#define FQ_NBMIN	1		// Minimum

// Compression bitrates (per block)
//		Name		Bits/block	   Kbps/channel
#define	FQ_BR176	2048		// 176
#define	FQ_BR88		1024		// 88
#define	FQ_BR44		512			// 44
#define	FQ_BR22		256			// 22

// Critical band constants
#define	FQ_CBDATA	gCBBlock	// Critical band data
#define FQ_CBNUM	27			// # of critical bands

// Sub-band constants
#define FQ_SBMODE	22			// # of modes
#define FQ_SBSIZE	aSBSBlock	// Sub-band sizes
#define	FQ_SBDATA	aSBDBlock	// Sub-band data
#define FQ_SBNUM	256			// # of sub-bands

#ifdef __cplusplus
extern "C" {
#endif

// Functions
//int fqInitQuant(FILE *pStream);

//int fqQuantBlock(char *aQBlock,float *aFPBlock,float *aFSBlock,float *aFMBlock,unsigned char *aEBlock,
//				 udword nLen,udword nRate,udword nSize,int nFlags);

int fqInitDequant(void);

int fqDequantBlock(char *aQBlock,float *aFPBlock,float *aFSBlock,unsigned char *aEBlock,
				 udword nLen,udword nRate,udword nSize);

//int fqSPack(short nVal,udword nLen,udword nPos,char *aBlock);
sdword fqSUnpack(udword nLen,udword nPos,char *aBlock);
udword fqUnpack(udword nLen,udword nPos,char *aBlock);

//int linint(float *xa,float *ya,udword n,float x,float *y);

#ifdef __cplusplus
}		// extern "C"
#endif

#endif
