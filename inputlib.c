/* C_inputlib version 8.4
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <limits.h>
#include "inputlib.h"

/* an implementation of the POSIX getline() function. read below
 * for differences in specs.
 *
 * Inputs
 * 1) *lineptr must point to an area allocated using the malloc()
 *    family of functions, or it must be NULL. if *lineptr is NULL,
 *    then the function starts by allocating its own memory and storing
 *    the address in *lineptr. Otherwise, the area pointed to by
 *    *lineptr is used by the function, growing it using realloc()
 *    as required.
 * 2) *n is the size of the area associated with *lineptr. The initial
 *    value inside *n doesn't matter if *lineptr is set to NULL.
 * 3) stream should be an input stream or an update stream or a NULL
 *    pointer. if it is a NULL pointer, the input is automatically taken
 *    from stdin. Otherwise, stream is the stream from which input is to
 *    be taken.
 *
 * Return value
 * zero
 *  in this case *lineptr and *n are left unmodified.
 *  this occurs if
 *   1) lineptr was NULL
 *   2) n was NULL
 *   3) EOF or read error occurred before the function was able to scan
 *      anything useful.
 *   4) *lineptr was NULL and the function failed to initially allocate
 *      an one byte storage for the empty string.
 * nonzero
 *   the return value is the number of bytes among the *n bytes that were
 *   stored into by this function.
 *
 * Notes
 * *lineptr must be freed using free() to avoid memory leaks.
 *
 * special notes when *lineptr is not NULL
 * 1) the function will use the storage area pointed to by *lineptr.
 *    Since the function grows the area using realloc(), the value of
 *    *lineptr might change, causing the previous value to become invalid.
 *    an attempt to access the storage pointed to by the previous *lineptr
 *    is undefined behaviour. in case a copy of the previous pointer was
 *    made, it must be updated after a call to this function.
 * 2) providing an incorrect size in *n is undefined behaviour
 *
 * this implementation is cross platform, but it is certainly slower than
 * a built-in library function. If you have such a function available, then
 * you should avoid using this.
 *
 * further, this function does not follow the specs of POSIX. In particular,
 * it does not set errno to anything, and it does not use an ssize_t type
 * as its return value. the function consumes all input till the first newline
 * character that appears. This is true even if there isn't enough space to
 * store all the characters. The newline character is discarded and not stored
 * in the storage area pointed to by *lineptr.
 *
 * the strings formed by this function are always null terminated. However,
 * the standard library functions may fail to operate properly if the input
 * itself somehow contained a null character.
 * A test for this case can be written as
 * strlen(*lineptr) == one less than the return value of this function,
 * given it was nonzero.
 */
size_t cust_getline(char **lineptr, size_t *n, FILE *stream)
{
        if (lineptr == NULL || n == NULL)
                return 0;
        if (stream == NULL)
                stream = stdin;

        /* do this instead of (feof(stream) || ferror(stream))
         * because it is possible that eof or error indicator has
         * not yet been set, but the next read attempt would set it
         */
        int ch = getc(stream);
        if (ch == EOF)
                return 0;

        if (*lineptr == NULL) {
                *lineptr = malloc(1);
                if (*lineptr)
                        *n = 1;
                else
                        return 0;
        }

        size_t i = 0;
        while (ch != '\n' && ch != EOF) {
                if (i == *n - 1) {
                        if (*n <= SIZE_MAX / 2) {
                                char *temp = realloc(*lineptr, *n * 2);
                                if (temp != NULL) {
                                        *n *= 2;
                                        *lineptr = temp;
                                } else
                                        break;
                        } else
                                break;
                }
                (*lineptr)[i++] = ch;
                ch = getc(stream);
        }
        (*lineptr)[i++] = '\0';

        /* if ch is not the newline character, consume input until
         * newline occurs
         */
        while (ch != '\n' && ch != EOF)
                ch = getc(stream);

        return i;
}

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
        if (stream == NULL)
                stream = stdin;

        long ret = 0;

        char *input = NULL;
        size_t n = 0;
        while (cust_getline(&input, &n, stream) != 0) {
                char *endptr = NULL;
                errno = 0;
                ret = strtol(input, &endptr, 0);
                if (input == endptr)
                        fputs("Invalid input\n", stderr);
                else if (errno == ERANGE || ret > INT_MAX || ret < INT_MIN)
                        /* manually do this because strtol won't set it
                         * unless input was > LONG_MAX or < LONG_MIN
                         */
                        fprintf(stderr, "%s\n", strerror(ERANGE));
                else
                        break;
        }
        free(input);

        return (int) ret;
}

/* gets a long int from a stream by persistently
 * nagging the user to enter the right thing
 *
 * Input
 * 1) stream is the stream from which input is to be taken.
 * stdin is used if stream is NULL
 *
 * Return
 * Returns a long int. 0 is returned in case of a read error
 * or EOF
 *
 * Any input after the long int until the next newline character
 * is consumed by this function
 */
long int get_long(FILE *stream)
{
        if (stream == NULL)
                stream = stdin;

        long ret = 0;

        char *input = NULL;
        size_t n = 0;
        while (cust_getline(&input, &n, stream) != 0) {
                char *endptr = NULL;
                errno = 0;
                ret = strtol(input, &endptr, 0);
                if (input == endptr)
                        fputs("Invalid input\n", stderr);
                else if (errno == ERANGE)
                        fprintf(stderr, "%s\n", strerror(errno));
                else
                        break;
        }
        free(input);

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
        if (stream == NULL)
                stream = stdin;

        unsigned long ret = 0;

        char *input = NULL;
        size_t n = 0;
        while (cust_getline(&input, &n, stream) != 0) {
                char *checksign = strchr(input, '-');
                char *endptr = NULL;
                errno = 0;
                ret = strtoul(input, &endptr, 0);
                if (input == endptr
                                || (checksign != NULL && checksign < endptr))
                        fputs("Invalid input\n", stderr);
                else if (errno == ERANGE || ret > UINT_MAX)
                        /* manually do this because strtoul might not
                         * set errno unless input was > ULONG_MAX
                         */
                        fprintf(stderr, "%s\n", strerror(ERANGE));
                else
                        break;
        }
        free(input);

        return (unsigned int) ret;
}

/* gets an unsigned long long int from a stream by persistently
 * nagging the user to enter the right thing
 *
 * Input
 * 1) stream is the stream from which input is to be taken.
 * stdin is used if stream is NULL
 *
 * Return
 * Returns an unsigned long long int. 0 is returned in case of a read error
 * or EOF
 *
 * Any input after the unsigned int until the next newline character
 * is consumed by this function
 */
unsigned long long int get_unsigned_long_long(FILE *stream)
{
        if (stream == NULL)
                stream = stdin;

        unsigned long long int ret = 0;

        char *input = NULL;
        size_t n = 0;
        while (cust_getline(&input, &n, stream) != 0) {
                char *checksign = strchr(input, '-');
                char *endptr = NULL;
                errno = 0;
                ret = strtoull(input, &endptr, 0);
                if (input == endptr
                                || (checksign != NULL && checksign < endptr))
                        fputs("Invalid input\n", stderr);
                else if (errno == ERANGE)
                        fprintf(stderr, "%s\n", strerror(ERANGE));
                else
                        break;
        }
        free(input);

        return ret;
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
        if (stream == NULL)
                stream = stdin;

        double ret = 0;

        char *input = NULL;
        size_t n = 0;
        while (cust_getline(&input, &n, stream)) {
                char *endptr = NULL;
                errno = 0;
                ret = strtod(input, &endptr);
                if (input == endptr)
                        fputs("Invalid input\n", stderr);
                else if (errno == ERANGE)
                        fprintf(stderr, "%s\n", strerror(errno));
                else
                        break;
        }
        free(input);

        return ret;
}
