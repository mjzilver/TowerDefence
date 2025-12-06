#pragma once
#include <functional>
#include "../ecs/System.h"
#include "../ecs/ComponentManager.h"
#include "../ecs/EntityFactory.h"

#include "../event/Event.h"
#include "../event/EventDispatcher.h"

class ClickSystem : public System {
public:
    ClickSystem(ComponentManager& componentManager, EntityFactory& entityFactory)
        : componentManager(componentManager), entityFactory(entityFactory) {
        EventDispatcher::getInstance().addListener(EventType::GRASS_TILE_CLICKED, std::bind(&ClickSystem::onEvent, this, std::placeholders::_1));
        EventDispatcher::getInstance().addListener(EventType::BUILD_TOWER_MENU_ITEM_CLICKED, std::bind(&ClickSystem::onEvent, this, std::placeholders::_1));
        EventDispatcher::getInstance().addListener(EventType::UPGRADE_MENU_ITEM_CLICKED, std::bind(&ClickSystem::onEvent, this, std::placeholders::_1));
        EventDispatcher::getInstance().addListener(EventType::UNSELECT, std::bind(&ClickSystem::onEvent, this, std::placeholders::_1));
    }

    void update(float) override { return; }

    void onClick(int button, int action, double x, double y);

    void onHover(double x, double y);
    
    void onEvent(const Event& event);

private:
    const int CLICK_SIZE = 1;
    ComponentManager& componentManager;
    EntityFactory& entityFactory;

    enum class MenuItem { NONE, UPGRADE_TOWER, BUILD_TOWER };
    MenuItem selectedMenuItem = MenuItem::NONE;
};