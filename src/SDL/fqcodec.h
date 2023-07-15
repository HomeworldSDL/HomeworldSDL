// Copyright (c) 1997-99 Relic Entertainment Inc.
// Written by Janik Joire
//
// $History: $
//
// Version 1.6a

#ifndef ___FQCODEC_H
#define ___FQCODEC_H

#include "Types.h"

// General constants
#ifndef OK
#define OK		0
#endif

#ifndef ERR
#define ERR		-1
#endif

// Rate constants
#define FQ_HRATE	11025L	// Hz
#define FQ_RATE		22050L	// Hz
#define FQ_DRATE	44100L	// Hz

// Size constants
#define FQ_QSIZE	64
#define FQ_HSIZE	128
#define FQ_SIZE		256
#define FQ_DSIZE	512

// Coef constants
#define FQ_HCOEF	aCHBlock
#define FQ_COEF		aCBlock
#define FQ_DCOEF	aCDBlock

// Window constants
#define FQ_HWIN		aWHBlock
#define FQ_WIN		aWBlock
#define FQ_DWIN		aWDBlock

// Mode constants
#define FQ_MINIT	0x0000		// Initialize CODEC
#define FQ_MDOUBLE	0x0001		// Double mode
#define FQ_MNORM	0x0002		// Normal mode
#define FQ_MHALF	0x0004		// Half mode

// Macro constants
#define FQ_BNUM ((float)(1<<26)*(1<<26)*1.5)
extern double fChopT;

// Macros
#define rfabs(x)	((x<0.0F)?(-(x)):(x)) // Fast floating point absolute value
#define rmax(x,y)	((x>y)?(x):(y)) // Fast maximize
#if 0 // Clashes with math.h and apparently not used.
#define rint(x)		((fChopT = (double)(x)+FQ_BNUM), *(int*)(&fChopT)) // Fast integer cast
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Functions
//int fqEncOver(float *aTPBlock,float *aTSBlock,float *aFPBlock,float *aFSBlock,
//		float *aCBlock,float *aWBlock,udword nSize);

//int fqEncBlock(float *aTPBlock,float *aTSBlock,
//	float *aFPBlock,float *aFSBlock,int nMode);

int fqDecOver(float *aFPBlock,float *aFSBlock,float *aTPBlock,float *aTSBlock,
		float *aCBlock,float *aWBlock,udword nSize);

int fqDecBlock(float *aFPBlock,float *aFSBlock,
	float *aTPBlock,float *aTSBlock,int nMode,int nFact);

int fqWriteTBlock(float *aLBlock,float *aRBlock,short nChan,
	void *pBuf1,udword nSize1,void *pBuf2,udword nSize2);

float compress(float sample);


#ifdef __cplusplus
}		// extern "C"
#endif

#endif
