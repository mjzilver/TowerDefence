#pragma once
#include <algorithm>
#include <map>
#include <queue>
#include <unordered_set>
#include <vector>

#include "../utils/ZLayer.h"
#include "Component.h"

class ComponentManager;

class EntityManager {
public:
    Entity createEntity();

    bool isEntityValid(Entity entity) const { return activeEntities.find(entity) != activeEntities.end(); }

    std::unordered_set<Entity> getEntities() const { return activeEntities; }

    const std::map<ZLayer, std::vector<Entity>>& getLayered() const { return layeredEntities; }

    std::vector<Entity> getEntitiesByLayer(ZLayer layer) const {
        auto it = layeredEntities.find(layer);
        if (it == layeredEntities.end()) return {};
        return it->second;
    }

    std::vector<Entity> getSortedEntities() const;

    void reset() {
        lastEntity = INVALID_ENTITY;
        activeEntities.clear();
        freedEntities = std::queue<Entity>();
    }

    void destroyEntity(Entity entity) {
        activeEntities.erase(entity);

        for (auto& [layer, vec] : layeredEntities) {
            vec.erase(std::remove(vec.begin(), vec.end(), entity), vec.end());
        }

        freedEntities.push(entity);
    }

private:
    Entity lastEntity = INVALID_ENTITY;
    std::unordered_set<Entity> activeEntities;
    std::queue<Entity> freedEntities;
    std::map<ZLayer, std::vector<Entity>> layeredEntities;

    friend class ComponentManager;
};
