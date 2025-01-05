#pragma once

#include "../components/AnimationComponent.h"
#include "../components/DirectionComponent.h"
#include "../components/HealthComponent.h"
#include "../components/PathfindingComponent.h"
#include "../components/TextureComponent.h"
#include "../components/VelocityComponent.h"
#include "../ecs/ComponentManager.h"
#include "../ecs/System.h"
#include "../event/Event.h"
#include "../event/EventDispatcher.h"
#include "../utils/TextureCoords.h"

class ClickSystem : public System {
public:
    ClickSystem(ComponentManager& componentManager) : componentManager(componentManager) {}

    void update(float deltaTime) override { return; }

    void onClick(int button, int action, double x, double y) {
        if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS) {
            return;
        }

        for (Entity entity : getEntities()) {
            auto* position = componentManager.getComponent<PositionComponent>(entity);
            auto* size = componentManager.getComponent<SizeComponent>(entity);
            auto* texture = componentManager.getComponent<TextureComponent>(entity);
            // Add ClickableComponent

            if (position && size && texture) {
                float left = position->x;
                float right = position->x + size->w;
                float top = position->y;
                float bottom = position->y + size->h;

                const int CLICK_SIZE = 2;

                if (CollisionSystem::checkCollision(x, y, CLICK_SIZE, CLICK_SIZE, left, top, right - left, bottom - top)) {
                    std::cout << "Clicked on entity " << entity << std::endl;

                    Event event;
                    event.type = EventType::EntityClicked;
                    event.addData("entity", &entity);
                    EventDispatcher::getInstance().dispatch(event);
                }
            }
        }
    }

private:
    ComponentManager& componentManager;
};