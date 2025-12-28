#include "QuadTree.h"

#include "../components/CollisionComponent.h"
#include "../components/PositionComponent.h"
#include "../systems/CollisionSystem.h"

QuadTree::QuadTree(ComponentManager& componentManager, const glm::vec4& bounds, int capacity)
    : componentManager(componentManager), bounds(bounds), capacity(capacity) {
    entities.reserve(capacity);
    children.reserve(4);
}

QuadTree::~QuadTree() { clear(); }

static bool fullyContains(const glm::vec4& outer, const glm::vec4& inner) {
    return inner.x >= outer.x && inner.y >= outer.y && inner.x + inner.z <= outer.x + outer.z && inner.y + inner.w <= outer.y + outer.w;
}

void QuadTree::subdivide() {
    if (!children.empty()) return;

    const float halfWidth = bounds.z * 0.5f;
    const float halfHeight = bounds.w * 0.5f;

    children.push_back(new QuadTree(componentManager, glm::vec4(bounds.x, bounds.y, halfWidth, halfHeight), capacity));
    children.push_back(new QuadTree(componentManager, glm::vec4(bounds.x + halfWidth, bounds.y, halfWidth, halfHeight), capacity));
    children.push_back(new QuadTree(componentManager, glm::vec4(bounds.x, bounds.y + halfHeight, halfWidth, halfHeight), capacity));
    children.push_back(new QuadTree(componentManager, glm::vec4(bounds.x + halfWidth, bounds.y + halfHeight, halfWidth, halfHeight), capacity));

    auto oldEntities = std::move(entities);
    entities.clear();

    for (const QuadItem& q : oldEntities) {
        bool inserted = false;
        for (QuadTree* child : children) {
            if (fullyContains(child->bounds, q.bounds)) {
                child->insert(q.entity, q.bounds);
                inserted = true;
                break;
            }
        }
        if (!inserted) {
            entities.push_back(q);
        }
    }
}

bool QuadTree::insert(Entity entity, const glm::vec4& entityBounds) {
    if (!CollisionSystem::checkCollision(bounds, entityBounds)) return false;

    if (!children.empty()) {
        for (QuadTree* child : children) {
            if (fullyContains(child->bounds, entityBounds)) {
                return child->insert(entity, entityBounds);
            }
        }
        entities.push_back({entity, entityBounds});
        return true;
    }

    entities.push_back({entity, entityBounds});

    if (entities.size() > static_cast<size_t>(capacity)) {
        subdivide();
    }

    return true;
}

void QuadTree::query(const glm::vec4& range, std::vector<QuadItem>& found) {
    if (!CollisionSystem::checkCollision(bounds, range)) return;

    const auto* positions = componentManager.getArray<PositionComponent>();
    const auto* collisions = componentManager.getArray<CollisionComponent>();

    for (const QuadItem& q : entities) {
        const auto* pos = positions->get(q.entity);
        const auto* col = collisions->get(q.entity);

        if (!pos || !col) continue;

        glm::vec4 entityBounds{pos->x + col->x, pos->y + col->y, col->w, col->h};

        if (CollisionSystem::checkCollision(range, entityBounds)) {
            found.push_back({q.entity, entityBounds});
        }
    }

    for (QuadTree* child : children) {
        child->query(range, found);
    }
}

std::vector<Entity> QuadTree::query(const glm::vec4& range) {
    std::vector<QuadItem> found;
    query(range, found);

    std::vector<Entity> result;
    result.reserve(found.size());

    for (const QuadItem& q : found) {
        result.push_back(q.entity);
    }

    return result;
}

void QuadTree::clear() {
    entities.clear();

    for (QuadTree* child : children) {
        delete child;
    }
    children.clear();
}
