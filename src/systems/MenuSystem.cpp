#include "MenuSystem.h"

#include <functional>

#include "../components/ChildComponent.h"
#include "../components/ClickableComponent.h"
#include "../components/PositionComponent.h"
#include "../components/RewardComponent.h"
#include "../components/SizeComponent.h"
#include "../components/TextComponent.h"
#include "../components/TextureComponent.h"
#include "../components/UpgradeComponent.h"
#include "../event/Event.h"
#include "../event/EventDispatcher.h"
#include "../utils/Globals.h"

MenuSystem::MenuSystem(EngineContext& ctx) : System(ctx, "MenuSystem") {
    context.eventDispatcher.addListener(EventType::ENTITY_DESTROYED, std::bind(&MenuSystem::onEvent, this, std::placeholders::_1));
    context.eventDispatcher.addListener(EventType::GRASS_TILE_CLICKED, std::bind(&MenuSystem::onEvent, this, std::placeholders::_1));
    context.eventDispatcher.addListener(EventType::TOWER_CLICKED, std::bind(&MenuSystem::onEvent, this, std::placeholders::_1));
    context.eventDispatcher.addListener(EventType::UNSELECT, std::bind(&MenuSystem::onEvent, this, std::placeholders::_1));
    context.eventDispatcher.addListener(EventType::ACTIVATE_CHEATS, std::bind(&MenuSystem::onEvent, this, std::placeholders::_1));

    writes.push_back(typeid(TextComponent));
    writes.push_back(typeid(ChildComponent));
    writes.push_back(typeid(TextureComponent));
    writes.push_back(typeid(ClickableComponent));
    writes.push_back(typeid(UpgradeComponent));

    reads.push_back(typeid(PositionComponent));
    reads.push_back(typeid(RewardComponent));
    reads.push_back(typeid(SizeComponent));
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

    const glm::vec2 rectPos{0.0f, SCREEN_HEIGHT - 100};
    const glm::vec2 rectSize{SCREEN_WIDTH, 100.0f};

    auto& componentManager = context.componentManager;
    auto& entityFactory = context.entityFactory;

    entityFactory.createRectangle(rectPos, rectSize);

    killCounterEntity = entityFactory.createKillCounter({buttonXOffset, buttonY});

    buttonXOffset += buttonSpacing;
    towerUpgradeButtonEntity = entityFactory.createUpgradeMenuItem(
        {buttonXOffset, buttonY},
        [&]() {
            unselect();
            menuMode = MenuMode::UPGRADE_TOWER;
            write<ClickableComponent>(towerUpgradeButtonEntity)->selected = true;
        },
        towerUpgradeCost);

    buttonXOffset += buttonSpacing;
    towerBuildButtonEntity = entityFactory.createBuildTowerMenuItem({buttonXOffset, buttonY}, [&]() {
        unselect();
        menuMode = MenuMode::BUILD_TOWER;
        write<ClickableComponent>(towerBuildButtonEntity)->selected = true;
    });

    buttonXOffset += buttonSpacing;
    currencyDisplayEntity = entityFactory.createCurrencyDisplay({buttonXOffset, buttonY});

    buttonXOffset += buttonSpacing;
    entityFactory.createMenuButton({buttonXOffset, buttonY}, [&]() {
        context.eventDispatcher.dispatch({EventType::OPEN_MENU_CLICKED});
    });
}

void MenuSystem::update(float) {
    auto* goldText = write<TextComponent>(currencyDisplayEntity);
    if (goldText) {
        goldText->text = std::to_string(currency) + " Gold";
    }

    auto* killText = write<TextComponent>(killCounterEntity);
    if (killText) {
        killText->text = "Kills\n" + std::to_string(killCount);
    }
}

void MenuSystem::buildClick(Entity entity) {
    if (currency >= towerBuildCost) {
        auto& componentManager = context.componentManager;

        const auto* position = read<PositionComponent>(entity);
        const auto* size = read<SizeComponent>(entity);
        auto* child = write<ChildComponent>(entity);

        if (!child) {
            auto tower = context.entityFactory.createTower({position->x, position->y - size->h + 16});
            ChildComponent childComponent;
            childComponent.child = tower;
            componentManager.addComponent(entity, childComponent);

            currency -= towerBuildCost;
            towerBuildCost += towerBuildCostIncrease;

            auto* textComponent = write<TextComponent>(towerBuildButtonEntity);
            textComponent->text = "Build Tower\n" + std::to_string(towerBuildCost) + " gold";

            auto* clickable = write<ClickableComponent>(towerBuildButtonEntity);
            clickable->selected = true;
        }
    }
}

void MenuSystem::upgradeClick(Entity entity) {
    if (currency >= towerUpgradeCost) {
        auto& componentManager = context.componentManager;

        auto* upgrade = write<UpgradeComponent>(entity);

        if (upgrade && upgrade->maxUpgradeLevel > upgrade->upgradeLevel) {
            context.entityFactory.upgradeTower(entity);

            currency -= towerUpgradeCost;

            auto* clickable = write<ClickableComponent>(towerUpgradeButtonEntity);
            clickable->selected = true;
        }
    }
}

void MenuSystem::onEvent(const Event& event) {
    auto& componentManager = context.componentManager;

    if (event.type == EventType::GRASS_TILE_CLICKED && menuMode == MenuMode::BUILD_TOWER) {
        Entity entity = event.getEntity("entity");
        buildClick(entity);
    } else if (event.type == EventType::TOWER_CLICKED && menuMode == MenuMode::UPGRADE_TOWER) {
        Entity entity = event.getEntity("entity");
        upgradeClick(entity);
    } else if (event.type == EventType::ENTITY_DESTROYED) {
        Entity entity = event.getEntity("entity");
        const auto* reward = read<RewardComponent>(entity);
        if (reward) {
            currency += reward->gold;
            killCount++;
        }
    } else if (event.type == EventType::UNSELECT) {
        unselect();
    } else if (event.type == EventType::ACTIVATE_CHEATS) {
        currency = 1000000;
        auto* clickables = writeArray<ClickableComponent>();

        for (Entity entity : clickables->getEntities()) {
            auto* clickable = clickables->get(entity);

            if (clickable && clickable->clickedEvent == EventType::GRASS_TILE_CLICKED) {
                buildClick(entity);
            }
        }

        for (Entity entity : clickables->getEntities()) {
            auto* clickable = clickables->get(entity);

            if (clickable && clickable->clickedEvent == EventType::TOWER_CLICKED) {
                upgradeClick(entity);
                upgradeClick(entity);
            }
        }
    }
}

void MenuSystem::unselect() {
    auto& componentManager = context.componentManager;
    write<ClickableComponent>(towerBuildButtonEntity)->selected = false;
    write<ClickableComponent>(towerUpgradeButtonEntity)->selected = false;

    menuMode = MenuMode::NONE;
};
