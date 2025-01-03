#pragma once

#include "../ecs/ComponentManager.h"
#include "../ecs/EntityManager.h"
#include "../ecs/Component.h"
#include "../texture/TextureManager.h"

#include <glm/glm.hpp>


class EntityFactory {
public:
    EntityFactory(ComponentManager& componentManager, EntityManager& entityManager) 
        : componentManager(componentManager), entityManager(entityManager) {}

    Entity createGrassTile(glm::vec2 position, TextureManager& textureManager);
    Entity createPathTile(glm::vec2 position, TextureManager& textureManager);
    Entity createStartTile(glm::vec2 position, TextureManager& textureManager) {
        return createPathTile(position, textureManager);
    }
    Entity createEndTile(glm::vec2 position, TextureManager& textureManager) {
        return createPathTile(position, textureManager);
    }

private:
    ComponentManager& componentManager;
    EntityManager& entityManager;
};