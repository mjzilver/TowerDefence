#include "InputRouter.h"

#include <GLFW/glfw3.h>

void installInputCallbacks(GLFWwindow* window, InputContext* ctx) {
    glfwSetWindowUserPointer(window, ctx);

    glfwSetMouseButtonCallback(window, [](GLFWwindow* win, int button, int action, int) {
        double x, y;
        glfwGetCursorPos(win, &x, &y);
        auto* ctx = static_cast<InputContext*>(glfwGetWindowUserPointer(win));

        if (ctx->stateSystem->getState() == EngineState::MAIN_MENU)
            ctx->menu->onClick({x, y});
        else
            ctx->click->onClick(button, action, x, y);
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* win, double x, double y) {
        auto* ctx = static_cast<InputContext*>(glfwGetWindowUserPointer(win));

        if (ctx->stateSystem->getState() == EngineState::MAIN_MENU)
            ctx->menu->onHover({x, y});
        else
            ctx->click->onHover(x, y);
    });

    glfwSetKeyCallback(window, [](GLFWwindow* win, int key, int scancode, int action, int mods) {
        (void)scancode;
        (void)mods;
        if (action == GLFW_PRESS) {
            auto* ctx = static_cast<InputContext*>(glfwGetWindowUserPointer(win));
            ctx->menu->onKeyPress(key);
        }
    });
}
