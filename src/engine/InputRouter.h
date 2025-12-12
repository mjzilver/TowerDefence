#pragma once
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "../menu/Menu.h"
#include "../systems/ClickSystem.h"

struct InputContext {
    ClickSystem* click;
    Menu* menu;
    bool* menuMode;
};

void installInputCallbacks(GLFWwindow* win, InputContext* ctx);
