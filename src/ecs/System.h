#pragma once

#include <unordered_set>

#include "Component.h"
#include "EntityManager.h"

class System {
public:
    virtual ~System() = default;
    virtual void update(float deltaTime) = 0;

    std::unordered_set<Entity> getEntities() {
        return this->entityManager->getEntities();
    }

    void SetEntityManager(EntityManager* entityManager) {
        this->entityManager = entityManager;
    }

protected:
    EntityManager* entityManager;
};