#pragma once

#include "ComponentManager.h"
#include "EntityManager.h"
#include "Component.h"
#include "../texture/TextureManager.h"

#include "../components/FlagComponent.h"

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
        flagComponent.type = START;
        componentManager.addComponent(entity, flagComponent);
        return entity;
    }
    
    Entity createEndTile(glm::vec2 position) {
        Entity entity = createPathTile(position);
        FlagComponent flagComponent;
        flagComponent.type = END;
        componentManager.addComponent(entity, flagComponent);
        return entity;
    }

    Entity createFireBug(glm::vec2 position);

    Entity createTower(glm::vec2 position);
    Entity createTowerProjectile(float x, float y, float targetX, float velocityX, float velocityY, float angle, int damage);

    Entity createUpgradeMenuItem(glm::vec2 offset);
    Entity createBuildTowerMenuItem(glm::vec2 offset);
    Entity createCurrencyDisplay(glm::vec2 offset);

    Entity upgradeTower(Entity& entity);

private:
    ComponentManager& componentManager;
    EntityManager& entityManager;
    TextureManager& textureManager;
};