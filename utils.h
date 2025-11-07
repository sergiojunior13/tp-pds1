#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdlib.h>

typedef struct Hp {
    int crr;
    int max;
} Hp;

void must_init(_Bool test, const char* description);

int NumToDigits(int n);

void ClearKeyboardKeys(unsigned char* keyboard_keys);

int GenRandomNum(int start, int end);

void ShuffleArray(int* array, int size);

#endif