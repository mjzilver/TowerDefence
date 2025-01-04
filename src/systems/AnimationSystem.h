#pragma once
#include "../ecs/System.h"
#include "../ecs/ComponentManager.h"
#include "../components/TextureComponent.h"
#include "../components/AnimationComponent.h"
#include "../components/DirectionComponent.h"
#include "../components/VelocityComponent.h"

#include "../utils/TextureCoords.h"

class AnimationSystem : public System {
public:
    AnimationSystem(ComponentManager& componentManager) 
        : componentManager(componentManager) {}

    void update(float deltaTime) override {
        for (Entity entity : entities) {
            auto* animation = componentManager.getComponent<AnimationComponent>(entity);
            auto* texture = componentManager.getComponent<TextureComponent>(entity);
            auto* direction = componentManager.getComponent<DirectionComponent>(entity);
            auto* velocity = componentManager.getComponent<VelocityComponent>(entity);

            if (animation && texture) {
                animation->time += deltaTime;

                if (velocity && (velocity->x != 0 || velocity->y != 0)) {
                    animation->state = State::Walking;
                } else {
                    animation->state = State::Idle;
                }

                if (animation->time >= animation->frameDuration) {
                    animation->time = 0;
                   
                    animation->frame = (animation->frame + 1) % animation->getFrameCount();
                    
                    texture->coords = getTextureCoords(
                        animation->baseTextureCoords.x + animation->frame,
                        animation->baseTextureCoords.y + animation->getRow(direction->direction),
                        animation->frameSize.x,
                        animation->frameSize.y,
                        texture->texture.size.x,
                        texture->texture.size.y
                    );
                }
            }
        }
    }

private:
    ComponentManager& componentManager;
};