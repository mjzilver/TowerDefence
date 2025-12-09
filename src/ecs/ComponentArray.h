#pragma once

#include <unordered_map>
#include <deque>
#include <vector>

#include "Component.h"

class IComponentArray {
public:
    virtual ~IComponentArray() = default;
    virtual void remove(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray {
public:
   void insert(Entity entity, T component) {
        size_t index;

        if (!freeIndices.empty()) {
            index = freeIndices.back();
            freeIndices.pop_back();
            components[index] = new T(std::move(component));
            entities[index] = entity;
        } else {
            index = entities.size();
            entities.push_back(entity);
            components.push_back(new T(std::move(component)));
        }

        entityToIndex[entity] = index;
    }

    void remove(Entity entity) override {
        auto it = entityToIndex.find(entity);
        if (it == entityToIndex.end()) return;

        size_t index = it->second;

        delete components[index];
        components[index] = nullptr;
        entities[index] = INVALID_ENTITY;
        freeIndices.push_back(index); 
        entityToIndex.erase(entity);
    }

    T* get(Entity entity) const {
        auto it = entityToIndex.find(entity);
        if (it == entityToIndex.end()) return nullptr;
        return components[it->second];
    }

    std::deque<T*>& getAll() { return components; }
    std::deque<Entity>& getEntities() { return entities; }

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
