#pragma once

#include "ComponentManager.h"
#include "EntityManager.h"
#include "Component.h"
#include "../texture/TextureManager.h"

#include "../components/FlagComponent.h"
#include "SystemManager.h"

#include <glm/glm.hpp>

class EntityFactory {
public:
    EntityFactory(ComponentManager& componentManager, EntityManager& entityManager, TextureManager& textureManager)
        : componentManager(componentManager), entityManager(entityManager), textureManager(textureManager) {}

    Entity createGrassTile(glm::vec2 position);
    Entity createPathTile(glm::vec2 position);

    Entity createStartTile(glm::vec2 position) {
        Entity entity = createPathTile(position);
        FlagComponent flagComponent;
        flagComponent.type = Start;
        componentManager.addComponent(entity, flagComponent);
        return entity;
    }
    Entity createEndTile(glm::vec2 position) {
        Entity entity = createPathTile(position);
        FlagComponent flagComponent;
        flagComponent.type = End;
        componentManager.addComponent(entity, flagComponent);
        return entity;
    }

    Entity createFireBug(glm::vec2 position);

    Entity createTower(glm::vec2 position);
    Entity createTowerProjectile(float x, float y, float targetX, float velocityX, float velocityY, float angle);

private:
    ComponentManager& componentManager;
    EntityManager& entityManager;
    TextureManager& textureManager;
};