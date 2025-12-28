#pragma once

#include <functional>
#include <iostream>

#include "../components/CollisionComponent.h"
#include "../components/ComponentDebugPrint.h"
#include "../components/DeathComponent.h"
#include "../components/HealthComponent.h"
#include "../components/PathfindingComponent.h"
#include "../components/PositionComponent.h"
#include "../components/SizeComponent.h"
#include "../components/SpeedComponent.h"
#include "../components/TextureComponent.h"
#include "../components/VelocityComponent.h"
#include "../ecs/System.h"
#include "../event/Event.h"

#define PRINT_COMPONENT(Type)                         \
    if (auto* comp = cm.getComponent<Type>(entity)) { \
        std::cout << #Type ": " << *comp << "\n";     \
    }

class DebugSystem : public System {
public:
    DebugSystem(EngineContext& ctx) : System(ctx) {
        context.eventDispatcher.addListener(EventType::ENTITY_CLICKED, std::bind(&DebugSystem::onEvent, this, std::placeholders::_1));

        reads.push_back(typeid(PositionComponent));
        reads.push_back(typeid(SizeComponent));
        reads.push_back(typeid(VelocityComponent));
        reads.push_back(typeid(CollisionComponent));
        reads.push_back(typeid(DeathComponent));
        reads.push_back(typeid(HealthComponent));
        reads.push_back(typeid(PathfindingComponent));
        reads.push_back(typeid(SpeedComponent));
        reads.push_back(typeid(TextureComponent));
    };

    void update(float) {}

    void onEvent(const Event& event) {
        if (event.type != EventType::ENTITY_CLICKED) return;

        Entity entity = event.getEntity("entity");
        std::cout << "Clicked entity: " << entity << "\n";

        auto& cm = context.componentManager;

        PRINT_COMPONENT(PositionComponent);
        PRINT_COMPONENT(SizeComponent);
        PRINT_COMPONENT(VelocityComponent);
        PRINT_COMPONENT(CollisionComponent);
        PRINT_COMPONENT(DeathComponent);
        PRINT_COMPONENT(HealthComponent);
        PRINT_COMPONENT(PathfindingComponent);
        PRINT_COMPONENT(SpeedComponent);
        PRINT_COMPONENT(TextureComponent);
    }

    void reset() {}
};
