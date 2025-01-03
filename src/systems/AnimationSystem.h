#pragma once
#include "../ecs/System.h"
#include "../ecs/ComponentManager.h"
#include "../components/TextureComponent.h"
#include "../components/AnimationComponent.h"

#include "../utils/TextureCoords.h"

class AnimationSystem : public System {
public:
    AnimationSystem(ComponentManager& componentManager) 
        : componentManager(componentManager) {}

    void update(float deltaTime) override {
        for (Entity entity : entities) {
            auto* animation = componentManager.getComponent<AnimationComponent>(entity);
            auto* texture = componentManager.getComponent<TextureComponent>(entity);

            if (animation && texture) {
                animation->time += deltaTime;

                if (animation->time >= animation->frameDuration) {
                    animation->time = 0;
                   
                    animation->frame = (animation->frame + 1) % animation->getFrameCount();
                    
                    texture->coords = getTextureCoords(
                        animation->baseTextureCoords.x + animation->frame,
                        animation->baseTextureCoords.y + animation->getRow(),
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