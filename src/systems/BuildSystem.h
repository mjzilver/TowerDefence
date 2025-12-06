#pragma once
#include <functional>
#include "../ecs/System.h"
#include "../ecs/ComponentManager.h"
#include "../ecs/EntityFactory.h"

#include "../event/Event.h"
#include "../event/EventDispatcher.h"

class BuildSystem : public System {
public:
    BuildSystem(ComponentManager& componentManager, EntityFactory& entityFactory, Entity& entity)
        : componentManager(componentManager), entityFactory(entityFactory), currencyDisplayEntity(entity) {
        EventDispatcher::getInstance().addListener(EventType::BUILD_TOWER, std::bind(&BuildSystem::onEvent, this, std::placeholders::_1));
        EventDispatcher::getInstance().addListener(EventType::UPGRADE_TOWER, std::bind(&BuildSystem::onEvent, this, std::placeholders::_1));
        EventDispatcher::getInstance().addListener(EventType::ENTITY_DESTROYED, std::bind(&BuildSystem::onEvent, this, std::placeholders::_1));
    }

    void update(float) override;    
    void onEvent(const Event& event);

private:
    ComponentManager& componentManager;
    EntityFactory& entityFactory;

    Entity& currencyDisplayEntity;

    int currency = 200;
};