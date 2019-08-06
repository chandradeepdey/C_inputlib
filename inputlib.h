#ifndef INPUTLIB_H_
#define INPUTLIB_H_

char* get_dynstring(FILE *stream);
size_t cust_getline(char **lineptr, size_t *n, FILE *stream);

int get_int(FILE *stream);
long int get_long(FILE *stream);

unsigned int get_unsigned(FILE *stream);
unsigned long long int get_unsigned_long_long(FILE *stream);

double get_double(FILE *stream);

#endif
