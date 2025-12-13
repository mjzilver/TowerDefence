#include "InputRouter.h"

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
}
