//
// Copyright: 2019-20 Chandradeep Dey
// License: GNU LGPL 3.0
//
#ifndef C_INPUTLIB_INPUTLIB_H
#define C_INPUTLIB_INPUTLIB_H

#include <stddef.h>
#include <stdio.h>

#if (_POSIX_C_SOURCE < 200809L && !_GNU_SOURCE)
size_t getline(char **restrict lineptr, size_t *restrict n, FILE *restrict stream);

size_t getdelim(char **restrict lineptr, size_t *restrict n, int delim, FILE *restrict stream);
#endif

short fgetshrt(FILE *stream);

unsigned short fgetushrt(FILE *stream);

int fgeti(FILE *stream);

unsigned int fgetu(FILE *stream);

long fgetl(FILE *stream);

unsigned long fgetul(FILE *stream);

long long fgetll(FILE *stream);

unsigned long long fgetull(FILE *stream);

float fgetf(FILE *stream);

double fgetd(FILE *stream);

long double fgetld(FILE *stream);

#endif //C_INPUTLIB_INPUTLIB_H
