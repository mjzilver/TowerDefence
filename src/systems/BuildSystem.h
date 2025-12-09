#pragma once
#include <functional>
#include "../ecs/System.h"
#include "../ecs/ComponentManager.h"
#include "../ecs/EntityFactory.h"

#include "../components/TextComponent.h"

#include "../event/Event.h"
#include "../event/EventDispatcher.h"
#include "../utils/Globals.h"

class BuildSystem : public System {
public:
    BuildSystem(ComponentManager& componentManager, EntityFactory& entityFactory)
        : componentManager(componentManager), entityFactory(entityFactory) {
        EventDispatcher::getInstance().addListener(EventType::BUILD_TOWER, std::bind(&BuildSystem::onEvent, this, std::placeholders::_1));
        EventDispatcher::getInstance().addListener(EventType::UPGRADE_TOWER, std::bind(&BuildSystem::onEvent, this, std::placeholders::_1));
        EventDispatcher::getInstance().addListener(EventType::ENTITY_DESTROYED, std::bind(&BuildSystem::onEvent, this, std::placeholders::_1));

        currencyDisplayEntity = entityFactory.createCurrencyDisplay(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        towerUpgradeButtonEntity = entityFactory.createUpgradeMenuItem(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        towerBuildButtonEntity = entityFactory.createBuildTowerMenuItem(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        killCounterEntity = entityFactory.createKillCounter(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));

        auto* upgradeText = componentManager.getComponent<TextComponent>(towerUpgradeButtonEntity);
        upgradeText->text = "Upgrade\n" + std::to_string(TOWER_UPGRADE_COST);

#if STRESS_TEST
        currency = 1000000;
#endif
    }

    void update(float) override;    
    void onEvent(const Event& event);

private:
    ComponentManager& componentManager;
    EntityFactory& entityFactory;

    Entity currencyDisplayEntity;
    Entity towerBuildButtonEntity;
    Entity towerUpgradeButtonEntity;
    Entity killCounterEntity;

    int currency = 100;
    int towerAmount = 0;
    int killCount = 0;
    int towerBuildCost = 50;

    const int TOWER_BUILD_COST_INCREASE = 25;
    const int TOWER_UPGRADE_COST = 150;
};