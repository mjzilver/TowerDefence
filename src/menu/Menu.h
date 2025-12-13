#pragma once

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <vector>

#include "../systems/StateSystem.h"
#include "../systems/RenderSystem.h"
#include "../ecs/SystemManager.h"
#include "../map/MapLoader.h"
#include "UIElement.h"

class Menu {
public:
    Menu() = default;

    void registerElement(UIElement* e) { elements.push_back(e); }

    void onClick(const glm::vec2& clickPos);
    
    void onKeyPress(int key);

    void onHover(const glm::vec2& pos);

    void onKeyPress(int key);

    void render(RenderSystem& renderSystem);

    void createMainMenu(MapLoader& mapLoader, ComponentManager& componentManager, SystemManager& systemManager, StateSystem& stateSystem, GLFWwindow* window);

    ~Menu() {
        for (auto* e : elements) {
            delete e;
        }
    };

private:
    std::vector<UIElement*> elements;
};
