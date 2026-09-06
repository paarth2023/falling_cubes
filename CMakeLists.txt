cmake_minimum_required(VERSION 3.20)

project(cubes LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GLFW
add_subdirectory(external/glfw-3.5.1)

# GLAD
add_library(glad STATIC
    external/glad/src/glad.c
)

target_include_directories(glad PUBLIC
    external/glad/include
)

# glm
add_subdirectory(external/glm)

add_executable(cubes
    src/main.cpp
	src/stb_image.cpp
)

target_link_libraries(cubes PRIVATE
    glfw
    glad
	glm
)

# OpenGL
find_package(OpenGL REQUIRED)

target_link_libraries(cubes PRIVATE
    OpenGL::GL
)
