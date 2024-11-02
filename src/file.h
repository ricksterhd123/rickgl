#pragma once

#include <stdlib.h>
#include <stdio.h>

char *read_file(const char *path)
{
    char *buffer;
    long length;

    FILE *f = fopen(path, "rb");
    if (f != NULL)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);

        buffer = (char *)calloc(length + 1, sizeof(char));
        if (buffer == NULL)
        {
            fprintf(stderr, "Failed to read from file %s: not enough memory!", path);
            fclose(f);
            return NULL;
        }

        size_t read_bytes = fread(buffer, sizeof(char), length, f);
        if (read_bytes != length && ferror(f))
        {
            fprintf(stderr, "Failed to read from file %s\n", path);
            free(buffer);
            fclose(f);
            return NULL;
        }

        buffer[length + 1] = '\0';

        fclose(f);
        return buffer;
    }
    return NULL;
}
