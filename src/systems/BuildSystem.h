#pragma once
#include "../ecs/System.h"
#include "../ecs/ComponentManager.h"
#include "../ecs/EntityFactory.h"

#include "../event/Event.h"

class BuildSystem : public System {
public:
    BuildSystem(ComponentManager& componentManager, EntityFactory& entityFactory);

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

    const int towerBuildCostIncrease = 25;
    const int towerUpgradeCost = 150;
};