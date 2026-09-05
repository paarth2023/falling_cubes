#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define null NULL

const int width = 800;
const int height = 600;

void framebuffer_callback(GLFWwindow *window, int width, int height);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    std::cout << "Hello world" << std::endl;
    GLFWwindow *window =
        glfwCreateWindow(width, height, "Hello, World!", null, null);
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

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}

void framebuffer_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
