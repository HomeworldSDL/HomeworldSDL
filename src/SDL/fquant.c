#include <stdio.h>
#include <string.h>
#include "fqcodec.h"
#include "fquant.h"

static float gSDBlock[FQ_EXP];
static unsigned short gCBBlock[FQ_CBNUM] = { 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0009, 0x000b, 0x000d, 0x000f, 0x0011, 0x0014, 0x0017, 0x001b, 0x001f, 0x0025, 0x002b, 0x0033, 0x003e, 0x004a, 0x0059, 0x006e, 0x008b, 0x00b4, 0x0100 };

int fqInitDequant(void) {
	unsigned int i;

	gSDBlock[0] = FQ_SCALE;
	for (i = 1; i < FQ_EXP; i++)
		gSDBlock[i] = gSDBlock[i - 1] * FQ_SCALE;
	for (i = 0; i < FQ_EXP; i++)
    gSDBlock[i] = FQ_FMASK / (double) ((1 << (i + 1)) - 1) * gSDBlock[i];

	return OK;
}

sdword fqSUnpack(udword bits, udword offset, char *buf) {
    int val; // edx

    val = (*(int *) &buf[offset >> 3]
            & (uint32_t) (((1 << bits) - 1) << (offset - 8 * (offset >> 3))))
            >> (offset - 8 * (offset >> 3));
    if ((uint32_t) val >> (bits - 1) == 1)
        val = -((uint32_t) (val << (32 - bits + 1)) >> (32 - bits + 1));
    return val;
}

udword fqUnpack(udword bits, udword offset, char *buf) {
    return (*(int *) &buf[offset >> 3]
            & (uint32_t) (((1 << bits) - 1) << (offset - 8 * (offset >> 3))))
            >> (offset - 8 * (offset >> 3));
}

int fqDequantBlock(char *aQBlock, float *aFPBlock, float *aFSBlock, unsigned char *aEBlock, udword nLen, udword nRate, udword nSize) {
	udword pos, count, ua, ub, uc, ud, u, i;
	sdword j, s;

	memset(aFPBlock, 0, FQ_SIZE * sizeof(float));
	memset(aFSBlock, 0, FQ_SIZE * sizeof(float));

	pos = 0;
	ua = fqUnpack(2, pos, aQBlock);
	pos += 2;
	ub = fqUnpack(3, pos, aQBlock);
	pos += 3;
	uc = fqUnpack(2, pos, aQBlock);
	pos += 2;
	ud = fqUnpack(4, pos, aQBlock);
	pos += 4;

	if ((ua & 1) == 1)
		memset(aEBlock, 0, FQ_SIZE);

	if (ub > 0 && uc > 0) {
		count = 0;
		for (i = 0; i < FQ_CBNUM - 1 && pos < nRate && pos < nLen << 3; i++) {
			u = fqUnpack(ub, pos, aQBlock);
			pos += ub;
			if (i > 0 && u == 0)
				break;
			count += u;
			u = fqUnpack(uc, pos, aQBlock);
			pos += uc;
			for (j = gCBBlock[count]; j < gCBBlock[count + 1]; j++) {
				aEBlock[j] = u;
			}
		}
	}

	if (nSize > 0 && ud > 0) {
		count = 0;
		for (i = 0; i < FQ_SIZE && pos < nRate && pos < nLen << 3; i++) {
			u = fqUnpack(ud, pos, aQBlock);
			pos += ud;
			if (i > 0 && u == 0)
				break;
			count += u;
			s = fqSUnpack(aEBlock[count] + 2, pos, aQBlock);
			pos += aEBlock[count] + 2;
			if (s != 0 && count < nSize && aEBlock[count] < 6)
				aFPBlock[count] = gSDBlock[aEBlock[count]] * s;
		}
		if ((ua & 2) == 2) {
			memcpy(aFSBlock, aFPBlock, FQ_SIZE * sizeof(float));
		} else {
			count = 0;
			for (i = 0; i < FQ_SIZE && pos < nRate && pos < nLen << 3; i++) {
				u = fqUnpack(ud, pos, aQBlock);
				pos += ud;
				if (i > 0 && u == 0)
					break;
				count += u;
				s = fqSUnpack(aEBlock[count] + 2, pos, aQBlock);
				pos += aEBlock[count] + 2;
				if (s != 0 && count < nSize && aEBlock[count] < 6)
					aFSBlock[count] = gSDBlock[aEBlock[count]] * s;
			}
		}
	}

	return OK;
}
