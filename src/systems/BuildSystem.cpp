#include "BuildSystem.h"

#include "../components/ChildComponent.h"
#include "../components/ClickableComponent.h"
#include "../components/PositionComponent.h"
#include "../components/RewardComponent.h"
#include "../components/SizeComponent.h"
#include "../components/TextComponent.h"
#include "../components/UpgradeComponent.h"

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
            towerBuildCost += TOWER_BUILD_COST_INCREASE;

            auto* textComponent = componentManager.getComponent<TextComponent>(towerBuildButtonEntity);
            textComponent->text = "Build Tower\n" + std::to_string(towerBuildCost) + " gold";

            auto* clickable = componentManager.getComponent<ClickableComponent>(towerBuildButtonEntity);
            clickable->selected = true;
        }
    } else if (event.type == EventType::UPGRADE_TOWER && currency >= TOWER_UPGRADE_COST) {
        Entity entity = *event.getData<Entity>("entity");
        auto* upgrade = componentManager.getComponent<UpgradeComponent>(entity);

        if (upgrade && upgrade->maxUpgradeLevel > upgrade->upgradeLevel) {
            entityFactory.upgradeTower(entity);

            currency -= TOWER_UPGRADE_COST;

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
