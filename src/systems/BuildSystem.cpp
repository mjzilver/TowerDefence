#include "BuildSystem.h"

#include <functional>

#include "../components/ChildComponent.h"
#include "../components/ClickableComponent.h"
#include "../components/PositionComponent.h"
#include "../components/RewardComponent.h"
#include "../components/SizeComponent.h"
#include "../components/TextComponent.h"
#include "../components/UpgradeComponent.h"
#include "../event/Event.h"
#include "../event/EventDispatcher.h"
#include "../utils/Globals.h"

BuildSystem::BuildSystem(ComponentManager& componentManager, EntityFactory& entityFactory)
    : componentManager(componentManager), entityFactory(entityFactory) {
    EventDispatcher::getInstance().addListener(EventType::BUILD_TOWER, std::bind(&BuildSystem::onEvent, this, std::placeholders::_1));
    EventDispatcher::getInstance().addListener(EventType::UPGRADE_TOWER, std::bind(&BuildSystem::onEvent, this, std::placeholders::_1));
    EventDispatcher::getInstance().addListener(EventType::ENTITY_DESTROYED, std::bind(&BuildSystem::onEvent, this, std::placeholders::_1));

    currencyDisplayEntity = entityFactory.createCurrencyDisplay(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
    towerUpgradeButtonEntity = entityFactory.createUpgradeMenuItem(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
    towerBuildButtonEntity = entityFactory.createBuildTowerMenuItem(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
    killCounterEntity = entityFactory.createKillCounter(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));

    auto* upgradeText = componentManager.getComponent<TextComponent>(towerUpgradeButtonEntity);
    upgradeText->text = "Upgrade\n" + std::to_string(towerUpgradeCost);

#if STRESS_TEST
    currency = 1000000;
#endif
}

void BuildSystem::update(float) {
    auto* goldText = componentManager.getComponent<TextComponent>(currencyDisplayEntity);
    goldText->text = std::to_string(currency) + " Gold";

    auto* killText = componentManager.getComponent<TextComponent>(killCounterEntity);
    killText->text = "Kills\n" + std::to_string(killCount);
}

void BuildSystem::onEvent(const Event& event) {
    if (event.type == EventType::BUILD_TOWER && currency >= towerBuildCost) {
        Entity entity = *event.getData<Entity>("entity");
        auto* position = componentManager.getComponent<PositionComponent>(entity);
        auto* size = componentManager.getComponent<SizeComponent>(entity);
        auto* child = componentManager.getComponent<ChildComponent>(entity);

        if (!child) {
            auto tower = entityFactory.createTower({position->x, position->y - size->h + 16});
            ChildComponent childComponent;
            childComponent.child = tower;
            componentManager.addComponent(entity, childComponent);

            currency -= towerBuildCost;
            towerBuildCost += towerBuildCostIncrease;

            auto* textComponent = componentManager.getComponent<TextComponent>(towerBuildButtonEntity);
            textComponent->text = "Build Tower\n" + std::to_string(towerBuildCost) + " gold";

            auto* clickable = componentManager.getComponent<ClickableComponent>(towerBuildButtonEntity);
            clickable->selected = true;
        }
    } else if (event.type == EventType::UPGRADE_TOWER && currency >= towerUpgradeCost) {
        Entity entity = *event.getData<Entity>("entity");
        auto* upgrade = componentManager.getComponent<UpgradeComponent>(entity);

        if (upgrade && upgrade->maxUpgradeLevel > upgrade->upgradeLevel) {
            entityFactory.upgradeTower(entity);

            currency -= towerUpgradeCost;

            auto* clickable = componentManager.getComponent<ClickableComponent>(towerUpgradeButtonEntity);
            clickable->selected = true;
        }
    } else if (event.type == EventType::ENTITY_DESTROYED) {
        Entity entity = *event.getData<Entity>("entity");
        auto* reward = componentManager.getComponent<RewardComponent>(entity);
        if (reward) {
            currency += reward->gold;
            killCount++;
        }
    }
}
