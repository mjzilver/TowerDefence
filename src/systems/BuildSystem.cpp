#include "BuildSystem.h"

#include "../components/ChildComponent.h"
#include "../components/PositionComponent.h"
#include "../components/RewardComponent.h"
#include "../components/SizeComponent.h"
#include "../components/TextComponent.h"
#include "../components/UpgradeComponent.h"

const int TOWER_BUILD_COST = 50;
const int TOWER_UPGRADE_COST = 100;

void BuildSystem::update(float) {
    auto* text = componentManager.getComponent<TextComponent>(currencyDisplayEntity);
    text->text = std::to_string(currency) + " Gold";
}

void BuildSystem::onEvent(const Event& event) {
    if (event.type == EventType::BUILD_TOWER && currency >= TOWER_BUILD_COST) {
        Entity entity = *event.getData<Entity>("entity");
        auto* position = componentManager.getComponent<PositionComponent>(entity);
        auto* size = componentManager.getComponent<SizeComponent>(entity);
        auto* child = componentManager.getComponent<ChildComponent>(entity);

        if (!child) {
            auto tower = entityFactory.createTower({position->x, position->y - size->h + 16});
            ChildComponent childComponent;
            childComponent.child = tower;
            componentManager.addComponent(entity, childComponent);

            currency -= TOWER_BUILD_COST;
        }
    } else if (event.type == EventType::UPGRADE_TOWER && currency >= TOWER_UPGRADE_COST) {
        Entity entity = *event.getData<Entity>("entity");
        auto* upgrade = componentManager.getComponent<UpgradeComponent>(entity);

        if (upgrade && upgrade->maxUpgradeLevel > upgrade->upgradeLevel) {
            entityFactory.upgradeTower(entity);

            currency -= TOWER_UPGRADE_COST;
        }
    } else if (event.type == EventType::ENTITY_DESTROYED) {
        Entity entity = *event.getData<Entity>("entity");
        auto* reward = componentManager.getComponent<RewardComponent>(entity);
        if (reward) {
            currency += reward->gold;
        }
    }
}
