#pragma once

#include "ComponentManager.h"
#include "EntityManager.h"
#include "Component.h"
#include "../texture/TextureManager.h"

#include <functional>
#include <glm/glm.hpp>

class EntityFactory {
public:
    EntityFactory(ComponentManager& componentManager, EntityManager& entityManager, TextureManager& textureManager)
        : componentManager(componentManager), entityManager(entityManager), textureManager(textureManager) {}

    Entity createGrassTile(glm::vec2 position);
    Entity createPathTile(glm::vec2 position);

    Entity createStartTile(glm::vec2 position);
    Entity createEndTile(glm::vec2 position);

    Entity createFireBug(glm::vec2 position, int health, int speed, int goldReward);

    Entity createTower(glm::vec2 position);
    Entity createTowerProjectile(float x, float y, float targetX, float velocityX, float velocityY, float angle, int damage);
    Entity createTowerProjectileImpact(glm::vec2 position);

    Entity createRectangle(const glm::vec2& position, const glm::vec2& size);

    Entity createUpgradeMenuItem(glm::vec2 pos, std::function<void()> onClick);
    Entity createBuildTowerMenuItem(glm::vec2 pos, std::function<void()> onClick);
    Entity createCurrencyDisplay(glm::vec2 pos);
    Entity createKillCounter(glm::vec2 pos);
    Entity createMenuButton(glm::vec2 pos, std::function<void()> onClick);

    Entity upgradeTower(Entity& entity);

private:
    ComponentManager& componentManager;
    EntityManager& entityManager;
    TextureManager& textureManager;
};