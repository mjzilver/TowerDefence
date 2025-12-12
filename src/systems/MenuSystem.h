#pragma once
#include "../ecs/System.h"
#include "../ecs/ComponentManager.h"
#include "../ecs/EntityFactory.h"

#include "../event/Event.h"

class MenuSystem : public System {
public:
    MenuSystem(ComponentManager& componentManager, EntityFactory& entityFactory, bool& menuMode);

    void update(float) override;    
    void onEvent(const Event& event);
    void createMenu();
    void reset() override;

private:
    ComponentManager& componentManager;
    EntityFactory& entityFactory;

    Entity currencyDisplayEntity;
    Entity towerBuildButtonEntity;
    Entity towerUpgradeButtonEntity;
    Entity killCounterEntity;

    const int startCurrency = 100;
    const int startTowerBuildCost = 50;

    int currency = startCurrency;
    int towerAmount = 0;
    int killCount = 0;
    int towerBuildCost = startTowerBuildCost;

    const int towerBuildCostIncrease = 25;
    const int towerUpgradeCost = 150;

    bool& menuMode;
};