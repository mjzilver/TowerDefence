#pragma once

#include "EntityManager.h"

class System {
public:
    virtual ~System() = default;
    virtual void update(float deltaTime) = 0;
    virtual void reset() = 0;

    void setEntityManager(EntityManager* entityManager) {
        this->entityManager = entityManager;
    }

protected:
    EntityManager* entityManager;
};