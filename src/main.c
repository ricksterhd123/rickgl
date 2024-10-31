#include "rickgl.h"

#include <stdio.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "model.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "tick.h"

const unsigned int SCR_WIDTH = 640;
const unsigned int SCR_HEIGHT = 480;

GLFWwindow *window;
Shader *shader;
Texture *texture1;
Texture *texture2;
Camera *camera;

Model* torus;
Model* cube;
Model* icosphere;

vec3 cameraPosition = {0, 0, -3.0f};
vec3 cameraLookAt = {0, 0, 0};

float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;
float distance = 5;
double lastxpos = 0;
double lastypos = 0;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
void process_input(GLFWwindow *window);

void main_loop()
{
    // Update camera position
    vec3 offset = {distance * cos(yaw) * sin(pitch), distance * cos(pitch), distance * sin(yaw) * sin(pitch)};
    set_camera_view(camera, offset, cameraLookAt);
    update_camera(camera);

    // Update shader values
    float tickCount = get_tick_count();
    shader_set_float(shader, "gTime", tickCount);
    shader_set_mat4(shader, "view", (float *)camera->view);
    shader_set_mat4(shader, "projection", (float *)camera->projection);

    // Process input
    process_input(window);

    // Clear scene
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set texture & shader
    use_texture(texture1);
    use_texture(texture2);
    // Draw model

    use_shader(shader);

    shader_set_mat4(shader, "model", (float *)torus->transform);
    draw_model(torus);

    shader_set_mat4(shader, "model", (float *)cube->transform);
    draw_model(cube);

    shader_set_mat4(shader, "model", (float *)icosphere->transform);
    draw_model(icosphere);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

int main(void)
{
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "rickgl demo", NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
#ifndef __EMSCRIPTEN__
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "Failed to initialize GLAD\n");
        return -1;
    }
#endif

    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    shader = load_shader(
        "assets/shaders/shader_vert.vert",
        "assets/shaders/shader_frag.frag");

    /////////////////////////////////////////

    torus = init_model("assets/torus.obj", 0, 0, 0);
    cube = init_model("assets/cube.obj", 2, 0, 0);
    icosphere = init_model("assets/icosphere.obj", -2, 0, 0);

    /////////////////////////////////////////

    texture1 = load_texture_2d("assets/textures/brickwall.jpg", GL_TEXTURE0);
    texture2 = load_texture_2d("assets/textures/wall.jpg", GL_TEXTURE1);

    /////////////////////////////////////////

    use_shader(shader);
    shader_set_int(shader, "texture1", 0);
    shader_set_int(shader, "texture2", 1);

    camera = init_camera(cameraPosition, cameraLookAt, fov, (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0, 100.0f);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 1);
#else
    while (!glfwWindowShouldClose(window))
    {
        main_loop();
    }
#endif

    destroy_model(torus);
    destroy_model(cube);
    destroy_model(icosphere);
    destroy_shader(shader);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    if (camera != NULL)
    {
        camera->aspect = (float)width / (float)height;
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    distance += yoffset * 0.5f;

    if (distance < 2)
    {
        distance = 2;
    }
    if (distance > 10)
    {
        distance = 10;
    }
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    double xpos = xposIn - lastxpos;
    double ypos = yposIn - lastypos;
    double sensitivity = 0.01;

    yaw += xpos * sensitivity;
    pitch += ypos * sensitivity;

    lastxpos = xposIn;
    lastypos = yposIn;
}

void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}
