#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

#define null NULL

const int sWidth = 1024;
const int sHeight = 768;

const float gravity = 9.8f;
const glm::vec3 initVeloctiy = glm::vec3(0.0f, 0.002f, 0.0f);
const glm::vec3 initPosition = glm::vec3(0.5f, 0.8f, -0.4f);

void framebuffer_callback(GLFWwindow *window, int width, int height);

enum format { rgb = GL_RGB, rgba = GL_RGBA };

const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTexCoord;\n"
    "out vec2 TexCoord;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
    "	TexCoord = aTexCoord;\n"
    "}\0";

const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D texture1;\n"
    "uniform sampler2D texture2;\n"
    "void main(){\n"
    "	FragColor = mix(texture(texture1, TexCoord), texture(texture2, "
    "TexCoord), 0.2);\n"
    "}\n\0";

glm::vec3 updatePosition(glm::vec3 position, float currTime, glm::mat4 view,
                         glm::mat4 projection)
{
    float y = position.y - 0.5f * gravity * currTime * currTime;
    glm::vec3 temp = glm::vec3(position.x, y, position.z);
    glm::vec4 clipSpace = projection * view * glm::vec4(temp, 1.0f);
    glm::vec3 ndc = glm::vec3(clipSpace) / clipSpace.w;
    bool isVisible = (ndc.x >= -1.0f && ndc.x <= 1.0f) &&
                     (ndc.y >= -1.0f && ndc.y <= 1.0f) &&
                     (ndc.z >= -1.0f && ndc.z <= 1.0f);
    if (!isVisible)
        return initPosition;
    return temp;
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    std::cout << "Hello world" << std::endl;
    GLFWwindow *window =
        glfwCreateWindow(sWidth, sHeight, "Hello, World!", null, null);
    if (window == NULL) {
        std::cout << "Could not create window" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD" << std::endl;
        return 1;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_callback);

    Shader shader;
    std::string vertexSource = "resources/vertexshader";
    shader.setVertexSource(vertexSource);
    shader.compileVertexShader();

    std::string fragmentSource = "resources/fragmentshader";
    shader.setFragmentSource(fragmentSource);
    shader.compileFragmentShader();

    shader.linkShaderProgram();

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

        -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};
    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char *data =
        stbi_load("resources/container.jpg", &width, &height, &nrChannels, 0);
    std::cout << nrChannels << std::endl;
    format setting;
    if (nrChannels == 3)
        setting = rgb;
    else if (nrChannels == 4)
        setting = rgba;

    GLuint vbo, vao, ebo, texture1, texture2;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &ebo);
    glGenTextures(1, &texture1);
    glGenTextures(1, &texture2);

    glBindVertexArray(vao);

    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, setting, width, height, 0, setting,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Error loading texture1" << std::endl;
    }
    stbi_image_free(data);

    data =
        stbi_load("resources/awesomeface.png", &width, &height, &nrChannels, 0);
    if (nrChannels == 4)
        setting = rgba;
    else if (nrChannels == 3)
        setting = rgba;
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, setting, width, height, 0, setting,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    float prevTime = glfwGetTime();
    float rotationAngle = 0.0f;
    float rotationSpeed = 90.0f;

    glm::vec3 position = initPosition;
    glm::vec3 velocity = glm::vec3(0.0f, 0.002f, 0.0f);

    while (!glfwWindowShouldClose(window)) {
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        shader.use();
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 projection;
        projection = glm::perspective(
            glm::radians(45.0f), (float)sWidth / (float)sHeight, 0.1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0f);
        float currTime = glfwGetTime();
        float deltaTime = currTime - prevTime;
        prevTime = currTime;
        rotationAngle += rotationSpeed * deltaTime;
        position = updatePosition(position, 5.0f * deltaTime, view, projection);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotationAngle),
                            glm::vec3(0.5f, 1.0f, 0.0f));

        shader.setMatrix4fv("model", glm::value_ptr(model));
        shader.setMatrix4fv("view", glm::value_ptr(view));
        shader.setMatrix4fv("projection", glm::value_ptr(projection));
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}

void framebuffer_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
