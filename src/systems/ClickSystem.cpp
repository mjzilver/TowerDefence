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

    auto* clickables = componentManager.getArray<ClickableComponent>();
    auto* positions = componentManager.getArray<PositionComponent>();
    auto* sizes = componentManager.getArray<SizeComponent>();

    for (Entity entity : clickables->getEntities()) {
        auto* position = positions->get(entity);
        auto* size = sizes->get(entity);
        auto* clickable = clickables->get(entity);

        if (position && size) {
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
    auto* clickables = componentManager.getArray<ClickableComponent>();
    auto* positions = componentManager.getArray<PositionComponent>();
    auto* sizes = componentManager.getArray<SizeComponent>();
    auto* shaders = componentManager.getArray<ShaderComponent>();
    auto* textures = componentManager.getArray<TextureComponent>();

    for (Entity entity : clickables->getEntities()) {
        auto* position = positions->get(entity);
        auto* size = sizes->get(entity);
        auto* clickable = clickables->get(entity);
        auto* texture = textures->get(entity);
        auto* shader = shaders->get(entity);

        if (position && size && texture) {
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
    auto* clickables = componentManager.getArray<ClickableComponent>();

    for (Entity entity : clickables->getEntities()) {
        auto* clickable = clickables->get(entity);
        clickable->selected = false;
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

        Event event;
        event.type = EventType::BUILD_TOWER;
        event.addData("entity", &entity);
        EventDispatcher::getInstance().dispatch(event);
    } else if (event.type == EventType::TOWER_CLICKED && selectedMenuItem == MenuItem::UPGRADE_TOWER) {
        Entity entity = *event.getData<Entity>("entity");

        Event event;
        event.type = EventType::UPGRADE_TOWER;
        event.addData("entity", &entity);
        EventDispatcher::getInstance().dispatch(event);
    }
}
