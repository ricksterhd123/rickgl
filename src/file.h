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
        buffer = (char*) malloc(length);
        if (buffer != NULL) {
            fread(buffer, 1, length, f);
        }
        fclose(f);
        return buffer;
    }
    return NULL;
}
