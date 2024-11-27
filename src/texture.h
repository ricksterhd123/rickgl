#pragma once

#include "rickgl.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

typedef struct Texture
{
    char textureName[256];
    unsigned int id;
    GLenum index;
} Texture;

Texture *init_texture_2d(const char *path, GLenum index);
void use_texture(Texture *texture);

Texture *init_texture_2d(const char *path, GLenum index)
{
    Texture *texture = (Texture *)malloc(sizeof(Texture));
    if (texture == NULL)
    {
        fprintf(stderr, "Failed to initialize texture: not enough memory!\n");
        return NULL;
    }

    texture->index = index;
    glGenTextures(1, &texture->id);

    use_texture(texture);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        fprintf(stderr, "Failed to load texture\n");
        return NULL;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    return texture;
}

void destroy_texture(Texture *texture)
{
    glDeleteTextures(1, &texture->id);
    free(texture);
}

void use_texture(Texture *texture)
{
    glActiveTexture(texture->index);
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

void clear_texture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
