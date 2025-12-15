#pragma once

#include <glad/glad.h>

#include <string>
#include <unordered_map>

#include "Texture.h"

class TextureManager {
public:
    TextureManager() = default;
    ~TextureManager() = default;

    Texture loadTexture(const std::string& filePath);

private:
    const std::string texturePath = "resources/images/";
    unsigned char* loadPngImage(const std::string& filePath, unsigned int& width, unsigned int& height);

    std::unordered_map<std::string, Texture> textureCache;
};