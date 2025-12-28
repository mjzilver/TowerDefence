#pragma once

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <vector>

#include "../ecs/SystemManager.h"
#include "../map/MapLoader.h"
#include "../systems/RenderSystem.h"
#include "../systems/StateSystem.h"
#include "UIElement.h"

class Menu {
public:
    Menu(EventDispatcher& eventDispatcher) : eventDispatcher(eventDispatcher) {}

    void registerElement(UIElement* e) { elements.push_back(e); }

    void onClick(const glm::vec2& clickPos);

    void onKeyPress(int key, int mods);

    void onHover(const glm::vec2& pos);

    void render(RenderSystem& renderSystem);

    void createMainMenu(MapLoader& mapLoader, SystemManager& systemManager, StateSystem& stateSystem, GLFWwindow* window);

    ~Menu() {
        for (auto* e : elements) {
            delete e;
        }
    };

private:
    EventDispatcher& eventDispatcher;
    std::vector<UIElement*> elements;
};
