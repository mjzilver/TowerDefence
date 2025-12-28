#include "ClickSystem.h"

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <functional>

#include "../components/ClickableComponent.h"
#include "../components/PositionComponent.h"
#include "../components/ShaderComponent.h"
#include "../components/SizeComponent.h"
#include "../components/TextureComponent.h"
#include "../ecs/ComponentManager.h"
#include "../event/Event.h"
#include "../event/EventDispatcher.h"
#include "CollisionSystem.h"

ClickSystem::ClickSystem(EngineContext& ctx) : System(ctx) {
    writes.push_back(typeid(ShaderComponent));
    writes.push_back(typeid(ClickableComponent));

    reads.push_back(typeid(PositionComponent));
    reads.push_back(typeid(SizeComponent));
    reads.push_back(typeid(TextureComponent));
}

void ClickSystem::onClick(int button, int action, double x, double y) {
    if (action != GLFW_PRESS) {
        return;
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        Event event;
        event.type = EventType::UNSELECT;
        context.eventDispatcher.dispatch(event);
        return;
    }

    if (button != GLFW_MOUSE_BUTTON_LEFT) {
        return;
    }

    auto& componentManager = context.componentManager;

    auto* clickables = componentManager.getArray<ClickableComponent>();
    const auto* positions = componentManager.getArray<PositionComponent>();
    const auto* sizes = componentManager.getArray<SizeComponent>();

    for (Entity entity : clickables->getEntities()) {
        const auto* position = positions->get(entity);
        const auto* size = sizes->get(entity);
        const auto* clickable = clickables->get(entity);

        if (position && size) {
            float left = position->x;
            float right = position->x + size->w;
            float top = position->y;
            float bottom = position->y + size->h;

            if (CollisionSystem::contains(left, top, right - left, bottom - top, x, y)) {
                if (clickable->type == ClickableType::EVENT) {
                    Event event;
                    event.type = clickable->clickedEvent;
                    event.addEntity("entity", entity);
                    context.eventDispatcher.dispatch(event);
                } else if (clickable->type == ClickableType::FUNCTION) {
                    clickable->onClick();
                }
            }
        }
    }
}

void ClickSystem::onHover(double x, double y) {
    auto& componentManager = context.componentManager;
    auto* clickables = componentManager.getArray<ClickableComponent>();
    const auto* positions = componentManager.getArray<PositionComponent>();
    const auto* sizes = componentManager.getArray<SizeComponent>();
    const auto* shaders = componentManager.getArray<ShaderComponent>();
    const auto* textures = componentManager.getArray<TextureComponent>();

    for (Entity entity : clickables->getEntities()) {
        const auto* position = positions->get(entity);
        const auto* size = sizes->get(entity);
        auto* clickable = clickables->get(entity);
        const auto* texture = textures->get(entity);
        auto* shader = shaders->get(entity);

        if (position && size && texture && clickable && shader) {
            clickable->hovered = false;
            shader->name = "default";

            float left = position->x;
            float right = position->x + size->w;
            float top = position->y;
            float bottom = position->y + size->h;

            if (CollisionSystem::contains(left, top, right - left, bottom - top, x, y)) {
                clickable->hovered = true;
                shader->name = "hover";
            }
        }
    }
}
