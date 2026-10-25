#include <glad/glad.h>
#include "error_reporting.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>
#include <vector>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "VertexBuffer.h"
#include "buffer_layout.h"
#include "VertexArray.h"
#include "Mesh.h"
#include "Camera.h"

#define null NULL

const int sWidth = 1024;
const int sHeight = 768;

void framebuffer_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, Camera &camera, float deltaTime);

enum format { rgb = GL_RGB, rgba = GL_RGBA };

struct FallingCube {
    glm::vec3 position;
    float speed;
    float rotationAngle;
    float rotationSpeed;
    glm::vec3 rotationAxis;
    float scale;
    float laneX;
    float laneZ;

    void reset(float startY, std::mt19937 &gen)
    {
        std::uniform_real_distribution<float> speedDist(1.5f, 3.5f);
        std::uniform_real_distribution<float> rotSpeedDist(30.0f, 120.0f);
        std::uniform_real_distribution<float> axisDist(0.2f, 1.0f);
        std::uniform_real_distribution<float> scaleDist(0.45f, 0.75f);

        position.x = laneX;
        position.z = laneZ;
        position.y = startY;

        speed = speedDist(gen);
        rotationAngle = 0.0f;
        rotationSpeed = rotSpeedDist(gen);
        rotationAxis = glm::normalize(
            glm::vec3(axisDist(gen), axisDist(gen), axisDist(gen)));
        scale = scaleDist(gen);
    }

    void update(float deltaTime, std::mt19937 &gen)
    {
        position.y -= speed * deltaTime;
        rotationAngle += rotationSpeed * deltaTime;

        // When completely offscreen below the viewing volume, respawn at the top in the same lane
        if (position.y < -4.5f) {
            std::uniform_real_distribution<float> yDist(4.0f, 7.5f);
            reset(yDist(gen), gen);
        }
    }

    glm::mat4 getModelMatrix() const
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotationAngle), rotationAxis);
        model = glm::scale(model, glm::vec3(scale));
        return model;
    }
};

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    std::cout << "Hello world" << std::endl;
    GLFWwindow *window =
        glfwCreateWindow(sWidth, sHeight, "Falling Cubes", null, null);
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

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        enableErrorReporting();
    }

    std::string resPrefix = "resources/";
    if (!std::filesystem::exists(resPrefix) &&
        std::filesystem::exists("../resources/")) {
        resPrefix = "../resources/";
    }

    std::filesystem::path vertexShader(resPrefix + "vertexshader.glsl");
    std::filesystem::path fragmentShader(resPrefix + "fragmentshader.glsl");
    Shader shader(vertexShader, fragmentShader);

    Camera camera(glm::vec3(0.0f, 0.5f, 4.0f));
    Mesh cubeMesh = Mesh::createCube();

    // Initialize randomized lanes of falling cubes
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<FallingCube> cubes;
    const std::vector<float> xLanes = {-3.5f, -2.1f, -0.7f, 0.7f, 2.1f, 3.5f};
    const std::vector<float> zLanes = {0.2f, -1.6f, -3.4f, -5.2f};
    std::uniform_real_distribution<float> initYDist(-4.0f, 6.0f);

    for (float x : xLanes) {
        for (float z : zLanes) {
            FallingCube cube;
            cube.laneX = x;
            cube.laneZ = z;
            cube.reset(initYDist(gen), gen);
            cubes.push_back(cube);
        }
    }

    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char *data = stbi_load((resPrefix + "container.jpg").c_str(),
                                    &width, &height, &nrChannels, 0);
    std::cout << nrChannels << std::endl;
    format setting;
    if (nrChannels == 3)
        setting = rgb;
    else if (nrChannels == 4)
        setting = rgba;

    GLuint texture1, texture2;
    glGenTextures(1, &texture1);
    glGenTextures(1, &texture2);
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

    data = stbi_load((resPrefix + "awesomeface.png").c_str(), &width, &height,
                     &nrChannels, 0);
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

    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    float prevTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        float currTime = glfwGetTime();
        float deltaTime = currTime - prevTime;
        prevTime = currTime;

        processInput(window, camera, deltaTime);

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.1f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        shader.use();

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f), (float)sWidth / (float)sHeight, 0.1f, 100.0f);

        shader.setMatrix("view", view);
        shader.setMatrix("projection", projection);

        // Update and draw all falling cubes
        for (auto &cube : cubes) {
            cube.update(deltaTime, gen);
            shader.setMatrix("model", cube.getModelMatrix());
            cubeMesh.draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}

void processInput(GLFWwindow *window, Camera &camera, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // WASD movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::RIGHT, deltaTime);

    // Arrow keys rotation
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.processRotation(CameraRotation::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.processRotation(CameraRotation::DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.processRotation(CameraRotation::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.processRotation(CameraRotation::RIGHT, deltaTime);
}

void framebuffer_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
