#pragma once

#include <glad/glad.h>
#include "file.h"

typedef struct Shader
{
    unsigned int id;
    const char *vertex_path;
    const char *fragment_path;
} Shader;

unsigned int load_shader_from_file(const char *path, int shader_type)
{
    int success;
    char infoLog[512];

    const char *source = read_file(path);

    unsigned int shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "Failed to compile shader: %s\n", infoLog);
        return -1;
    }

    return shader;
}

Shader *load_shader(const char *vertexPath, const char *fragmentPath)
{
    Shader *result = (Shader *)malloc(sizeof(Shader));

    if (result == NULL)
    {
        fprintf(stderr, "Failed to initialize shader obj: Not enough memory!\n");
        return NULL;
    }

    result->vertex_path = vertexPath;
    result->fragment_path = fragmentPath;

    unsigned int vertexShader = load_shader_from_file(vertexPath, GL_VERTEX_SHADER);
    unsigned int fragmentShader = load_shader_from_file(fragmentPath, GL_FRAGMENT_SHADER);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "Failed to link shaders: %s\n", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    result->id = shaderProgram;

    return result;
}

void use_shader(Shader *shader)
{
    glUseProgram(shader->id);
}

void destroy_shader(Shader *shader)
{
    glDeleteProgram(shader->id);
}

void shader_set_int(Shader *shader, const char *name, int value)
{
    glUniform1i(glGetUniformLocation(shader->id, name), value);
}

void shader_set_bool(Shader *shader, const char *name, int value)
{
    glUniform1i(glGetUniformLocation(shader->id, name), value);
}

void shader_set_float(Shader *shader, const char *name, float value)
{
    glUniform1f(glGetUniformLocation(shader->id, name), value);
}
