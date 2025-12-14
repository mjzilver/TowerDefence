#pragma once

#include <queue>
#include <typeindex>
#include <unordered_map>

#include "Component.h"
#include "ComponentArray.h"
#include "EntityManager.h"
#include "../components/PositionComponent.h"

class ComponentManager {
public:
    ComponentManager(EntityManager& em)
        : em(em) {}

    template <typename T>
    void addComponent(Entity entity, T component) {
        getArray<T>()->insert(entity, std::move(component));
    }

    void addComponent(Entity entity, PositionComponent pos) {
        getArray<PositionComponent>()->insert(entity, std::move(pos));
        em.reorder(pos.zIndex, entity);
    }

    template <typename T>
    T* getComponent(Entity entity) {
        return getArray<T>()->get(entity);
    }

    template <typename T>
    void removeComponent(Entity entity) {
        getArray<T>()->remove(entity);
    }

    template <typename T>
    ComponentArray<T>* getArray() {
        auto typeIndex = std::type_index(typeid(T));
        if (componentArrays.find(typeIndex) == componentArrays.end()) {
            componentArrays[typeIndex] = new ComponentArray<T>();
        }
        return static_cast<ComponentArray<T>*>(componentArrays[typeIndex]);
    }

    void scheduleDestruction(Entity entity) { scheduledForDestruction.push(entity); }

    void flushDestructions(EntityManager& entityManager) {
        while (!scheduledForDestruction.empty()) {
            Entity entity = scheduledForDestruction.front();
            scheduledForDestruction.pop();

            for (auto& [type, array] : componentArrays) {
                array->remove(entity);
            }

            entityManager.destroyEntity(entity);
        }
    }

    void destroyAll() {
        for (auto& [type, array] : componentArrays) {
            array->clear();
        }
    }

    ~ComponentManager() {
        for (auto& [type, array] : componentArrays) {
            delete array;
        }
    }

private:
    EntityManager& em;

    std::unordered_map<std::type_index, IComponentArray*> componentArrays;
    std::queue<Entity> scheduledForDestruction;
};
