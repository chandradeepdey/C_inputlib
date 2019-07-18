#ifndef INPUTLIB_H_
#define INPUTLIB_H_

char* get_dynstring(FILE *stream);
int get_int(FILE *stream);
long get_long(FILE *stream);
double get_double(FILE *stream);
unsigned int get_unsigned(FILE *stream);

#endif
