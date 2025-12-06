#include "ClickSystem.h"

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "../components/ClickableComponent.h"
#include "../components/PositionComponent.h"
#include "../components/ShaderComponent.h"
#include "../components/SizeComponent.h"
#include "../components/TextureComponent.h"
#include "../ecs/ComponentManager.h"
#include "../event/Event.h"
#include "../event/EventDispatcher.h"
#include "CollisionSystem.h"

void ClickSystem::onClick(int button, int action, double x, double y) {
    if (action != GLFW_PRESS) {
        return;
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        Event event;
        event.type = EventType::UNSELECT;
        EventDispatcher::getInstance().dispatch(event);
        return;
    }

    if (button != GLFW_MOUSE_BUTTON_LEFT) {
        return;
    }

    for (Entity entity : getEntities()) {
        auto* position = componentManager.getComponent<PositionComponent>(entity);
        auto* size = componentManager.getComponent<SizeComponent>(entity);
        auto* clickable = componentManager.getComponent<ClickableComponent>(entity);

        if (clickable && position && size) {
            float left = position->x;
            float right = position->x + size->w;
            float top = position->y;
            float bottom = position->y + size->h;

            if (CollisionSystem::checkCollision(x, y, CLICK_SIZE, CLICK_SIZE, left, top, right - left, bottom - top)) {
                Event event;
                event.type = clickable->clickedEvent;
                event.addData("entity", &entity);
                EventDispatcher::getInstance().dispatch(event);
            }
        }
    }
}

void ClickSystem::onHover(double x, double y) {
    for (Entity entity : getEntities()) {
        auto* position = componentManager.getComponent<PositionComponent>(entity);
        auto* size = componentManager.getComponent<SizeComponent>(entity);
        auto* texture = componentManager.getComponent<TextureComponent>(entity);
        auto* clickable = componentManager.getComponent<ClickableComponent>(entity);
        auto* shader = componentManager.getComponent<ShaderComponent>(entity);

        if (clickable && position && size && texture) {
            clickable->hovered = false;
            shader->name = "default";

            float left = position->x;
            float right = position->x + size->w;
            float top = position->y;
            float bottom = position->y + size->h;

            if (CollisionSystem::checkCollision(x, y, CLICK_SIZE, CLICK_SIZE, left, top, right - left, bottom - top)) {
                clickable->hovered = true;
                shader->name = "hover";
            }
        }
    }
}

void ClickSystem::onEvent(const Event& event) {
    for (Entity entity : getEntities()) {
        auto* clickable = componentManager.getComponent<ClickableComponent>(entity);
        if (clickable) {
            clickable->selected = false;
        }
    }

    if (event.type == EventType::UNSELECT) {
        selectedMenuItem = MenuItem::NONE;
    } else if (event.type == EventType::UPGRADE_MENU_ITEM_CLICKED) {
        Entity entity = *event.getData<Entity>("entity");
        auto* clickable = componentManager.getComponent<ClickableComponent>(entity);

        clickable->selected = true;
        selectedMenuItem = MenuItem::UPGRADE_TOWER;
    } else if (event.type == EventType::BUILD_TOWER_MENU_ITEM_CLICKED) {
        Entity entity = *event.getData<Entity>("entity");
        auto* clickable = componentManager.getComponent<ClickableComponent>(entity);

        clickable->selected = true;
        selectedMenuItem = MenuItem::BUILD_TOWER;
    } else if (event.type == EventType::GRASS_TILE_CLICKED && selectedMenuItem == MenuItem::BUILD_TOWER) {
        Entity entity = *event.getData<Entity>("entity");
        auto* position = componentManager.getComponent<PositionComponent>(entity);
        auto* size = componentManager.getComponent<SizeComponent>(entity);

        entityFactory.createTower({position->x, position->y - size->h + 16});
    } else if (event.type == EventType::TOWER_CLICKED && selectedMenuItem == MenuItem::UPGRADE_TOWER) {
        Entity entity = *event.getData<Entity>("entity");
    }
}
