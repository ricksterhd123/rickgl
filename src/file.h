#pragma once

#include <stdlib.h>
#include <stdio.h>

char* read_file(const char* path) {
    char* buffer;
    long length;

    FILE* f = fopen(path, "rb");
    if (f != NULL) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);

        buffer = (char*) calloc(length + 1, sizeof(char));
        if (buffer != NULL) {
            fread(buffer, sizeof(char), length, f);
            buffer[length + 1] = '\0';
        }

        fclose(f);
        return buffer;
    }
    return NULL;
}
