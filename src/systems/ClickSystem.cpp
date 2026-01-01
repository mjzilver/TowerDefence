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
#include "../event/Event.h"
#include "../event/EventDispatcher.h"
#include "CollisionSystem.h"

ClickSystem::ClickSystem(EngineContext& ctx) : System(ctx, "ClickSystem") {
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

    const auto* clickables = readArray<ClickableComponent>();
    const auto* positions = readArray<PositionComponent>();
    const auto* sizes = readArray<SizeComponent>();

    for (Entity entity : clickables->getEntities()) {
        const auto* position = positions->get(entity);
        const auto* size = sizes->get(entity);
        const auto* clickable = clickables->get(entity);

        if (clickable && position && size) {
            const float left = position->x;
            const float right = position->x + size->w;
            const float top = position->y;
            const float bottom = position->y + size->h;

            if (CollisionSystem::contains(left, top, right - left, bottom - top, x, y)) {
                std::lock_guard<std::mutex> lock(clickQueueMutex);
                clickQueue.push_back({entity, ClickEventType::CLICK_EVENT});
            }
        }
    }
}

void ClickSystem::onHover(double x, double y) {
    const auto* clickables = readArray<ClickableComponent>();
    const auto* positions = readArray<PositionComponent>();
    const auto* sizes = readArray<SizeComponent>();
    const auto* shaders = readArray<ShaderComponent>();
    const auto* textures = readArray<TextureComponent>();

    for (Entity entity : clickables->getEntities()) {
        const auto* position = positions->get(entity);
        const auto* size = sizes->get(entity);
        const auto* clickable = clickables->get(entity);
        const auto* texture = textures->get(entity);
        const auto* shader = shaders->get(entity);

        if (position && size && texture && clickable && shader) {
            const float left = position->x;
            const float right = position->x + size->w;
            const float top = position->y;
            const float bottom = position->y + size->h;

            if (CollisionSystem::contains(left, top, right - left, bottom - top, x, y)) {
                std::lock_guard<std::mutex> lock(clickQueueMutex);
                clickQueue.push_back({
                    entity,
                    ClickEventType::HOVER_EVENT,
                });
            }
        }
    }
}

void ClickSystem::update(float) {
    auto* clickables = writeArray<ClickableComponent>();
    auto* shaders = writeArray<ShaderComponent>();
    const auto* textures = readArray<TextureComponent>();

    bool hasHover = false;
    {
        std::lock_guard<std::mutex> lock(clickQueueMutex);
        for (auto& evt : clickQueue) {
            if (evt.type == ClickEventType::HOVER_EVENT) {
                hasHover = true;
                break;
            }
        }
    }

    if (hasHover) {
        for (Entity entity : clickables->getEntities()) {
            auto* clickable = clickables->get(entity);
            auto* shader = shaders->get(entity);
            const auto* texture = textures->get(entity);

            if (!clickable || !shader || !texture) continue;

            clickable->hovered = false;
            shader->name = "default";
        }
    }

    std::lock_guard<std::mutex> lock(clickQueueMutex);

    while (!clickQueue.empty()) {
        auto cur = clickQueue.back();
        clickQueue.pop_back();

        auto* clickable = clickables->get(cur.entity);
        auto* shader = shaders->get(cur.entity);

        if (cur.type == ClickEventType::CLICK_EVENT) {
            if (clickable->type == ClickableType::EVENT) {
                Event event;
                event.type = clickable->clickedEvent;
                event.addEntity("entity", cur.entity);
                context.eventDispatcher.dispatch(event);
            } else if (clickable->type == ClickableType::FUNCTION) {
                clickable->onClick();
            }
        } else if (cur.type == ClickEventType::HOVER_EVENT) {
            clickable->hovered = true;
            shader->name = "hover";
        }
    }
}