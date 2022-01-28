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

float gaussian() {
	float a, b, c, d;

	if (gotgauss) {
		gotgauss = 0;
		b = gauss;
	} else {
		do {
			a = ((float)(pRandF(nRandP) & 0x7fff) / 32767.0 - 0.5) * 2.0;
			b = ((float)(pRandF(nRandP) & 0x7fff) / 32767.0 - 0.5) * 2.0;
			c = a * a + b * b;
		} while (c > 1.0);
		d = pSqrtF(log(c) * -2.0 / c);
    gauss = a * d;
		b = b * d;
		gotgauss = 1;
	}

	return b;
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
	nBSize = nSize < 256 ? nSize : 256;
	return OK;
}

int fqAdd(float *aPBlock, float *aSBlock) {
	udword i;

	for (i = 0; i < nBSize; i++)
		aPBlock[i] += aSBlock[i];

	return OK;
}

int fqScale(float *aBlock, float fLev) {
	udword i;

	if (fLev == 0.0)
		memset(aBlock, 0, nBSize << 2);
	else if (fLev != 1.0)
		for (i = 0; i < nBSize; i++)
			aBlock[i] *= fLev;

	return OK;
}

int fqMix(float *aPBlock, float *aSBlock, float fLev) {
	udword i;

	if (fLev > 0.0) {
		if (fLev == 1.0)
			for (i = 0; i < nBSize; i++)
				aPBlock[i] += aSBlock[i];
		else
			for (i = 0; i < nBSize; i++)
				aPBlock[i] += aSBlock[i] * fLev;
	}

	return OK;
}

int fqPitchShift(float *aBlock, float fShift) {
	udword i, ipos;
	float pos, buf[FQ_DSIZE];

	if (fShift != 1.0) {
		memset(buf, 0, nBSize * sizeof(float));
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

    if (nBSize - 1 < aEQBlock[i]) break;
    jEnd = aEQBlock[i + 1];
    if (nBSize < jEnd) jEnd = nBSize;

    if (aEq[i] == 0.0)
			memset(&aBlock[aEQBlock[i]], 0, (aEQBlock[i + 1] - aEQBlock[i]) << 2);
		else if (aEq[i] != 1.0)
			for (j = aEQBlock[i]; j < jEnd; j++)
				aBlock[j] *= aEq[i];
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
	memset(aBlock, 0, rmin(rEffect->nFiltMinFreq, nBSize) << 2);
	memset(&aBlock[rmin(rEffect->nFiltMaxFreq, nBSize) << 2], 0, (nBSize - rmin(rEffect->nFiltMaxFreq, nBSize)) << 2);

	return OK;
}

int fqAddNoiseE(float *aBlock, EFFECT *rEffect) {
	udword i;

	if (rEffect->fNoiseLev > 0.0)
		for(i = rmin(rEffect->nFiltMinFreq, nBSize); i < rmin(rEffect->nFiltMaxFreq, nBSize); i++)
			aBlock[i] += gaussian() * rEffect->fNoiseLev;

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

	if (rEffect->fLimitLev != 1.0) {
    udword nFiltMinFreq = rmin(rEffect->nFiltMinFreq, nBSize);
    udword nFiltMaxFreq = rmin(rEffect->nFiltMaxFreq, nBSize);

		if (rEffect->fLimitLev == 0.0) {
			memset(&aBlock[nFiltMinFreq], 0, (nFiltMaxFreq - nFiltMinFreq) * 4);
		} else {
      float oneOver3fLimitLev = 1.0 / (rEffect->fLimitLev * 3.0);
      float fLimitLev = rEffect->fLimitLev * 32767.0;

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
