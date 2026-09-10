#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define null NULL

const int sWidth = 800;
const int sHeight = 600;

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

    GLuint vertexshader;
    vertexshader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexshader, 1, &vertexShaderSource, null);
    glCompileShader(vertexshader);

    GLuint fragmentshader;
    fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentshader, 1, &fragmentShaderSource, null);
    glCompileShader(fragmentshader);

    GLuint shaderprogram;
    shaderprogram = glCreateProgram();
    glAttachShader(shaderprogram, vertexshader);
    glAttachShader(shaderprogram, fragmentshader);
    glLinkProgram(shaderprogram);

    glDeleteShader(vertexshader);
    glDeleteShader(fragmentshader);

    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // top right
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f,  0.0f, 0.0f, 1.0f  // top left
    };
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

    glUseProgram(shaderprogram);
    glUniform1i(glGetUniformLocation(shaderprogram, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shaderprogram, "texture2"), 1);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUseProgram(shaderprogram);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f),
                            glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 projection;
        projection = glm::perspective(
            glm::radians(45.0f), (float)sWidth / (float)sHeight, 0.1f, 100.0f);

        GLint modelLoc = glGetUniformLocation(shaderprogram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        GLint projectionLoc = glGetUniformLocation(shaderprogram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE,
                           glm::value_ptr(projection));
        GLint viewLoc = glGetUniformLocation(shaderprogram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glBindVertexArray(vao);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}

void framebuffer_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
