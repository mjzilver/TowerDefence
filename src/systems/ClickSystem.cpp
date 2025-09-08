#include "ClickSystem.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
    if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS) {
        return;
    }

    for (Entity entity : getEntities()) {
        auto* position = componentManager.getComponent<PositionComponent>(entity);
        auto* size = componentManager.getComponent<SizeComponent>(entity);
        auto* texture = componentManager.getComponent<TextureComponent>(entity);
        auto* clickable = componentManager.getComponent<ClickableComponent>(entity);

        if (clickable && position && size && texture) {
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
            clickable->selected = false;
            shader->name = "default";

            float left = position->x;
            float right = position->x + size->w;
            float top = position->y;
            float bottom = position->y + size->h;

            if (CollisionSystem::checkCollision(x, y, CLICK_SIZE, CLICK_SIZE, left, top, right - left, bottom - top)) {
                clickable->selected = true;
                shader->name = "hover";
            }
        }
    }
}