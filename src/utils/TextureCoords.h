#pragma once

#include <glm/glm.hpp>

inline static glm::vec4 getTextureCoords(int x, int y, int width, int height, int , int textureHeight) {
    int textureTopY = textureHeight - height;
    return glm::vec4(x * width, textureTopY - (height * y), width, height);
}