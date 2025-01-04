#pragma once

#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>

#include "../ecs/System.h"
#include "../ecs/ComponentManager.h"
#include "../ecs/Component.h"

class PathfindingSystem : public System {
public:
    PathfindingSystem(ComponentManager& componentManager) : componentManager(componentManager) {}

    void update(float deltaTime) override;
    void generatePath();

private:
    std::vector<Entity> pathTiles;

    Entity start;
    Entity end;

    ComponentManager& componentManager;
};
