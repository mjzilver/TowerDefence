#include "QuadTree.h"

#include <iostream>

#include "../ecs/Component.h"
#include "../systems/CollisionSystem.h"

void QuadTree::subdivide() {
    if (children.size() < 4) {
        const float halfWidth = bounds.z / 2.0f;
        const float halfHeight = bounds.w / 2.0f;

        children.push_back(QuadTree(componentManager, glm::vec4(bounds.x, bounds.y, halfWidth, halfHeight), capacity));
        children.push_back(QuadTree(componentManager, glm::vec4(bounds.x + halfWidth, bounds.y, halfWidth, halfHeight), capacity));
        children.push_back(QuadTree(componentManager, glm::vec4(bounds.x, bounds.y + halfHeight, halfWidth, halfHeight), capacity));
        children.push_back(QuadTree(componentManager, glm::vec4(bounds.x + halfWidth, bounds.y + halfHeight, halfWidth, halfHeight), capacity));

        std::vector<Entity> oldEntities = entities;
        entities.clear();

        for (Entity e : oldEntities) {
            for (auto& child : children) {
                if (child.insert(e)) break;
            }
        }
    }
}

bool fullyContains(const glm::vec4& outer, const glm::vec4& inner) {
    return inner.x >= outer.x && inner.y >= outer.y && inner.x + inner.z <= outer.x + outer.z && inner.y + inner.w <= outer.y + outer.w;
}

bool QuadTree::insert(Entity entity) {
    auto* pos = componentManager.getComponent<PositionComponent>(entity);
    auto* size = componentManager.getComponent<SizeComponent>(entity);

    if (!pos || !size) {
        std::cout << entity << " has no pos or size\n";
        return false;
    };

    glm::vec4 entityBounds{pos->x, pos->y, size->w, size->h};

    if (!CollisionSystem::checkCollision(bounds, entityBounds)) {
        return false;
    }

    if (!children.empty()) {
        for (auto& child : children) {
            if (fullyContains(child.bounds, entityBounds)) {
                return child.insert(entity);
            }
        }

        // Overlaps multiple children
        entities.push_back(entity);
        return true;
    }

    entities.push_back(entity);

    if (entities.size() > capacity) {
        subdivide();

        auto old = entities;
        entities.clear();

        for (Entity e : old) {
            insert(e);
        }
    }

    return true;
}

void QuadTree::query(const glm::vec4& range, std::vector<Entity>& found) {
    if (!CollisionSystem::checkCollision(bounds, range)) return;

    for (auto entity : entities) {
        auto* pos = componentManager.getComponent<PositionComponent>(entity);
        auto* size = componentManager.getComponent<SizeComponent>(entity);
        if (pos && size) {
            if (CollisionSystem::checkCollision(range, {pos->x, pos->y, size->w, size->h})) {
                found.push_back(entity);
            }
        }
    }

    for (auto& child : children) {
        child.query(range, found);
    }
}

std::vector<Entity> QuadTree::query(const PositionComponent* pos, const SizeComponent* size) { return query({pos->x, pos->y, size->w, size->h}); }

std::vector<Entity> QuadTree::query(const glm::vec4& range) {
    std::vector<Entity> found;
    query(range, found);
    return found;
}

void QuadTree::clear() {
    entities.clear();
    children.clear();
}