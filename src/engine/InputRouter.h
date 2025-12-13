#pragma once
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "../menu/Menu.h"
#include "../systems/ClickSystem.h"
#include "../systems/StateSystem.h"

struct InputContext {
    ClickSystem* click;
    Menu* menu;
    StateSystem* stateSystem;
};

void installInputCallbacks(GLFWwindow* win, InputContext* ctx);
