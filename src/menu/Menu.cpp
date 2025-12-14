#include "Menu.h"

#include "../event/Event.h"
#include "../event/EventDispatcher.h"
#include "../systems/CollisionSystem.h"
#include "../utils/Color.h"
#include "../utils/Globals.h"
#include "UIButton.h"
#include "UILabel.h"

void Menu::onClick(const glm::vec2& clickPos) {
    for (auto* e : elements) {
        if (e && CollisionSystem::contains(e->getPos(), clickPos)) {
            e->onClick();
            break;
        }
    }
}

void Menu::onHover(const glm::vec2& pos) {
    for (auto* e : elements) {
        bool inside = CollisionSystem::contains(e->getPos(), pos);
        e->setHovered(inside);
    }
}

void Menu::onKeyPress(int key) {
    switch (key) {
        case GLFW_KEY_C:
            Event event;
            event.type = EventType::ACTIVATE_CHEATS;
            EventDispatcher::getInstance().dispatch(event);
            break;
    }
}

void Menu::render(RenderSystem& renderSystem) {
    for (auto* e : elements) {
        renderSystem.renderText(e->getPos(), e->getText(), e->getTextColor(), renderSystem.getShader("text"));

        glm::vec3 color = e->getBgColor();

        if (e->isClickable() && e->hovered()) {
            color = color * e->getHoverColor();
        }

        renderSystem.renderRectangle(e->getPos(), color, renderSystem.getShader("rect"));
    }
}

void Menu::createMainMenu(MapLoader& mapLoader, SystemManager& systemManager, StateSystem& stateSystem, GLFWwindow* window) {
    const float centerX = (SCREEN_WIDTH - 250) * 0.5f;
    const float buttonHeight = 60;
    const float buttonmargin = 20;

    auto make = [&](float y, const std::string& text, std::function<void()> clb) {
        UIButton* e = new UIButton();
        e->setPos({centerX, y, 250, buttonHeight});
        e->setText(text);
        e->setBgColor(getColor());
        e->setCallback(clb);
        registerElement(e);
    };

    auto callbackMake = [&](const std::string& filename) -> std::function<void()> {
        return [&, filename]() {
            systemManager.resetSystems();
            mapLoader.loadMap(filename);
            stateSystem.startGame();
        };
    };

    UILabel* title = new UILabel();
    title->setPos({0, 0, SCREEN_WIDTH, 120});
    title->setText("--== Tower Defence ==--\n\n*** Select a map ***");
    title->setTextColor({0.9f, 0.9f, 0.9f});
    registerElement(title);

    float buttonY = 170;
    make(buttonY, "Map 1", callbackMake("map1.txt"));

    buttonY += buttonHeight + buttonmargin;
    make(buttonY, "Map 2", callbackMake("map2.txt"));

    buttonY += buttonHeight + buttonmargin;
    make(buttonY, "Map 3", callbackMake("map3.txt"));

    buttonY += buttonHeight + buttonmargin;
    make(buttonY, "Map 4", callbackMake("map4.txt"));

    buttonY += buttonHeight * 2 + buttonmargin;
    make(buttonY, "Quit", [window] { glfwSetWindowShouldClose(window, GLFW_TRUE); });
}