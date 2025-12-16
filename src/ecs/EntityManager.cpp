#include "EntityManager.h"

Entity EntityManager::createEntity() {
    Entity entity;
    if (!freedEntities.empty()) {
        entity = freedEntities.front();
        freedEntities.pop();
    } else {
        entity = ++lastEntity;
    }

    activeEntities.insert(entity);
    layeredEntities[ZLayer::UNKNOWN].push_back(entity);
    return entity;
}

std::vector<Entity> EntityManager::getSortedEntities() {
    std::vector<Entity> sorted;
    size_t total = 0;
    for (auto& [layer, entities] : layeredEntities) total += entities.size();
    sorted.reserve(total);

    for (auto it = layeredEntities.rbegin(); it != layeredEntities.rend(); ++it) {
        sorted.insert(sorted.end(), it->second.rbegin(), it->second.rend());
    }
    return sorted;
}
