#pragma once

#include <unordered_set>

#include "Component.h"

class System {
public:
    virtual ~System() = default;
    virtual void update(float deltaTime) = 0;

    void addEntity(Entity entity) { entities.insert(entity); }
    void removeEntity(Entity entity) { entities.erase(entity); }

protected:
    std::unordered_set<Entity> entities;
};