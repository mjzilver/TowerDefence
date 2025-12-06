#include "AnimationSystem.h"

#include <iostream>
#include <ostream>

#include "../components/AnimationComponent.h"
#include "../components/DirectionComponent.h"
#include "../components/TextureComponent.h"
#include "../utils/TextureCoords.h"

void AnimationSystem::update(float deltaTime) {
    auto* animations = componentManager.getArray<AnimationComponent>();
    auto* directions = componentManager.getArray<DirectionComponent>();
    auto* textures = componentManager.getArray<TextureComponent>();

    for (Entity entity : animations->getEntities()) {
        auto* animation = animations->get(entity);
        auto* texture = textures->get(entity);
        auto* direction = directions->get(entity);

        if (texture) {
            animation->time += deltaTime;

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
                    } else if (animation->removeAtEnd) {
                       componentManager.scheduleDestruction( entity);
                    } else if (animation->state != State::DEAD) {
                        animation->state = State::IDLE;
                        animation->frame = 0;
                    }
                }

                if (direction) {
                    texture->coords = getTextureCoords(
                        animation->baseTextureCoords.x + animation->frame, animation->baseTextureCoords.y + animation->getRow(direction->direction),
                        animation->frameSize.x, animation->frameSize.y, texture->texture.size.x, texture->texture.size.y);
                } else {
                    texture->coords =
                        getTextureCoords(animation->baseTextureCoords.x + animation->frame, animation->baseTextureCoords.y, animation->frameSize.x,
                                         animation->frameSize.y, texture->texture.size.x, texture->texture.size.y);
                }
            }
        }
    }
}

void AnimationSystem::onEvent(const Event& event) {
    if (event.type == EventType::ENTITY_DESTROYED) {
        Entity entity = *event.getData<Entity>("entity");
        auto* animation = componentManager.getComponent<AnimationComponent>(entity);

        if (animation) {
            animation->state = State::DEAD;
            animation->loop = false;
            animation->frame = 0;
        }
    }
}
