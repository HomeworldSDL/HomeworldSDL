#ifndef ___DCT_H
#define ___DCT_H

#ifdef __cplusplus
extern "C" {
#endif

int Initdct(float *buf, udword len);
int idct(float *a, float *b, float *c, udword len);

#ifdef __cplusplus
}		// extern "C"
#endif

#endif
