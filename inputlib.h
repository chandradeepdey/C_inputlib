#ifndef INPUTLIB_H_
#define INPUTLIB_H_

char* get_string(char *restrict s, int n, FILE *restrict stream);
void clear_line(FILE *stream);
int get_int(FILE *stream);
int get_long(FILE *stream);
double get_double(FILE *stream);

#endif
