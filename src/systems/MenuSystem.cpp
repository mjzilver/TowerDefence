#include "MenuSystem.h"

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
#include "StateSystem.h"

MenuSystem::MenuSystem(ComponentManager& componentManager, EntityFactory& entityFactory, StateSystem& stateSystem)
    : componentManager(componentManager), entityFactory(entityFactory), stateSystem(stateSystem) {
    EventDispatcher::getInstance().addListener(EventType::ENTITY_DESTROYED, std::bind(&MenuSystem::onEvent, this, std::placeholders::_1));
    EventDispatcher::getInstance().addListener(EventType::GRASS_TILE_CLICKED, std::bind(&MenuSystem::onEvent, this, std::placeholders::_1));
    EventDispatcher::getInstance().addListener(EventType::TOWER_CLICKED, std::bind(&MenuSystem::onEvent, this, std::placeholders::_1));
    EventDispatcher::getInstance().addListener(EventType::UNSELECT, std::bind(&MenuSystem::onEvent, this, std::placeholders::_1));
    EventDispatcher::getInstance().addListener(EventType::ACTIVATE_CHEATS, std::bind(&MenuSystem::onEvent, this, std::placeholders::_1));

    createMenu();
}

void MenuSystem::reset() {
    createMenu();
    currency = startCurrency;
    towerAmount = 0;
    killCount = 0;
    towerBuildCost = startTowerBuildCost;
}

void MenuSystem::createMenu() {
    int buttonXOffset = 50;
    const int buttonY = SCREEN_HEIGHT - 85;
    const int buttonSpacing = (SCREEN_WIDTH - buttonXOffset) / 5;

    glm::vec2 rectPos{0.0f, SCREEN_HEIGHT - 100};
    glm::vec2 rectSize{SCREEN_WIDTH, 100.0f};

    entityFactory.createRectangle(rectPos, rectSize);

    killCounterEntity = entityFactory.createKillCounter({buttonXOffset, buttonY});

    buttonXOffset += buttonSpacing;
    towerUpgradeButtonEntity = entityFactory.createUpgradeMenuItem({buttonXOffset, buttonY}, [&]() {
        unselect();
        menuMode = MenuMode::UPGRADE_TOWER;
        componentManager.getComponent<ClickableComponent>(towerUpgradeButtonEntity)->selected = true;
    });

    buttonXOffset += buttonSpacing;
    towerBuildButtonEntity = entityFactory.createBuildTowerMenuItem({buttonXOffset, buttonY}, [&]() {
        unselect();
        menuMode = MenuMode::BUILD_TOWER;
        componentManager.getComponent<ClickableComponent>(towerBuildButtonEntity)->selected = true;
    });

    buttonXOffset += buttonSpacing;
    currencyDisplayEntity = entityFactory.createCurrencyDisplay({buttonXOffset, buttonY});

    buttonXOffset += buttonSpacing;
    entityFactory.createMenuButton({buttonXOffset, buttonY}, [&]() { stateSystem.openMainMenu(); });

    auto* upgradeText = componentManager.getComponent<TextComponent>(towerUpgradeButtonEntity);
    upgradeText->text = "Upgrade\n" + std::to_string(towerUpgradeCost);
}

void MenuSystem::update(float) {
    auto* goldText = componentManager.getComponent<TextComponent>(currencyDisplayEntity);
    auto* killText = componentManager.getComponent<TextComponent>(killCounterEntity);

    if (goldText && killText) {
        goldText->text = std::to_string(currency) + " Gold";
        killText->text = "Kills\n" + std::to_string(killCount);
    }
}

void MenuSystem::buildClick(Entity entity) {
    if (currency >= towerBuildCost) {
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
    }
}

void MenuSystem::upgradeClick(Entity entity) {
    if (currency >= towerUpgradeCost) {
        auto* upgrade = componentManager.getComponent<UpgradeComponent>(entity);

        if (upgrade && upgrade->maxUpgradeLevel > upgrade->upgradeLevel) {
            entityFactory.upgradeTower(entity);

            currency -= towerUpgradeCost;

            auto* clickable = componentManager.getComponent<ClickableComponent>(towerUpgradeButtonEntity);
            clickable->selected = true;
        }
    }
}

void MenuSystem::onEvent(const Event& event) {
    if (event.type == EventType::GRASS_TILE_CLICKED && menuMode == MenuMode::BUILD_TOWER) {
        Entity entity = *event.getData<Entity>("entity");
        buildClick(entity);
    } else if (event.type == EventType::TOWER_CLICKED && menuMode == MenuMode::UPGRADE_TOWER) {
        Entity entity = *event.getData<Entity>("entity");
        upgradeClick(entity);
    } else if (event.type == EventType::ENTITY_DESTROYED) {
        Entity entity = *event.getData<Entity>("entity");
        auto* reward = componentManager.getComponent<RewardComponent>(entity);
        if (reward) {
            currency += reward->gold;
            killCount++;
        }
    } else if (event.type == EventType::UNSELECT) {
        unselect();
    } else if (event.type == EventType::ACTIVATE_CHEATS) {
        currency = 1000000;
        auto* clickables = componentManager.getArray<ClickableComponent>();

        for (Entity entity : clickables->getEntities()) {
            auto* clickable = clickables->get(entity);

            if (clickable->clickedEvent == EventType::GRASS_TILE_CLICKED) {
                buildClick(entity);
            }
        }

        for (Entity entity : clickables->getEntities()) {
            auto* clickable = clickables->get(entity);

            if (clickable->clickedEvent == EventType::TOWER_CLICKED) {
                upgradeClick(entity);
                upgradeClick(entity);
            }
        }
    }
}

void MenuSystem::unselect() {
    componentManager.getComponent<ClickableComponent>(towerBuildButtonEntity)->selected = false;
    componentManager.getComponent<ClickableComponent>(towerUpgradeButtonEntity)->selected = false;

    menuMode = MenuMode::NONE;
};
