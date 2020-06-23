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
#endif

int fgeti(FILE *stream);

long fgetl(FILE *stream);

unsigned int fgetu(FILE *stream);

unsigned long long int fgetull(FILE *stream);

double fgetd(FILE *stream);

#endif //C_INPUTLIB_INPUTLIB_H
