#pragma once

#include <functional>
#include <glm/glm.hpp>

#include "../texture/TextureManager.h"
#include "Component.h"
#include "ComponentManager.h"
#include "EntityManager.h"

class EntityFactory {
public:
    EntityFactory(ComponentManager& componentManager, EntityManager& entityManager, TextureManager& textureManager);

    Entity createGrassTile(const glm::vec2& position);
    Entity createPathTile(const glm::vec2& position);

    Entity createStartTile(const glm::vec2& position);
    Entity createEndTile(const glm::vec2& position);

    Entity createFireBug(const glm::vec2& position, int health, int speed, int goldReward);

    Entity createTower(const glm::vec2& position);
    Entity createTowerProjectile(float x, float y, float targetX, float velocityX, float velocityY, float angle, int damage);
    Entity createTowerProjectileImpact(const glm::vec2 position);

    Entity createRectangle(const glm::vec2& position, const glm::vec2& size);

    Entity createUpgradeMenuItem(const glm::vec2& pos, std::function<void()> onClick, int cost);
    Entity createBuildTowerMenuItem(const glm::vec2& pos, std::function<void()> onClick);
    Entity createCurrencyDisplay(const glm::vec2& pos);
    Entity createKillCounter(const glm::vec2& pos);
    Entity createMenuButton(const glm::vec2& pos, std::function<void()> onClick);

    Entity upgradeTower(Entity entity);

private:
    ComponentManager& componentManager;
    EntityManager& entityManager;
    TextureManager& textureManager;
};