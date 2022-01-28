#include <stdio.h>
#include <math.h>
#include "fqcodec.h"
#include "dct.h"
#include "mixfft.h"

#ifndef PI
#define PI		3.14159265358979323846F
#endif

int Initdct(float *buf, udword len) {
	udword i;
	float f;

	for (i = 0; i < len >> 2; i++) {
		f = ((float)i + 0.125) * (PI * 2.0) * (1.0 / (float)len);
		buf[i] = sin(f);
		buf[i + (len >> 2)] = cos(f);
	}

	return OK;
}

int idct(float *spectra, float *samples, float *window, udword size) {
    float size_sqrt; // ST84_4
    float coef1; // ST8C_4
    float coef2; // ST88_4
    float fft_temp; // ST7C_4
    uint32_t size_3quarter; // [esp+5Ch] [ebp-1028h]
    uint32_t size_quarter; // [esp+60h] [ebp-1024h]
    uint32_t size_half; // [esp+64h] [ebp-1020h]
    int i;
    float factor;
    float fft_out_im[128]; // [esp+84h] [ebp-1000h]
    float fft_out_re[128]; // [esp+284h] [ebp-E00h]
    float fft_im[128]; // [esp+484h] [ebp-C00h]
    float fft_re[128]; // [esp+684h] [ebp-A00h]
    float samples_tmp[512]; // [esp+884h] [ebp-800h]

    size_half = size >> 1;
    size_quarter = size >> 2;
    size_3quarter = 3 * (size >> 2);
    factor = 8.0 / sqrt(size);

    for (i = 0; i < size_quarter; ++i) {         // prerotation?
        coef1 = spectra[2 * i] * 0.5;
        coef2 = spectra[size_half - 1 - 2 * i] * 0.5;
        fft_re[i] = coef1 * window[size_quarter + i] + coef2 * window[i];
        fft_im[i] = -coef1 * window[i] + coef2 * window[size_quarter + i];
    }

    fft(size_quarter, fft_re, fft_im, fft_out_re, fft_out_im);

    for (i = 0; i < size_quarter; i++) {    // postrotation, window and reorder?
        fft_temp = fft_out_re[i];
        fft_out_re[i] = (fft_out_re[i] * window[size_quarter + i] + fft_out_im[i] * window[i]) * factor;
        fft_out_im[i] = (-fft_temp * window[i] + fft_out_im[i] * window[size_quarter + i]) * factor;
    }
    for (i = 0; i < size_quarter; i++) {
        samples_tmp[2 * i] = fft_out_re[i];
        samples_tmp[2 * i + size_half] = fft_out_im[i];
    }
    for (i = 1; i < size; i += 2) {
        samples_tmp[i] = -samples_tmp[size - 1 - i];
    }

    for (i = 0; i < size_3quarter; i++) {
        samples[i] = samples_tmp[size_quarter + i];
    }
    for (i = size_3quarter; i < size; i++) {
        samples[i] = -samples_tmp[i - size_3quarter];
    }
    return OK;
}
