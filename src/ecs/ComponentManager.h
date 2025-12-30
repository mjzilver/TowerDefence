#pragma once

#include <cassert>
#include <functional>
#include <iostream>
#include <mutex>
#include <typeindex>
#include <unordered_map>

#include "../components/AnimationComponent.h"
#include "../components/ChildComponent.h"
#include "../components/ClickableComponent.h"
#include "../components/CollisionComponent.h"
#include "../components/ColorComponent.h"
#include "../components/DamageComponent.h"
#include "../components/DeathComponent.h"
#include "../components/DirectionComponent.h"
#include "../components/FlagComponent.h"
#include "../components/HealthComponent.h"
#include "../components/PathfindingComponent.h"
#include "../components/PositionComponent.h"
#include "../components/RewardComponent.h"
#include "../components/RotationComponent.h"
#include "../components/ShaderComponent.h"
#include "../components/SizeComponent.h"
#include "../components/SpeedComponent.h"
#include "../components/TextComponent.h"
#include "../components/TextureComponent.h"
#include "../components/UpgradeComponent.h"
#include "../components/VelocityComponent.h"
#include "../components/WeaponComponent.h"
#include "Component.h"
#include "ComponentArray.h"
#include "EntityManager.h"

class ComponentManager {
public:
    ComponentManager(EntityManager& em) : em(em) {
        registerComponent<AnimationComponent>();
        registerComponent<ChildComponent>();
        registerComponent<ClickableComponent>();
        registerComponent<CollisionComponent>();
        registerComponent<ColorComponent>();
        registerComponent<DamageComponent>();
        registerComponent<DeathComponent>();
        registerComponent<DirectionComponent>();
        registerComponent<FlagComponent>();
        registerComponent<HealthComponent>();
        registerComponent<PathfindingComponent>();
        registerComponent<PositionComponent>();
        registerComponent<RewardComponent>();
        registerComponent<RotationComponent>();
        registerComponent<ShaderComponent>();
        registerComponent<SizeComponent>();
        registerComponent<SpeedComponent>();
        registerComponent<TextComponent>();
        registerComponent<TextureComponent>();
        registerComponent<UpgradeComponent>();
        registerComponent<VelocityComponent>();
        registerComponent<WeaponComponent>();
    }

    template <typename T>
    void addComponent(Entity e, T component) {
        std::lock_guard<std::mutex> guard(commandMutex);
        commands.push_back(
            [=, c = std::move(component)](ComponentManager& cm, EntityManager&) mutable { cm.getArray<T>()->insert(e, std::move(c)); });
    }

    void addComponent(Entity e, PositionComponent pos) {
        std::lock_guard<std::mutex> guard(commandMutex);
        commands.push_back([=, c = std::move(pos)](ComponentManager& cm, EntityManager&) mutable {
            cm.getArray<PositionComponent>()->insert(e, std::move(c));
            reorder(e, pos.zIndex);
        });
    }

    template <typename T>
    void removeComponent(Entity e) {
        std::lock_guard<std::mutex> guard(commandMutex);
        commands.push_back([=](ComponentManager& cm, EntityManager&) { cm.getArray<T>()->remove(e); });
    }

    void scheduleDestruction(Entity e) {
        std::lock_guard<std::mutex> guard(commandMutex);
        commands.push_back([=](ComponentManager& cm, EntityManager& em) { cm.destroyEntityAndComponents(e, em); });
    }

    template <typename T>
    const T* getComponent(Entity entity) const {
        return getArray<T>()->get(entity);
    }

    template <typename T>
    T* getComponent(Entity entity) {
        return getArray<T>()->get(entity);
    }

    template <typename T>
    void registerComponent() {
        auto typeIndex = std::type_index(typeid(T));
        if (componentArrays.find(typeIndex) == componentArrays.end()) {
            componentArrays[typeIndex] = new ComponentArray<T>();
        }
    }

    template <typename T>
    ComponentArray<T>* getArray() const {
        auto typeIndex = std::type_index(typeid(T));
        auto it = componentArrays.find(typeIndex);

        if (it == componentArrays.end()) {
            std::cerr << "Component not registered: " << typeid(T).name() << "\n";
            std::abort();
        }

        return static_cast<ComponentArray<T>*>(it->second);
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

    std::mutex commandMutex;
    std::vector<std::function<void(ComponentManager&, EntityManager&)>> commands;

    std::unordered_map<std::type_index, IComponentArray*> componentArrays;
};
