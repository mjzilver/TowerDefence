#pragma once

#include "../ecs/System.h"
#include "../event/Event.h"

enum class MenuMode { NONE, BUILD_TOWER, UPGRADE_TOWER };

class MenuSystem : public System {
public:
    MenuSystem(EngineContext& ctx);

    void update(float) override;
    void onEvent(const Event& event);
    void createMenu();
    void reset() override;

    MenuMode menuMode = MenuMode::NONE;

private:
    void buildClick(Entity entity);
    void upgradeClick(Entity entity);
    void unselect();

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
};