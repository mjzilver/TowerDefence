#pragma once

#include "../ecs/EntityFactory.h"
#include "../event/EventDispatcher.h"
#include "../font/FontLoader.h"
#include "../map/MapLoader.h"

struct EngineContext {
    EntityManager& entityManager;
    ComponentManager& componentManager;

    FontLoader& fontLoader;
    MapLoader& mapLoader;
    EventDispatcher& eventDispatcher;
    EntityFactory& entityFactory;
};
