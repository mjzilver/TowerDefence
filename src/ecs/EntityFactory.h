#pragma once

#include "ComponentManager.h"
#include "EntityManager.h"
#include "Component.h"
#include "../texture/TextureManager.h"

#include "../components/FlagComponent.h"

#include <glm/glm.hpp>


class EntityFactory {
public:
    EntityFactory(ComponentManager& componentManager, EntityManager& entityManager) 
        : componentManager(componentManager), entityManager(entityManager) {}

    Entity createGrassTile(glm::vec2 position, TextureManager& textureManager);
    Entity createPathTile(glm::vec2 position, TextureManager& textureManager);

    Entity createStartTile(glm::vec2 position, TextureManager& textureManager) {
        Entity entity = createPathTile(position, textureManager);
        FlagComponent flagComponent;
        flagComponent.type = Start;
        componentManager.addComponent(entity, flagComponent);
        return entity;
    }
    Entity createEndTile(glm::vec2 position, TextureManager& textureManager) {
        Entity entity = createPathTile(position, textureManager);
        FlagComponent flagComponent;
        flagComponent.type = End;
        componentManager.addComponent(entity, flagComponent);
        return entity;
    }

    Entity createFireBug(glm::vec2 position, TextureManager& textureManager);

private:
    ComponentManager& componentManager;
    EntityManager& entityManager;
};