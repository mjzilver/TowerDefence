#pragma once

#include <iostream>
#include <functional>
#include "../ecs/System.h"
#include "../event/Event.h"

#include "../components/CollisionComponent.h"
#include "../components/DeathComponent.h"
#include "../components/HealthComponent.h"
#include "../components/PathfindingComponent.h"
#include "../components/PositionComponent.h"
#include "../components/SizeComponent.h"
#include "../components/SpeedComponent.h"
#include "../components/TextureComponent.h"
#include "../components/VelocityComponent.h"
#include "../components/ComponentDebugPrint.h"

class DebugSystem : public System {
public:
    DebugSystem(EngineContext& ctx) : System(ctx) {
        context.eventDispatcher.addListener(EventType::ENTITY_CLICKED, 
            std::bind(&DebugSystem::onEvent, this, std::placeholders::_1));
    };

    void update(float) {}

    void onEvent(const Event& event) {
        if(event.type != EventType::ENTITY_CLICKED) return;

        Entity entity = *event.getData<Entity>("entity");
        std::cout << "Clicked entity: " << entity << "\n";

        auto& cm = context.componentManager;

        #define PRINT_COMPONENT(Type) \
            if(auto* comp = cm.getComponent<Type>(entity)) { \
                std::cout << #Type ": " << *comp << "\n"; \
            }

        PRINT_COMPONENT(PositionComponent);
        PRINT_COMPONENT(SizeComponent);
        PRINT_COMPONENT(VelocityComponent);
        PRINT_COMPONENT(CollisionComponent);
        PRINT_COMPONENT(DeathComponent);
        PRINT_COMPONENT(HealthComponent);
        PRINT_COMPONENT(PathfindingComponent);
        PRINT_COMPONENT(SpeedComponent);
        PRINT_COMPONENT(TextureComponent);

        #undef PRINT_COMPONENT
    }

    void reset() {}
};
