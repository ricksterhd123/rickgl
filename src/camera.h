#pragma once

#include <cglm/cglm.h>

typedef struct Camera
{
    mat4* view;
    mat4* projection;

    float fovy;
    float aspect;
    float nearZ;
    float farZ;
} Camera;

void update_camera(Camera* camera) {
    glm_perspective(camera->fovy, camera->aspect, camera->nearZ, camera->farZ, (vec4*) camera->projection);
}

vec3* get_camera_position(Camera* camera) {
    return (vec3*) camera->view[3];
}

void set_camera_view(Camera* camera, vec3 position, vec3 lookAt)
{
    vec3 center;
    glm_vec3_sub(lookAt, position, center);
    glm_normalize(center);
    glm_vec3_add(position, center, center);
    vec3 up = {0, 1.0f, 0};

    glm_lookat(position, center, up, (vec4*) camera->view);
}

Camera* init_camera(vec3 position, vec3 lookAt, float fovy, float aspect, float nearZ, float farZ)
{
    Camera* camera = (Camera*) malloc(sizeof(Camera));

    camera->projection = (mat4*) malloc(sizeof(mat4));
    camera->view = (mat4*) malloc(sizeof(mat4));
    
    glm_mat4_identity((vec4*) camera->projection);
    glm_mat4_identity((vec4*) camera->view);

    camera->fovy = fovy;
    camera->aspect = aspect;
    camera->nearZ = nearZ;
    camera->farZ = farZ;

    update_camera(camera);
    set_camera_view(camera, position, lookAt);

    return camera;
}

void destroy_camera(Camera* camera)
{
    free(camera->projection);
    free(camera->view);
    free(camera);
}