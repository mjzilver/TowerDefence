#pragma once

#include "../ecs/ComponentManager.h"
#include "../ecs/EntityFactory.h"
#include "../ecs/System.h"
#include "../event/Event.h"
#include "../event/EventDispatcher.h"

class CombatSystem : public System {
public:
    CombatSystem(ComponentManager& componentManager, EntityFactory& entityFactory)
        : componentManager(componentManager), entityFactory(entityFactory)  {
        EventDispatcher::getInstance().addListener(EventType::PROJECTILE_HIT, std::bind(&CombatSystem::onEvent, this, std::placeholders::_1));
    }

    void update(float) override { return; }

    void onEvent(const Event& event);
private:
    ComponentManager& componentManager;
    EntityFactory& entityFactory;
};
