#ifndef ___LZSS_H
#define ___LZSS_H

#include <stdio.h>
#include "BitIO.h"

void lzssCompressFile(FILE *input, BitFile *output);
int  lzssCompressBuffer(char *input, int inputSize, char *output, int outputSize);
void lzssExpandFile(BitFile *input, FILE *output);
int  lzssExpandBuffer(char *input, int inputSize, char *output, int outputSize);
int  lzssExpandFileToBuffer(BitFile *input, char *output, int outputSize);

#endif
