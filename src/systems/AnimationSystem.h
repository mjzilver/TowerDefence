#pragma once

#include "../components/AnimationComponent.h"
#include "../components/DirectionComponent.h"
#include "../components/TextureComponent.h"
#include "../components/VelocityComponent.h"
#include "../components/PathfindingComponent.h"
#include "../components/HealthComponent.h"
#include "../ecs/ComponentManager.h"
#include "../ecs/System.h"
#include "../utils/TextureCoords.h"

#include "../event/Event.h"
#include "../event/EventDispatcher.h"

class AnimationSystem : public System {
public:
    AnimationSystem(ComponentManager& componentManager) : componentManager(componentManager) {
        EventDispatcher::getInstance().addListener(EventType::EntityDestroyed, std::bind(&AnimationSystem::onEvent, this, std::placeholders::_1));
    }

    void update(float deltaTime) override {
        for (Entity entity : getEntities()) {
            auto* animation = componentManager.getComponent<AnimationComponent>(entity);
            auto* texture = componentManager.getComponent<TextureComponent>(entity);
            auto* direction = componentManager.getComponent<DirectionComponent>(entity);
            auto* velocity = componentManager.getComponent<VelocityComponent>(entity);

            if (animation && texture) {
                animation->time += deltaTime;

                // TODO: move this to a separate system (StateSystem)
                if (velocity) {
                    if (velocity->x != 0 || velocity->y != 0) {
                        animation->state = State::Active;
                    } else {
                        animation->state = State::Idle;
                    }
                }

                if (animation->time >= animation->frameDuration) {
                    animation->time = 0;

                    if (animation->loop) {
                        if (animation->getFrameCount() > 0) {
                            animation->frame = (animation->frame + 1) % animation->getFrameCount();
                        } else {
                            std::cerr << "No frame count for state " << static_cast<int>(animation->state) << std::endl;
                        }
                    } else {
                        if (animation->frame < animation->getFrameCount() - 1) {
                            animation->frame++;
                        } else if (animation->state != State::Dead) {
                            animation->state = State::Idle;
                            animation->frame = 0;
                        }
                    }

                    if (direction) {
                        texture->coords =
                            getTextureCoords(animation->baseTextureCoords.x + animation->frame,
                                             animation->baseTextureCoords.y + animation->getRow(direction->direction), animation->frameSize.x,
                                             animation->frameSize.y, texture->texture.size.x, texture->texture.size.y);
                    } else {
                        texture->coords =
                            getTextureCoords(animation->baseTextureCoords.x + animation->frame, animation->baseTextureCoords.y,
                                             animation->frameSize.x, animation->frameSize.y, texture->texture.size.x, texture->texture.size.y);
                    }
                }
            }
        }
    }

    void onEvent(const Event& event) {
        if (event.type == EventType::EntityDestroyed) {
            Entity entity = *event.getData<Entity>("entity");
            auto* animation = componentManager.getComponent<AnimationComponent>(entity);
            auto* velocity = componentManager.getComponent<VelocityComponent>(entity);
            auto* pathfinding = componentManager.getComponent<PathfindingComponent>(entity);
            auto* health = componentManager.getComponent<HealthComponent>(entity);

            if (animation) {
                animation->state = State::Dead;
                animation->loop = false;
                animation->frame = 0;
            }
        }
    }

private:
    ComponentManager& componentManager;
};