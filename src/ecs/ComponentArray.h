#pragma once

#include <cassert>
#include <deque>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "../utils/Globals.h"
#include "Component.h"

class IComponentArray {
public:
    virtual ~IComponentArray() = default;
    virtual void remove(Entity entity) = 0;
    virtual void clear() = 0;
};

template <typename T>
class ComponentArray : public IComponentArray {
public:
    void insert(Entity e, T component) {
        if constexpr (DEBUG_ENABLED) {
            auto it = entityToIndex.find(e);
            if (it != entityToIndex.end()) {
                std::cerr << "Component: " << typeid(T).name() << " already exists on entity " << e << "\n";
                std::abort();
            }
        }

        size_t index;
        if (!freeIndices.empty()) {
            index = freeIndices.back();
            freeIndices.pop_back();
            components[index] = new T(std::move(component));
            entities[index] = e;
        } else {
            index = entities.size();
            entities.push_back(e);
            components.push_back(new T(std::move(component)));
        }

        entityToIndex[e] = index;
    }

    void insertOrReplace(Entity e, T component) {
        auto it = entityToIndex.find(e);
        if (it != entityToIndex.end()) {
            *components[it->second] = std::move(component);
            return;
        }

        insert(e, component);
    }

    void clear() override {
        for (T* component : components) {
            delete component;
        }
        
        entityToIndex.clear();
        components.clear();
        entities.clear();
        freeIndices.clear();
    }

    void remove(Entity e) override {
        auto it = entityToIndex.find(e);
        if (it == entityToIndex.end()) return;

        size_t index = it->second;

        delete components[index];
        components[index] = nullptr;
        entities[index] = INVALID_ENTITY;
        freeIndices.push_back(index);
        entityToIndex.erase(e);
    }

    const T* get(Entity entity) const {
        auto it = entityToIndex.find(entity);
        if (it == entityToIndex.end()) return nullptr;
        return components[it->second];
    }

    T* get(Entity entity) {
        auto it = entityToIndex.find(entity);
        if (it == entityToIndex.end()) return nullptr;
        return components[it->second];
    }

    std::deque<T*>& getAll() { return components; }
    std::deque<Entity>& getEntities() { return entities; }
    const std::deque<Entity>& getEntities() const { return entities; }

    ~ComponentArray() {
        for (T* component : components) {
            if (component) delete component;
        }
    }

private:
    std::deque<Entity> entities;
    std::deque<T*> components;
    std::unordered_map<Entity, size_t> entityToIndex;
    std::vector<size_t> freeIndices;
};