#pragma once

#include <functional>
#include <typeindex>
#include <unordered_map>

#include "../components/PositionComponent.h"
#include "Component.h"
#include "ComponentArray.h"
#include "EntityManager.h"

class ComponentManager {
public:
    ComponentManager(EntityManager& em) : em(em) {}

    template <typename T>
    void addComponent(Entity e, T component) {
        commands.push_back(
            [=, c = std::move(component)](ComponentManager& cm, EntityManager&) mutable { cm.getArray<T>()->insert(e, std::move(c)); });
    }

    void addComponent(Entity e, PositionComponent pos) {
        commands.push_back([=, c = std::move(pos)](ComponentManager& cm, EntityManager&) mutable {
            cm.getArray<PositionComponent>()->insert(e, std::move(c));
            reorder(e, pos.zIndex);
        });
    }

    template <typename T>
    void removeComponent(Entity e) {
        commands.push_back([=](ComponentManager& cm, EntityManager&) { cm.getArray<T>()->remove(e); });
    }

    void scheduleDestruction(Entity e) {
        commands.push_back([=](ComponentManager& cm, EntityManager& em) { cm.destroyEntityAndComponents(e, em); });
    }

    template <typename T>
    T* getComponent(Entity entity) {
        return getArray<T>()->get(entity);
    }

    template <typename T>
    ComponentArray<T>* getArray() {
        auto typeIndex = std::type_index(typeid(T));
        if (componentArrays.find(typeIndex) == componentArrays.end()) {
            componentArrays[typeIndex] = new ComponentArray<T>();
        }
        return static_cast<ComponentArray<T>*>(componentArrays[typeIndex]);
    }

    void flush(EntityManager& em) {
        for (auto& cmd : commands) {
            cmd(*this, em);
        }
        commands.clear();
    }

    void destroyAll() {
        for (auto& [type, array] : componentArrays) {
            array->clear();
        }
    }

    void reorder(Entity entity, ZLayer newLayer);

    void destroyEntityAndComponents(Entity e) { destroyEntityAndComponents(e, em); }

    ~ComponentManager() {
        for (auto& [type, array] : componentArrays) {
            delete array;
        }
    }

private:
    void destroyEntityAndComponents(Entity e, EntityManager& em) {
        for (auto& [_, array] : componentArrays) {
            array->remove(e);
        }
        em.destroyEntity(e);
    }

    EntityManager& em;

    std::vector<std::function<void(ComponentManager&, EntityManager&)>> commands;

    std::unordered_map<std::type_index, IComponentArray*> componentArrays;
};
