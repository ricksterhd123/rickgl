#include "rickgl.h"

#include <stdio.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION

#include <nuklear.h>
#include <nuklear_glfw_gl3.h>

#include "model.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "tick.h"

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

struct nk_glfw glfw;
struct nk_context *context;

GLFWwindow *window;
Shader *shader;
Texture *texture1;
Texture *texture2;
Camera *camera;

Model *torus;
Model *cube;
Model *icosphere;

vec3 cameraPosition = {0, 0, -3.0f};
vec3 cameraLookAt = {0, 0, 0};

float yaw = 0.0f;
float pitch = 45.0f;
float fov = 45.0f;
float distance = 50;
double lastxpos = 0;
double lastypos = 0;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
void process_input(GLFWwindow *window);
void draw_gui();
void main_loop();

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

    context = nk_glfw3_init(&glfw, window, NK_GLFW3_DEFAULT);
    struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(&glfw, &atlas);
    nk_glfw3_font_stash_end(&glfw);

    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    shader = load_shader(
        "assets/shaders/shader_vert.vert",
        "assets/shaders/shader_frag.frag");

    /////////////////////////////////////////

    torus = init_model("assets/models/map.obj", 0, 0, 0);
    cube = init_model("assets/models/cube.obj", 2, 1, 0);
    icosphere = init_model("assets/models/icosphere.obj", -2, 1, 0);

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
    nk_glfw3_shutdown(&glfw);

    glfwTerminate();
    return 0;
}

static char GUI_TEXT_BUFFER[256];
void draw_gui()
{
    nk_glfw3_new_frame(&glfw);

    if (nk_begin(context, "Settings", nk_rect(0, 0, 200, 200), NK_WINDOW_BORDER | NK_WINDOW_TITLE))
    {
        sprintf(GUI_TEXT_BUFFER, "Camera Yaw: %f", yaw);
        nk_layout_row_static(context, 20, 150, 1);
        nk_label(context, GUI_TEXT_BUFFER, NK_TEXT_LEFT);

        sprintf(GUI_TEXT_BUFFER, "Camera Pitch: %f", pitch);
        nk_layout_row_static(context, 20, 150, 1);
        nk_label(context, GUI_TEXT_BUFFER, NK_TEXT_LEFT);

        sprintf(GUI_TEXT_BUFFER, "Camera Distance: %f", distance);
        nk_layout_row_push(context, 150);
        nk_label(context, GUI_TEXT_BUFFER, NK_TEXT_LEFT);

        nk_layout_row_begin(context, NK_STATIC, 30, 2);
        {
            nk_layout_row_push(context, 150);
            nk_slider_float(context, 10.0f, &distance, 75.0f, 0.1f);
        }
        nk_layout_row_end(context);

        nk_end(context);
    }
    nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}

void main_loop()
{
    float tickCount = get_tick_count();
    yaw += 0.1f;
    if (yaw > 360)
    {
        yaw = 0;
    }

    if (pitch > 179.0f)
    {
        pitch = 179.0f;
    }
    if (pitch < 0.1f)
    {
        pitch = 0.1;
    }

    // Update camera position
    vec3 offset = {distance * cos(glm_rad(yaw)) * sin(glm_rad(pitch)), distance * cos(glm_rad(pitch)), distance * sin(glm_rad(yaw)) * sin(glm_rad(pitch))};
    set_camera_view(camera, offset, cameraLookAt);
    update_camera(camera);

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

    shader_set_float(shader, "gTime", tickCount);
    shader_set_mat4(shader, "view", (float *)camera->view);
    shader_set_mat4(shader, "projection", (float *)camera->projection);

    shader_set_mat4(shader, "model", (float *)torus->transform);
    draw_model(torus);

    shader_set_mat4(shader, "model", (float *)cube->transform);
    draw_model(cube);

    shader_set_mat4(shader, "model", (float *)icosphere->transform);
    draw_model(icosphere);

    draw_gui();

    glfwSwapBuffers(window);
    glfwPollEvents();
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
    // distance += yoffset * 0.5f;
    // if (distance < 2)
    // {
    //     distance = 2;
    // }
    // if (distance > 100)
    // {
    //     distance = 10;
    // }
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    // double xpos = xposIn - lastxpos;
    // double ypos = yposIn - lastypos;
    // double sensitivity = 0.5;
    // yaw += xpos * sensitivity;
    // pitch += ypos * sensitivity;
    // lastxpos = xposIn;
    // lastypos = yposIn;
}

void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        pitch += 5;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        pitch -= 5;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        yaw += 5;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        yaw -= 5;
}
