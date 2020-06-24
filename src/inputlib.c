//
// Copyright: 2019-20 Chandradeep Dey
// License: GNU LGPL 3.0
//
#include "inputlib.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#if (_POSIX_C_SOURCE < 200809L && !_GNU_SOURCE)
#include <stdint.h>
#endif

#include <string.h>
#include <errno.h>
#include <limits.h>

#if (_POSIX_C_SOURCE < 200809L && !_GNU_SOURCE)
size_t getline(char **restrict lineptr, size_t *restrict n, FILE *restrict stream) {
    return getdelim(lineptr, n, '\n', stream);
}

size_t getdelim(char **restrict lineptr, size_t *restrict n, int delim, FILE *restrict stream) {
    if (lineptr == NULL || n == NULL || stream == NULL) { return 0; }

    /* do this instead of (feof(stream) || ferror(stream))
     * because it is possible that eof or error indicator has
     * not yet been set, but the next read attempt would set it
     */
    int ch = getc(stream);
    if (ch == EOF) { return 0; }

    if (*lineptr == NULL) {
        *lineptr = malloc(1);
        if (*lineptr) {
            *n = 1;
        } else {
            ungetc(ch, stream);
            return 0;
        }
    }

    size_t i = 0;
    while (ch != delim && ch != EOF) {
        if (i == *n - 1) {
            if (*n <= SIZE_MAX / 2) {
                char *temp = realloc(*lineptr, *n * 2);
                if (temp != NULL) {
                    *n *= 2;
                    *lineptr = temp;
                } else {
                    break;
                }
            } else {
                break;
            }
        }
        (*lineptr)[i++] = ch;
        ch = getc(stream);
    }
    (*lineptr)[i++] = '\0';

    /* if ch is not the newline character, consume input until
     * newline occurs
     */
    while (ch != delim && ch != EOF) { ch = getc(stream); }
    return i;
}
#endif

short fgetshrt(FILE *stream) {
    long ret = 0;

    char *input = NULL;
    size_t n = 0;
    while (getline(&input, &n, stream) != 0) {
        char *endptr = NULL;
        errno = 0;
        ret = strtol(input, &endptr, 0);
        if (input == endptr) {
            fputs("Invalid input\n", stderr);
        } else if (errno == ERANGE || ret > SHRT_MAX || ret < SHRT_MIN) {
            // manually do this because strtol won't set it unless input was > LONG_MAX or < LONG_MIN
            fprintf(stderr, "%s\n", strerror(ERANGE));
        } else {
            break;
        }
    }
    free(input);

    return (short) ret;
}

unsigned short fgetushrt(FILE *stream) {
    unsigned long ret = 0;

    char *input = NULL;
    size_t n = 0;
    while (getline(&input, &n, stream) != 0) {
        char *checksign = strchr(input, '-');
        char *endptr = NULL;
        errno = 0;
        ret = strtoul(input, &endptr, 0);
        if (input == endptr || (checksign != NULL && checksign < endptr)) {
            fputs("Invalid input\n", stderr);
        } else if (errno == ERANGE || ret > USHRT_MAX) {
            // manually do this because strtoul might not set errno unless input was > ULONG_MAX
            fprintf(stderr, "%s\n", strerror(ERANGE));
        } else {
            break;
        }
    }
    free(input);

    return (unsigned short) ret;
}

int fgeti(FILE *stream) {
    long ret = 0;

    char *input = NULL;
    size_t n = 0;
    while (getline(&input, &n, stream) != 0) {
        char *endptr = NULL;
        errno = 0;
        ret = strtol(input, &endptr, 0);
        if (input == endptr) {
            fputs("Invalid input\n", stderr);
        } else if (errno == ERANGE || ret > INT_MAX || ret < INT_MIN) {
            // manually do this because strtol won't set it unless input was > LONG_MAX or < LONG_MIN
            fprintf(stderr, "%s\n", strerror(ERANGE));
        } else {
            break;
        }
    }
    free(input);

    return (int) ret;
}

unsigned int fgetu(FILE *stream) {
    unsigned long ret = 0;

    char *input = NULL;
    size_t n = 0;
    while (getline(&input, &n, stream) != 0) {
        char *checksign = strchr(input, '-');
        char *endptr = NULL;
        errno = 0;
        ret = strtoul(input, &endptr, 0);
        if (input == endptr || (checksign != NULL && checksign < endptr)) {
            fputs("Invalid input\n", stderr);
        } else if (errno == ERANGE || ret > UINT_MAX) {
            // manually do this because strtoul might not set errno unless input was > ULONG_MAX
            fprintf(stderr, "%s\n", strerror(ERANGE));
        } else {
            break;
        }
    }
    free(input);

    return (unsigned int) ret;
}

long fgetl(FILE *stream) {
    long ret = 0;

    char *input = NULL;
    size_t n = 0;
    while (getline(&input, &n, stream) != 0) {
        char *endptr = NULL;
        errno = 0;
        ret = strtol(input, &endptr, 0);
        if (input == endptr) {
            fputs("Invalid input\n", stderr);
        } else if (errno == ERANGE) {
            fprintf(stderr, "%s\n", strerror(errno));
        } else {
            break;
        }
    }
    free(input);

    return ret;
}

unsigned long fgetul(FILE *stream) {
    unsigned long ret = 0;

    char *input = NULL;
    size_t n = 0;
    while (getline(&input, &n, stream) != 0) {
        char *checksign = strchr(input, '-');
        char *endptr = NULL;
        errno = 0;
        ret = strtoul(input, &endptr, 0);
        if (input == endptr || (checksign != NULL && checksign < endptr)) {
            fputs("Invalid input\n", stderr);
        } else if (errno == ERANGE) {
            fprintf(stderr, "%s\n", strerror(errno));
        } else {
            break;
        }
    }
    free(input);

    return ret;
}

long long fgetll(FILE *stream) {
    long long ret = 0;

    char *input = NULL;
    size_t n = 0;
    while (getline(&input, &n, stream) != 0) {
        char *endptr = NULL;
        errno = 0;
        ret = strtoll(input, &endptr, 0);
        if (input == endptr) {
            fputs("Invalid input\n", stderr);
        } else if (errno == ERANGE) {
            fprintf(stderr, "%s\n", strerror(errno));
        } else {
            break;
        }
    }
    free(input);

    return ret;
}

unsigned long long fgetull(FILE *stream) {
    unsigned long long ret = 0;

    char *input = NULL;
    size_t n = 0;
    while (getline(&input, &n, stream) != 0) {
        char *checksign = strchr(input, '-');
        char *endptr = NULL;
        errno = 0;
        ret = strtoull(input, &endptr, 0);
        if (input == endptr || (checksign != NULL && checksign < endptr)) {
            fputs("Invalid input\n", stderr);
        } else if (errno == ERANGE) {
            fprintf(stderr, "%s\n", strerror(errno));
        } else {
            break;
        }
    }
    free(input);

    return ret;
}

float fgetf(FILE *stream) {
    float ret = 0;

    char *input = NULL;
    size_t n = 0;
    while (getline(&input, &n, stream)) {
        char *endptr = NULL;
        errno = 0;
        ret = strtof(input, &endptr);
        if (input == endptr) {
            fputs("Invalid input\n", stderr);
        } else if (errno == ERANGE) {
            fprintf(stderr, "%s\n", strerror(errno));
        } else {
            break;
        }
    }
    free(input);

    return ret;
}

double fgetd(FILE *stream) {
    double ret = 0;

    char *input = NULL;
    size_t n = 0;
    while (getline(&input, &n, stream)) {
        char *endptr = NULL;
        errno = 0;
        ret = strtod(input, &endptr);
        if (input == endptr) {
            fputs("Invalid input\n", stderr);
        } else if (errno == ERANGE) {
            fprintf(stderr, "%s\n", strerror(errno));
        } else {
            break;
        }
    }
    free(input);

    return ret;
}

long double fgetld(FILE *stream) {
    long double ret = 0;

    char *input = NULL;
    size_t n = 0;
    while (getline(&input, &n, stream)) {
        char *endptr = NULL;
        errno = 0;
        ret = strtold(input, &endptr);
        if (input == endptr) {
            fputs("Invalid input\n", stderr);
        } else if (errno == ERANGE) {
            fprintf(stderr, "%s\n", strerror(errno));
        } else {
            break;
        }
    }
    free(input);

    return ret;
}
