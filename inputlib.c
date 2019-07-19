/* C_inputlib version 6.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <limits.h>
#define INIT_SIZE 10
#include "inputlib.h"

/* gets a string upto the next newline from a given stream
 *
 * Parameters
 * 1) stream is the stream from which input is taken. if stream
 * is NULL, stdin is used.
 *
 * Return
 * a pointer to the new string. In case there are errors allocating
 * memory, NULL pointer is returned.
 * If the return value is not NULL, it should be freed using free()
 * before exiting the program.
 */
char* get_dynstring(FILE *stream)
{
        if (stream == NULL)
                stream = stdin;

        char *ret, *temp = NULL;
        size_t size = INIT_SIZE;
        ret = malloc(size);

        if (ret != NULL) {
                int ch;
                size_t i = 0;
                while ((ch = getc(stream)), ch != '\n' && ch != EOF) {
                        if (i == size - 1) {
                                if (size <= SIZE_MAX / 2)
                                        temp = realloc(ret, size * 2);
                                if (temp != NULL) {
                                        size *= 2;
                                        ret = temp;
                                        temp = NULL;
                                } else {
                                        free(ret);
                                        ret = NULL;
                                }
                        }
                        if (ret == NULL)
                                break;
                        ret[i] = ch;
                        i++;
                }
                if (ret != NULL)
                        ret[i] = '\0';
        }

        return ret;
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
        char *input;
        char *endptr;

        int valid = 0;
        while (valid == 0 && feof(stream) == 0 && ferror(stream) == 0) {
                if ((input = get_dynstring(stream)) != NULL) {
                        ret = strtol(input, &endptr, 0);
                        if (input == endptr)
                                fputs("Invalid input\n", stderr);
                        else if (ret > INT_MAX || ret < INT_MIN) {
                                /* manually do this because strtol won't set it
                                 * unless input was > LONG_MAX or < LONG_MIN
                                 */
                                fprintf(stderr, "%s\n", strerror(ERANGE));
                                errno = 0;
                        } else
                                valid = 1;
                        free(input);
                }
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
        if (stream == NULL)
                stream = stdin;

        long ret = 0;
        char *input;
        char *endptr;

        int valid = 0;
        while (valid == 0 && feof(stream) == 0 && ferror(stream) == 0) {
                if ((input = get_dynstring(stream)) != NULL) {
                        ret = strtol(input, &endptr, 0);
                        if (input == endptr)
                                fputs("Invalid input\n", stderr);
                        else if (errno == ERANGE) {
                                fprintf(stderr, "%s\n", strerror(errno));
                                errno = 0;
                        } else
                                valid = 1;
                        free(input);
                }
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
        if (stream == NULL)
                stream = stdin;

        unsigned long ret = 0;
        char *input;
        char *endptr;

        int valid = 0;
        while (valid == 0 && feof(stream) == 0 && ferror(stream) == 0) {
                if ((input = get_dynstring(stream)) != NULL) {
                        ret = strtoul(input, &endptr, 0);
                        if (input == endptr)
                                fputs("Invalid input\n", stderr);
                        else if (ret > UINT_MAX || errno == ERANGE) {
                                /* manually do this because strtoul might not
                                 * set errno unless input was > ULONG_MAX
                                 */
                                fprintf(stderr, "%s\n", strerror(ERANGE));
                                errno = 0;
                        } else
                                valid = 1;
                        free(input);
                }
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
        if (stream == NULL)
                stream = stdin;

        double ret = 0;
        char *input;
        char *endptr;

        int valid = 0;
        while (valid == 0 && feof(stream) == 0 && ferror(stream) == 0) {
                if ((input = get_dynstring(stream)) != NULL) {
                        ret = strtod(input, &endptr);
                        if (input == endptr)
                                fputs("Invalid input\n", stderr);
                        else if (errno == ERANGE) {
                                fprintf(stderr, "%s\n", strerror(errno));
                                errno = 0;
                        } else
                                valid = 1;
                        free(input);
                }
        }

        return ret;
}
