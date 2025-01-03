#include "EntityFactory.h"

#include "../components/PositionComponent.h"
#include "../components/TextureComponent.h"
#include "../components/SizeComponent.h"
#include "../components/DirectionComponent.h"

#include "../utils/TextureCoords.h"

Entity EntityFactory::createGrassTile(glm::vec2 position, TextureManager& textureManager) {
    Entity entity = entityManager.createEntity();

    static const int TEXTURE_SIZE = 64;
    static const int TILE_SIZE = 80;

    PositionComponent positionComponent;
    positionComponent.x = position.x;
    positionComponent.y = position.y;
    componentManager.addComponent(entity, positionComponent);

    SizeComponent sizeComponent;
    sizeComponent.w = TILE_SIZE;
    sizeComponent.h = TILE_SIZE;
    componentManager.addComponent(entity, sizeComponent);

    TextureComponent textureComponent;
    textureComponent.texture = textureManager.loadTexture("tiles/Grass.png");
    textureComponent.coords = getTextureCoords(2, 1, TEXTURE_SIZE, TEXTURE_SIZE, textureComponent.texture.size.x, textureComponent.texture.size.y);
    componentManager.addComponent(entity, textureComponent);

    return entity;
}

Entity EntityFactory::createPathTile(glm::vec2 position, TextureManager& textureManager) { 
    Entity entity = entityManager.createEntity();

    static const int TEXTURE_SIZE = 64;
    static const int TILE_SIZE = 80;

    PositionComponent positionComponent;
    positionComponent.x = position.x;
    positionComponent.y = position.y;
    componentManager.addComponent(entity, positionComponent);

    SizeComponent sizeComponent;
    sizeComponent.w = TILE_SIZE;
    sizeComponent.h = TILE_SIZE;
    componentManager.addComponent(entity, sizeComponent);

    TextureComponent textureComponent;
    textureComponent.texture = textureManager.loadTexture("tiles/Grass.png");
    textureComponent.coords = getTextureCoords(9, 2, TEXTURE_SIZE, TEXTURE_SIZE, textureComponent.texture.size.x, textureComponent.texture.size.y);
    componentManager.addComponent(entity, textureComponent);

    return entity;
}
