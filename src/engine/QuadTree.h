#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "../ecs/Component.h"
#include "../ecs/ComponentManager.h"

struct QuadItem {
    Entity entity;
    glm::vec4 bounds;
};

class QuadTree {
public:
    QuadTree(ComponentManager& componentManager, const glm::vec4& bounds, int capacity = 8)
        : bounds(bounds), capacity(capacity), componentManager(componentManager) {}

    ~QuadTree() { children.clear(); }

    void subdivide();

    bool insert(Entity entity, const glm::vec4& entityBounds);

    std::vector<Entity> query(const glm::vec4& range);

    void clear();

private:
    void query(const glm::vec4& range, std::vector<QuadItem>& found);

    ComponentManager& componentManager;

    glm::vec4 bounds;
    int capacity;
    std::vector<QuadItem> entities;
    std::vector<QuadTree> children;
};