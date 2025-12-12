#pragma once

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on#include
#include <vector>

#include "../systems/CollisionSystem.h"
#include "../systems/RenderSystem.h"
#include "../map/MapLoader.h"
#include "UIElement.h"
#include "../utils/Globals.h"

class Menu {
public:
    Menu() = default;

    void registerElement(UIElement* e) { elements.push_back(e); }

    void onClick(const glm::vec2& clickPos) {
        for (auto* e : elements) {
            if (e && CollisionSystem::contains(e->getPos(), clickPos)) {
                e->onClick();
                break;
            }
        }
    }

    void onHover(const glm::vec2& pos) {
        for (auto* e : elements) {
            bool inside = CollisionSystem::contains(e->getPos(), pos);
            e->setHovered(inside);
        }
    }

    void render(RenderSystem& renderSystem) {
        for (auto& e : elements) {
            glm::vec3 color = e->hovered() ? e->getHoverColor() : e->getBgColor();

            renderSystem.renderText(
                e->getPos(),
                e->getText(),
                e->getTextColor(),
                renderSystem.getShader("text")
            );

            renderSystem.renderSquare(
                e->getPos(), 
                color, 
                renderSystem.getShader("square")
            );
        }
    };

void createMainMenu(MapLoader& mapLoader, bool& menuMode, GLFWwindow* window) {
    float centerX = (SCREEN_WIDTH - 250) * 0.5f; 
    float titleWidth = 400;
    float titleX = (SCREEN_WIDTH - titleWidth) * 0.5f;

    auto make = [&](float y, const std::string& text, const glm::vec3& bg, std::function<void()> clb) {
        UIElement* e = new UIElement();
        e->setPos({centerX, y, 250, 60});
        e->setText(text);
        e->setBgColor(bg);
        e->setCallback(clb);
        registerElement(e);
    };
    
    UIElement* title = new UIElement();
    title->setPos({titleX, 20, titleWidth, 80});
    title->setText("--== Tower Defence ==--\n\n*** Select a map ***");
    title->setBgColor({0.2f, 0.2f, 0.2f});
    title->setTextColor({0.9f, 0.9f, 0.9f});
    registerElement(title);

    make(130, "Map 1", {0.5f, 0.5f, 0.8f}, [&]{
        mapLoader.loadMap("map1.txt");
        menuMode = false;
    });

    make(210, "Map 2", {0.5f, 0.8f, 0.5f}, [&]{
        mapLoader.loadMap("map2.txt");
        menuMode = false;
    });

    make(290, "Map 3", {0.8f, 0.5f, 0.5f}, [&]{
        mapLoader.loadMap("map3.txt");
        menuMode = false;
    });

    make(370, "Map 4", {0.8f, 0.8f, 0.5f}, [&]{
        mapLoader.loadMap("map4.txt");
        menuMode = false;
    });

    make(480, "Quit", {0.9f, 0.3f, 0.3f}, [window]{
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    });
}

private:
    std::vector<UIElement*> elements;
};
