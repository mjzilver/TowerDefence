#pragma once
#include "Component.h"
#include <unordered_map>
#include <unordered_set>

class EntityManager {
public:
    Entity createEntity() {
        Entity entity = ++lastEntity;
        activeEntities.insert(entity);
        return entity;
    }

    void destroyEntity(Entity entity) {
        activeEntities.erase(entity);
    }

    bool isEntityValid(Entity entity) const {
        return activeEntities.find(entity) != activeEntities.end();
    }

    std::unordered_set<Entity> getEntities() const {
        return activeEntities;
    }

private:
    Entity lastEntity = INVALID_ENTITY;
    std::unordered_set<Entity> activeEntities;
};