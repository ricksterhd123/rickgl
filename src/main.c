#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "tick.h"

const unsigned int SCR_WIDTH = 640;
const unsigned int SCR_HEIGHT = 480;

Camera *camera;
float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;
float distance = 5;

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

double lastxpos = 0;
double lastypos = 0;

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

int main(void)
{
    GLFWwindow *window;

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
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "Failed to initialize GLAD\n");
        return -1;
    }

    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    Shader *shader = load_shader(
        "assets/shaders/shader_vert.vs",
        "assets/shaders/shader_frag.fs");

    // ------------------------------------------------------------------
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    /////////////////////////////////////////

    Texture *texture1 = load_texture_2d("assets/textures/brickwall.jpg", GL_TEXTURE0);
    Texture *texture2 = load_texture_2d("assets/textures/wall.jpg", GL_TEXTURE1);

    /////////////////////////////////////////

    use_shader(shader);
    shader_set_int(shader, "texture1", 0);
    shader_set_int(shader, "texture2", 1);

    vec3 cameraPosition = {0, 0, -3.0f};
    vec3 cameraLookAt = {0, 0, 0};
    camera = init_camera(cameraPosition, cameraLookAt, fov, (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0, 100.0f);

    mat4 model;
    glm_mat4_identity(model);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        // Update camera position
        vec3 offset = {distance * cos(yaw) * sin(pitch), distance * cos(pitch), distance * sin(yaw) * sin(pitch)};
        set_camera_view(camera, offset, cameraLookAt);
        update_camera(camera);

        // Update shader values
        float tickCount = get_tick_count();
        shader_set_float(shader, "gTime", tickCount);
        shader_set_mat4(shader, "model", (float *)model);
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
        use_shader(shader);

        // Draw model
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    destroy_shader(shader);

    glfwTerminate();
    return 0;
}
