/* C_inputlib version 2.6
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "inputlib.h"

/* gets a string from a given stream
 *
 * Parameters
 * 1) s is the char array where the string will be stored.
 * 2) n is the number of characters to be stored, including
 * the terminating null character
 * 3) stream is the stream from which input is taken. if stream
 * is NULL, stdin is used.
 *
 * Return
 * NULL pointer if n is less than 1 or s is a NULL pointer
 * Otherwise returns the same as fgets(s, n, stream)
 *
 * Any input after the string until the next newline character
 * is consumed by this function
 * Behaviour is undefined if s cannot store n elements
 */
char* get_string(char *restrict s, int n, FILE *restrict stream)
{
        if (n < 1 || s == NULL)
                return NULL;
        if (stream == NULL)
                stream = stdin;

        char *ret;
        char *newline;

        ret = fgets(s, n, stream);
        if (ret) {
                newline = strchr(s, '\n');
                if (newline)
                        /* input ended before string finished, process
                         * newline character
                         */
                        *newline = '\0';
                else
                        /* input ended after string finished, clear
                         * remaining input
                         */
                        clear_line(stream);
        }

        return ret;
}

/*
 * Any input until the next newline character is consumed by this function
 *
 * Inputs
 * 1) stream is the stream on which the operation is to be performed. stdin
 * is used if stream is NULL
 */
void clear_line(FILE *stream)
{
        if (stream == NULL)
                stream = stdin;

        int ch;
        while ((ch = getc(stream)) != '\n' && ch != EOF)
                ;
}

/*
 * buffer used by functions below this point
 */
#define STRSIZE 500
static char input[STRSIZE];

/* gets an int from a stream by persistently
 * nagging the user to enter the right thing
 *
 * Input
 * 1) stream is the stream from which input is to be taken.
 * stdin is used if stream is NULL
 *
 * Return
 * Returns an int. 0 is returned in case of a read error
 * or EOF
 *
 * Any input after the int until the next newline character
 * is consumed by this function
 */
int get_int(FILE *stream)
{
        long ret;
        char *status;
        char *endptr;

        while ((status = get_string(input, STRSIZE, stream)) != NULL) {
                ret = strtol(input, &endptr, 0);
                if (input == endptr) {
                        fputs("Invalid input\n", stderr);
                        continue;
                } else if (ret > INT_MAX || ret < INT_MIN) {
                        /* manually do this because strtol won't set it
                         * unless input was > LONG_MAX or < LONG_MIN
                         */
                        fprintf(stderr, "%s\n", strerror(ERANGE));
                        errno = 0;
                        continue;
                } else
                        break;
        }
        if (status == NULL) {
                ret = 0;
        }

        return (int) ret;
}

/* gets a long from a stream by persistently
 * nagging the user to enter the right thing
 *
 * Input
 * 1) stream is the stream from which input is to be taken.
 * stdin is used if stream is NULL
 *
 * Return
 * Returns a long. 0 is returned in case of a read error
 * or EOF
 *
 * Any input after the long until the next newline character
 * is consumed by this function
 */
long get_long(FILE *stream)
{
        long ret;
        char *status;
        char *endptr;

        while ((status = get_string(input, STRSIZE, stream)) != NULL) {
                ret = strtol(input, &endptr, 0);
                if (input == endptr) {
                        fputs("Invalid input\n", stderr);
                        continue;
                } else if (errno == ERANGE) {
                        fprintf(stderr, "%s\n", strerror(errno));
                        errno = 0;
                        continue;
                } else
                        break;
        }
        if (status == NULL) {
                ret = 0;
        }

        return ret;
}

/* gets an unsigned int from a stream by persistently
 * nagging the user to enter the right thing
 *
 * Input
 * 1) stream is the stream from which input is to be taken.
 * stdin is used if stream is NULL
 *
 * Return
 * Returns an unsigned int. 0 is returned in case of a read error
 * or EOF
 *
 * Any input after the unsigned int until the next newline character
 * is consumed by this function
 */
unsigned int get_unsigned(FILE *stream)
{
        unsigned long ret;
        char *status;
        char *endptr;

        while ((status = get_string(input, STRSIZE, stream)) != NULL) {
                ret = strtoul(input, &endptr, 0);
                if (input == endptr) {
                        fputs("Invalid input\n", stderr);
                        continue;
                } else if (ret > UINT_MAX || errno == ERANGE) {
                        /* manually do this because strtoul might not
                         * set errno unless input was > ULONG_MAX
                         */
                        fprintf(stderr, "%s\n", strerror(ERANGE));
                        errno = 0;
                        continue;
                } else
                        break;
        }
        if (status == NULL) {
                ret = 0;
        }

        return (unsigned int) ret;
}

/* gets a double from a stream by persistently
 * nagging the user to enter the right thing
 *
 * Input
 * 1) stream is the stream from which input is to be taken.
 * stdin is used if stream is NULL
 *
 * Return
 * Returns a double. 0 is returned in case of a read error
 * or EOF
 *
 * Any input after the double until the next newline character
 * is consumed by this function
 */
double get_double(FILE *stream)
{
        double ret;
        char *status;
        char *endptr;

        while ((status = get_string(input, STRSIZE, stream)) != NULL) {
                ret = strtod(input, &endptr);
                if (input == endptr) {
                        fputs("Invalid input\n", stderr);
                        continue;
                } else if (errno == ERANGE) {
                        fprintf(stderr, "%s\n", strerror(errno));
                        errno = 0;
                        continue;
                } else
                        break;
        }
        if (status == NULL) {
                ret = 0;
        }

        return ret;
}
