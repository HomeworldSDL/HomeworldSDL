#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "fqcodec.h"
#include "fqeffect.h"

#define rmin(x,y) ((x>y)?(y):(x))

#define FQ_TIME_DELAY 0
#define FQ_TIME_ACMODEL 1

static unsigned int aEQBlock[FQ_EQNUM] = { 0, 1, 2, 4, 8, 16, 32, 64, 128, 256 };
static unsigned int nBSize = 256;

double (*pSqrtF)(double) = sqrt;
int (*pRandF)(int) = rrand;
int nRandP;

static float gauss = 0.0;
static char gotgauss = 0;

// Generate gaussian random numbers based on Box Muller method
// Two random numbers are generated and one is cached for the next call
float gaussian() {
	float x, y, r, d;

	if (gotgauss) {
		gotgauss = 0;
		return gauss;
	} 

	do {
		x = ((float)(pRandF(nRandP) & 0x7fff) / 32767.0 - 0.5) * 2.0;
		y = ((float)(pRandF(nRandP) & 0x7fff) / 32767.0 - 0.5) * 2.0;
		r = x * x + y * y;
	} while (r >= 1.0 || r == 0.0);
	
	d = pSqrtF(-2.0 * log(r) / r);
	gauss = x * d;
	gotgauss = 1;
	
	return y * d;
}

int rrand(int nDummy) {
	return nDummy ? rand() : rand();
}

int fqRand(int (*pFunc)(int), int nParam) {
	pRandF = pFunc;
	nRandP = nParam;

	return OK;
}

double fqSqrt(double (*pFunc)(double)) {
	pSqrtF = pFunc;
	
	return OK;
}

int fqSize(udword nSize) {
	nBSize = nSize;
	if (nSize > 256) nBSize = 256;
	
	return OK;
}

int fqAdd(float *aPBlock, float *aSBlock) {
	udword i;

	for (i = 0; i < nBSize; i++) {
		aPBlock[i] += aSBlock[i];
	}

	return OK;
}

int fqScale(float *aBlock, float fLev) {
	udword i;

	if (fLev == 0.0) {
		memset(aBlock, 0, 4 * nBSize);
	}
	else if (fLev != 1.0) {
		for (i = 0; i < nBSize; i++) {
			aBlock[i] *= fLev;
		}
	}

	return OK;
}

int fqMix(float *aPBlock, float *aSBlock, float fLev) {
	udword i;

	if (fLev > 0.0) {
		for (i = 0; i < nBSize; i++) {
			aPBlock[i] += aSBlock[i] * fLev;
		}
	}

	return OK;
}

int fqPitchShift(float *aBlock, float fShift) {
	udword i, ipos;
	float pos, buf[FQ_DSIZE];

	if (fShift != 1.0) {
		for (i = 0; i < nBSize; i++) {
			pos = (float)i * fShift;
			if (pos < (float)nBSize) {
				ipos = (udword)floor(pos);
				if (ipos > nBSize - 2)
					ipos = nBSize - 2;
				buf[i] = (aBlock[ipos + 1] - aBlock[ipos]) * (pos - (float)ipos) + aBlock[ipos];
			} else {
				buf[i] = 0.0;
			}
		}

		memcpy(aBlock, buf, nBSize * 4);
	}

	return OK;
}

int fqEqualize(float *aBlock, float *aEq) {
	udword i, j, jEnd;

	for (i = 0; i < FQ_EQNUM - 1; i++) {

		if (nBSize - 1 < aEQBlock[i])
			break;

		jEnd = aEQBlock[i + 1];

		if (nBSize < jEnd)
			jEnd = nBSize;

		if (aEq[i] == 0.0) {
			memset(&aBlock[aEQBlock[i]], 0, (aEQBlock[i + 1] - aEQBlock[i]) << 2);
		} else if (aEq[i] != 1.0) {
			for (j = aEQBlock[i]; j < jEnd; j++) {
				aBlock[j] *= aEq[i];
			}
		}
	}

	return OK;
}

static int fqIncEffect(float *aBlock, float *fVal, udword nDur, float *aBuf, sdword nSize, sdword *nPos, char aEffect) {
	udword s;
	sdword t;

	if (aBlock == 0) {
		if (nSize < FQ_SIZE) {
			return ERR;
		} else {
			memset(aBuf, 0, nSize << 2);
			*nPos = 0;
			return OK;
		}
	}

	if (nDur > 0) {
		if (*nPos >= nSize)
			*nPos = 0;
		s = (udword)floor(((float)nSize / (float)FQ_SIZE) * FQ_SLICE);
		t = *nPos - ((udword)floor((float)rmin(nDur, s) / FQ_SLICE) * FQ_SIZE);
		if (t < 0)
			t += nSize;
		fqAdd(aBlock, &aBuf[t]);
		memcpy(&aBuf[*nPos], aBlock, nBSize << 2);
		switch (aEffect) {
			case FQ_TIME_DELAY: fqScale(&aBuf[*nPos], *fVal); break;
			case FQ_TIME_ACMODEL: fqEqualize(&aBuf[*nPos], fVal); break;
		}
		*nPos += FQ_SIZE;
	}

	return OK;
}

int fqDelay(float *aBlock, float fLev, udword nDur, float *aBuf, sdword nSize, sdword *nPos) {
	return fqIncEffect(aBlock, &fLev, nDur, aBuf, nSize, nPos, FQ_TIME_DELAY);
}

int fqAcModel(float *aBlock, float *aEq, udword nDur, float *aBuf, sdword nSize, sdword *nPos) {
	return fqIncEffect(aBlock, aEq, nDur, aBuf, nSize, nPos, FQ_TIME_ACMODEL);
}

int fqInitE (EFFECT *rEffect) {
	rEffect->nClockCount = 0;
	rEffect->nFiltMinFreq = 0;
	rEffect->nFiltMaxFreq = 255;
	rEffect->nToneMinFreq = 0;
	rEffect->nToneMaxFreq = 0;
	rEffect->nToneDur = 0;
	rEffect->nToneMute = 0;
	rEffect->nToneCount = 0;
	rEffect->nBreakMaxRate = 0;
	rEffect->nBreakMaxDur = 0;
	rEffect->nQNoiseMaxRate = 0;
	rEffect->nQNoiseMaxDur = 0;
	rEffect->fScaleLev = 1.0;
	rEffect->fNoiseLev = 0.0;
	rEffect->fToneLev = 0.0;
	rEffect->fLimitLev = 1.0;
	rEffect->fPitchShift = 1.0;
	rEffect->pEQLev = 0;

	return OK;
}

int fqFilterE(float *aBlock, EFFECT *rEffect) {
	udword nFiltMinFreq = rEffect->nFiltMinFreq;
	udword nFiltMaxFreq = rEffect->nFiltMaxFreq;

	if (nFiltMinFreq > nBSize) nFiltMinFreq = nBSize;
	if (nFiltMaxFreq > nBSize) nFiltMaxFreq = nBSize;

	memset(aBlock, 0, 4 * nFiltMinFreq);
	memset((char*)aBlock + 4 * nFiltMaxFreq, 0, 4 * (nBSize - nFiltMaxFreq));

	return OK;
}

int fqAddNoiseE(float *aBlock, EFFECT *rEffect) {
	udword i;
	udword nFiltMinFreq = rEffect->nFiltMinFreq;
	udword nFiltMaxFreq = rEffect->nFiltMaxFreq;

	if (nFiltMinFreq > nBSize) nFiltMinFreq = nBSize;
	if (nFiltMaxFreq > nBSize) nFiltMaxFreq = nBSize;

	if (rEffect->fNoiseLev != 0.0) {
		for(i = nFiltMinFreq; i < nFiltMaxFreq; i++) {
			aBlock[i] += gaussian() * rEffect->fNoiseLev;
		}
	}

	return OK;
}

int fqAddToneE(float *aBlock, EFFECT *rEffect) {
	if (rEffect->nToneDur > 0 && rEffect->nToneCount > 0 && rEffect->fToneLev > 0.0) {
		printf("unimplemented: fqAddToneE\n");
	}

	return OK;
}

int fqAddBreakE(float *aBlock, EFFECT *rEffect) {
	if (rEffect->nBreakMaxRate > 0 && rEffect->nBreakMaxDur > 0) {
		printf("unimplemented: fqAddBreakE\n");
	}

	return OK;
}

int fqGenQNoiseE(char *aQBlock, udword nRate, EFFECT *rEffect) {
	if (rEffect->nQNoiseMaxRate > 0 && rEffect->nQNoiseMaxDur > 0) {
		printf("unimplemented: fqGenQNoiseE\n");
	}

	return OK;
}

int fqLimitE(float *aBlock, EFFECT *rEffect) {
	udword i;

	float fLimitLev = rEffect->fLimitLev;
	udword nFiltMinFreq = rEffect->nFiltMinFreq;
	udword nFiltMaxFreq = rEffect->nFiltMaxFreq;

	if (nFiltMinFreq > nBSize) nFiltMinFreq = nBSize;
	if (nFiltMaxFreq > nBSize) nFiltMaxFreq = nBSize;

	if (fLimitLev != 1.0f) {
		if (fLimitLev == 0.0f) {
			memset(&aBlock[nFiltMinFreq], 0, (nFiltMaxFreq - nFiltMinFreq) * 4);
		} else {

			float oneOver3fLimitLev = 1.0f / (fLimitLev * 3.0f);
			fLimitLev = fLimitLev * 32767.0f;

			for (i = nFiltMinFreq; i < nFiltMaxFreq; i++) {
				if ((aBlock[i] > 0.0) && (aBlock[i] > fLimitLev))
					aBlock[i] = fLimitLev;
				if ((aBlock[i] < 0.0) && (aBlock[i] < -fLimitLev))
					aBlock[i] = -fLimitLev;
			}

			if (oneOver3fLimitLev > 1.0)
				for (i = nFiltMinFreq; i < nFiltMaxFreq; i++)
					if (aBlock[i] != 0.0)
						aBlock[i] *= oneOver3fLimitLev;
		}
	}

	return OK;
}
