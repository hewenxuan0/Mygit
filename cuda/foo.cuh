#ifndef FOO_CUH
#define FOO_CUH


#include <stdio.h>
#include "/data/GPU_virtualization/Client.h"

extern Client client[100];

extern "C" 
void useCUDA(int *a,int *b,int *c,int width,int num);


#endif
