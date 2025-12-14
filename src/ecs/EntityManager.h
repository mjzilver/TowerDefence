#pragma once
#include <map>
#include <queue>
#include <unordered_set>
#include <vector>

#include "../utils/ZLayer.h"
#include "Component.h"

class ComponentManager;

class EntityManager {
public:
    Entity createEntity() {
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

    bool isEntityValid(Entity entity) const { return activeEntities.find(entity) != activeEntities.end(); }

    std::unordered_set<Entity> getEntities() const { return activeEntities; }

    const std::map<ZLayer, std::vector<Entity>>& getLayered() const {
        return layeredEntities;
    }

    std::vector<Entity> getEntitiesByLayer(ZLayer layer) const {
        auto it = layeredEntities.find(layer);
        if (it == layeredEntities.end()) return {};
        return it->second;
    }

    std::vector<Entity> getSortedEntities() {
        std::vector<Entity> sorted;
        size_t total = 0;
        for (auto& [layer, entities] : layeredEntities) total += entities.size();
        sorted.reserve(total);

        for (auto it = layeredEntities.rbegin(); it != layeredEntities.rend(); ++it) {
            sorted.insert(sorted.end(), it->second.rbegin(), it->second.rend());
        }
        return sorted;
    }

    void reset() {
        lastEntity = INVALID_ENTITY;
        activeEntities.clear();
        freedEntities = std::queue<Entity>();
    }

private:
    void destroyEntity(Entity entity) {
        activeEntities.erase(entity);
        freedEntities.push(entity);
    }

    Entity lastEntity = INVALID_ENTITY;
    std::unordered_set<Entity> activeEntities;
    std::queue<Entity> freedEntities;
    std::map<ZLayer, std::vector<Entity>> layeredEntities;

    friend class ComponentManager;
};
