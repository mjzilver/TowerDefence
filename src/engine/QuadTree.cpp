#include "QuadTree.h"
#include <vector>

#include "../components/CollisionComponent.h"
#include "../components/PositionComponent.h"
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

        std::vector<QuadItem> oldEntities = entities;
        entities.clear();

        for (QuadItem q : oldEntities) {
            for (auto& child : children) {
                if (child.insert(q.entity, q.bounds)) break;
            }
        }
    }
}

bool fullyContains(const glm::vec4& outer, const glm::vec4& inner) {
    return inner.x >= outer.x && inner.y >= outer.y && inner.x + inner.z <= outer.x + outer.z && inner.y + inner.w <= outer.y + outer.w;
}

bool QuadTree::insert(Entity entity, const glm::vec4& entityBounds) {
    if (!CollisionSystem::checkCollision(bounds, entityBounds)) {
        return false;
    }

    if (!children.empty()) {
        for (auto& child : children) {
            if (fullyContains(child.bounds, entityBounds)) {
                return child.insert(entity, entityBounds);
            }
        }

        // Overlaps multiple children
        entities.push_back({entity, entityBounds});
        return true;
    }

    entities.push_back({entity, entityBounds});

    if (entities.size() > capacity) {
        subdivide();

        auto old = entities;
        entities.clear();

        for (QuadItem q : old) {
            insert(q.entity, entityBounds);
        }
    }

    return true;
}

void QuadTree::query(const glm::vec4& range, std::vector<QuadItem>& found) {
    if (!CollisionSystem::checkCollision(bounds, range)) return;

    auto* positions = componentManager.getArray<PositionComponent>();
    auto* collisions = componentManager.getArray<CollisionComponent>();

    for (auto q : entities) {
        auto* pos = positions->get(q.entity);
        auto* col = collisions->get(q.entity);
        if (pos && col) {
            glm::vec4 entityBounds{pos->x + col->x, pos->y + col->y, col->w, col->h};
            if (CollisionSystem::checkCollision(range, entityBounds)) {
                found.push_back({q.entity, entityBounds});
            }
        }
    }

    for (auto& child : children) {
        child.query(range, found);
    }
}

std::vector<Entity> QuadTree::query(const glm::vec4& range) {
    std::vector<QuadItem> found;
    query(range, found);
    std::vector<Entity> ret;

    for(auto& q : found) {
        ret.push_back(q.entity);
    }

    return ret;
}

void QuadTree::clear() {
    entities.clear();
    for (auto& child : children) {
        child.clear();
    }
}