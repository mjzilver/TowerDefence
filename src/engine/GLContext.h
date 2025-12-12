#pragma once
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

class GLContext {
public:
    GLFWwindow* initWindow(int width, int height, const char* title);
    void setupOpenGL();
};
