#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "../ecs/Component.h"
#include "../ecs/ComponentManager.h"
#include "../components/PositionComponent.h"
#include "../components/SizeComponent.h"

class QuadTree {
public:
    QuadTree(ComponentManager& componentManager, const glm::vec4& bounds, int capacity = 8)
        : bounds(bounds), capacity(capacity), componentManager(componentManager) {}

    ~QuadTree() {
        children.clear();
    }

    void subdivide();

    bool insert(Entity entity);

    std::vector<Entity> query(const glm::vec4& range);
    std::vector<Entity> query(const PositionComponent* pos, const SizeComponent* size);

    void clear();

private:
    void query(const glm::vec4& range, std::vector<Entity>& found);

    ComponentManager& componentManager;

    glm::vec4 bounds;
    int capacity;
    std::vector<Entity> entities;
    std::vector<QuadTree> children;
};