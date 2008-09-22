// Copyright (c) 1998-99 Relic Entertainment Inc.
// Written by Janik Joire
//
// $History: $
//
// Version 1.6a

#ifndef ___FQEFFECT_H
#define ___FQEFFECT_H

// Constants
#define UNIFORM		((float)(pRandF(nRandP)&RAND_MAX)/RAND_MAX-0.5F)

// Freq and time constants
#define FQ_BAND			86.1328125F		// Hz
#define FQ_SLICE		11.60997732426F	// msec

// Factor constants
#define FQ_FNOISE		1000.0F
#define FQ_FTONE		1000.0F

// EQ constants
#define FQ_EQNUM		10			// # of EQ bands
#define FQ_EQTAB		aEQBlock	// EQ bands

// Process constants
#define FQ_PGEN			0
#define FQ_PADD			1

// Structures
typedef struct
{
	udword	nClockCount;	// Clock count (1 unit = approx 6 ms)

	udword	nFiltMinFreq;	// Minimum filter frequency (1 unit = approx. 86 Hz)
	udword	nFiltMaxFreq;	// Maximum filter frequency (1 unit = approx. 86 Hz)

	udword	nToneMinFreq;	// Minimum tone frequency (1 unit = approx. 86 Hz)
	udword	nToneMaxFreq;	// Maximum tone frequency (1 unit = approx. 86 Hz)
	udword	nToneDur;		// Tone duration (1 unit = approx. 6 msec)
	udword	nToneMute;		// Tone mute (1 unit = approx. 6 msec)
	udword	nToneCount;		// Tone count

	udword	nBreakMaxRate;	// Maximum break rate (1 unit = approx. 6 msec)
	udword	nBreakMaxDur;	// Maximum break duration (1 unit = approx. 6 msec)

	udword	nQNoiseMaxRate;	// Maximum q-noise rate (1 unit = approx. 6 msec)
	udword	nQNoiseMaxDur;	// Maximum q-noise duration (1 unit = approx. 6 msec)

	float			fScaleLev;		// Scale level (1.0 = 100%)
	float			fNoiseLev;		// Noise level (maximum = arbit. 1000.0)
	float			fToneLev;		// Tone level (maximum = arbit. 10000.0)
	float			fLimitLev;		// Limiter level (1.0 = 100%)

	float			fPitchShift;	// Pitch shift (0.5 = 1 octave down, 1.0 = none, 2.0 = 1 octave up)

	float			*pEQLev;		// EQ levels (1.0 = 100%)	
} EFFECT; // 72 bytes

#ifdef __cplusplus
extern "C" {
#endif

// Functions
int fqRand(int (*pFunc)(int),int nParam);
double fqSqrt(double (*pFunc)(double));
int fqSize(udword nSize);

int fqAdd(float *aPBlock,float *aSBlock);
//int fqMult(float *aPBlock,float *aSBlock);
//int fqMax(float *aPBlock,float *aSBlock);
int fqScale(float *aBlock,float fLev);
int fqMix(float *aPBlock,float *aSBlock,float fLev);
//int fqFilter(float *aBlock,udword nMinFreq,udword nMaxFreq);
//int fqAddNoise(float *aBlock,float fLev,udword nMinFreq,udword nMaxFreq);
//int fqGenNoise(float *aBlock,float fLev,udword nMinFreq,udword nMaxFreq);

int fqPitchShift(float *aBlock,float fShift);
//int fqPitchSlide(float *aBlock,sdword nSlideFreq);
//int fqStretch(float *aRBlock,float *aBlock,float *aBuf);
//int fqGate(float *aBlock,float fLev);
//int fqLimit(float *aBlock,float fLev);
int fqEqualize(float *aBlock,float *aEq);
int fqDelay(float *aBlock,float fLev,udword nDur,float *aBuf,sdword nSize,sdword *nPos);
int fqAcModel(float *aBlock,float *aEq,udword nDur,float *aBuf,sdword nSize,sdword *nPos);

int fqInitE(EFFECT *rEffect);
//int fqScaleE(float *aBlock,EFFECT *rEffect);
//int fqMixE(float *aPBlock,float *aSBlock,EFFECT *rEffect);
int fqFilterE(float *aBlock,EFFECT *rEffect);

int fqAddNoiseE(float *aBlock,EFFECT *rEffect);
int fqAddToneE(float *aBlock,EFFECT *rEffect);
int fqAddBreakE(float *aBlock,EFFECT *rEffect);

//int fqGenNoiseE(float *aBlock,EFFECT *rEffect);
//int fqGenBreakE(float *fLev,EFFECT *rEffect);
int fqGenQNoiseE(char *aQBlock,udword nRate,EFFECT *rEffect);

//int fqPitchShiftE(float *aBlock,EFFECT *rEffect);
int fqLimitE(float *aBlock,EFFECT *rEffect);
//int fqEqualizeE(float *aBlock,EFFECT *rEffect);

int rrand(int nDummy);

// Math functions
float gaussian();

#ifdef __cplusplus
}		// extern "C"
#endif

#endif
