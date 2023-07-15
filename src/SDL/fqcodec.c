#include <stdio.h>
#include <string.h>
#include <math.h>
#include "fqcodec.h"
#include "dct.h"

#define iclamp(x, a, b) (x > a ? a : (x < b ? b : x))

#ifndef PI
#define PI		3.14159265358979323846F
#endif

static float gWHBlock[FQ_HSIZE];
static float gWBlock[FQ_SIZE];
static float gWDBlock[FQ_DSIZE];

static float gCHBlock[FQ_HSIZE];
static float gCBlock[FQ_SIZE];
static float gCDBlock[FQ_DSIZE];


float compress(float sample) {
  static float limitLev = 0.5 * 32767;
  if (sample >  limitLev) return  limitLev + log(1.0 + 0.75 * ( sample - limitLev)/limitLev) * limitLev;
  if (sample < -limitLev) return -limitLev - log(1.0 + 0.75 * (-sample - limitLev)/limitLev) * limitLev;
  return sample;
}


static void fqWriteTBlockBuf(float *aLBlock, float *aRBlock, short nChan, short *pBuf, udword nSize) {
	udword i;

	if ((pBuf != 0) && (nSize > 0)) {
		for (i = 0; i < nSize; i += 2) {
			pBuf[i] = iclamp(rint(compress(aLBlock[i/2])), 32767, -32768);
			if (nChan <= 1) {
				pBuf[i + 1] = pBuf[i];
			} else {
				pBuf[i + 1] = iclamp(rint(compress(aRBlock[i/2])), 32767, -32768);
			}
		}
	}
}

int fqWriteTBlock(float *aLBlock, float *aRBlock, short nChan, void *pBuf1, udword nSize1, void *pBuf2, udword nSize2) {
	fqWriteTBlockBuf(aLBlock, aRBlock, nChan, pBuf1, nSize1);
	fqWriteTBlockBuf(aLBlock, aRBlock, nChan, pBuf2, nSize2);
	return OK;
}

int fqDecOver(float *aFPBlock, float *aFSBlock, float *aTPBlock, float *aTSBlock, float *aCBlock, float *aWBlock, udword nSize) {
	float buf[FQ_DSIZE];
	udword h = nSize >> 1, i;

	memcpy(aTPBlock, aTSBlock, nSize * sizeof(float));
	idct(aFPBlock, buf, aCBlock, nSize);
	idct(aFSBlock, aTSBlock, aCBlock, nSize);
	for(i = 0; i < h; i++) {
		aTPBlock[i + h] = (aTPBlock[i + h] * aWBlock[i + h]) + (buf[i] * aWBlock[i]);
		aTSBlock[i] = (buf[i + h] * aWBlock[i + h]) + (aTSBlock[i] * aWBlock[i]);
	}

	return OK;
}

int fqDecBlock(float *aFPBlock, float *aFSBlock, float *aTPBlock, float *aTSBlock, int nMode, int nFact) {
	float buf[FQ_DSIZE];
	unsigned int i;

	if(nMode == 0) {
		memset(aTSBlock, 0, FQ_DSIZE * sizeof(float));
		for (i = 0; i < FQ_HSIZE; i++)
			gWHBlock[i] = sin(PI / FQ_HSIZE * i);
		for (i = 0; i < FQ_SIZE; i++)
			gWBlock[i] = sin(PI / FQ_SIZE * i);
		for (i = 0; i < FQ_DSIZE; i++)
			gWDBlock[i] = sin(PI / FQ_DSIZE * i);
		Initdct(gCHBlock, FQ_HSIZE);
		Initdct(gCBlock, FQ_SIZE);
		Initdct(gCDBlock, FQ_DSIZE);
	} else {
		if(nFact == 4) {
			fqDecOver(aFPBlock, aFSBlock, aTPBlock, aTSBlock, gCHBlock, gWHBlock, FQ_HSIZE);
		} else {
			if(nFact == 2) {
				if(nMode == 4) {
					fqDecOver(aFPBlock, aFSBlock, buf, aTSBlock, gCHBlock, gWHBlock, FQ_HSIZE);
					for(i = 0; i < FQ_SIZE - 1; i += 2) {
						aTPBlock[i] = buf[i >> 1];
						aTPBlock[i + 1] = buf[i >> 1];
					}
				} else {
					fqDecOver(aFPBlock, aFSBlock, aTPBlock, aTSBlock, gCBlock, gWBlock, FQ_SIZE);
				}
			} else {
				if(nMode == 4) {
					fqDecOver(aFPBlock, aFSBlock, buf, aTSBlock, gCHBlock, gWHBlock, FQ_HSIZE);
					for(i = 0; i < FQ_DSIZE - 1; i += 4) {
						aTPBlock[i] = buf[i >> 2];
						aTPBlock[i + 1] = buf[i >> 2];
						aTPBlock[i + 2] = buf[i >> 2];
						aTPBlock[i + 3] = buf[i >> 2];
					}
				} else {
					if(nMode == 2) {
						fqDecOver(aFPBlock, aFSBlock, buf, aTSBlock, gCBlock, gWBlock, FQ_SIZE);
						for(i = 0; i < FQ_DSIZE - 1; i += 2) {
							aTPBlock[i] = buf[i >> 1];
							aTPBlock[i + 1] = buf[i >> 1];
						}
					} else {
						fqDecOver(aFPBlock, aFSBlock, aTPBlock, aTSBlock, gCDBlock, gWDBlock, FQ_DSIZE);
					}
				}
			}
		}
	}
	return OK;
}
