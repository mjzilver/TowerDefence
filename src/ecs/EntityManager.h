#pragma once
#include "Component.h"
#include <queue>
#include <unordered_set>

class ComponentManager;

class EntityManager {
public:
    Entity createEntity() {
        Entity entity;
        if(!freedEntities.empty()) {
            entity = freedEntities.front();
            freedEntities.pop();
        } else {
            entity = ++lastEntity;
        }
        
        activeEntities.insert(entity);
        return entity;
    }

    bool isEntityValid(Entity entity) const {
        return activeEntities.find(entity) != activeEntities.end();
    }

    std::unordered_set<Entity> getEntities() const {
        return activeEntities;
    }

    void reset(){
        lastEntity = INVALID_ENTITY;
        activeEntities.clear();
        freedEntities = std::queue<Entity>();
    }

private:
    void destroyEntity(Entity entity) {
        activeEntities.erase(entity);
        freedEntities.push(entity);
    }

    Entity lastEntity = INVALID_ENTITY;
    std::unordered_set<Entity> activeEntities;
    std::queue<Entity> freedEntities;

    friend class ComponentManager;
};
