#pragma once

#include <glm/glm.hpp>

// helper function to turn x, y and width, height into normalized texture coordinates
static glm::vec4 getTextureCoords(int x, int y, int width, int height, int textureWidth, int textureHeight) {
    int textureTopY = textureHeight - height;
    return glm::vec4(x * width, textureTopY - (height * y), width, height);
}