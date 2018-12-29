#include "reader.h"
#include "types.h"
#include <string.h>
#include <sys/types.h>

int my_strlen(const char *string)
{
    if (string == NULL)
    {
        return 0;
    }

    int i = 0;
    while (*(string + i) != '\0')
    {
        i++;
    }

    return i;
}


char *my_strcpy(char *dest, const char *src)
{
    if (dest == NULL || src == NULL)
    {
        return NULL;
    }

    int i = 0;
    while (*(src + i) != '\0')
    {
        *(dest + i) = *(src + i);
        i++;
    }
    *(dest + i) = '\0';

    return dest;
}

ssize_t my_getline(char **lineptr, size_t *n, FILE *stream)
{
    if (lineptr == NULL || n == NULL || stream == NULL)
    {
        return EOF;
    }

    if (ferror(stream))
    {
        return EOF;
    }

    if (feof(stream))
    {
        return EOF;
    }

    if (*lineptr == NULL && *n > 0)
    {
        return EOF;
    }

    ssize_t result = 0;
    char buffer[DEFAULT_BUFFER_SIZE] = { 0 };
    size_t current_length = 0;
    size_t allocated_length = *n;
    int found_eol = FALSE;

    while (found_eol == FALSE)
    {
        char *res = fgets(buffer, DEFAULT_BUFFER_SIZE, stream);

        size_t read_length = my_strlen(buffer);

        if (read_length == 0)
        {
            *n = allocated_length;
            result = current_length;
            if (result == 0)
            {
                return EOF;
            }

            return result;
        }

        if (buffer != NULL && res == NULL && found_eol == FALSE)
        {
            found_eol = TRUE;

            return my_strlen(buffer);
        }

        if (*(buffer + read_length - 1) == '\n')
        {
            found_eol = TRUE;
            // *(buffer + read_length - 1) = '\0';
        }

        if (current_length + read_length + 1 > allocated_length)
        {
            size_t tmp_length = allocated_length * 2 + 1;
            if (read_length + 1 > tmp_length)
            {
                tmp_length = read_length + 1;
            }

            char *tmp = realloc(*lineptr, tmp_length);
            if (tmp == NULL)
            {
                free(*lineptr);
                return EOF;
            }

            *lineptr = tmp;
            allocated_length = tmp_length;
        }

        my_strcpy(*lineptr + current_length, buffer);
        current_length += read_length;
    }

    *n = allocated_length;

    result = current_length;
    return result;
}

int read_option(void)
{
    char *line = NULL;
    size_t n = 0;
    ssize_t read_bytes = my_getline(&line, &n, stdin);
    if (read_bytes != EOF)
    {
        int opt = atoi(line);

        free(line);
        return opt;
    }

    return EOF;
}
