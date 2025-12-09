#pragma once

#include <string>
#include <unordered_map>
#include <glad/glad.h>

#include "Texture.h"

const std::string texturePath = "resources/images/";

class TextureManager {
public:
    TextureManager() = default;
    ~TextureManager() = default;

    Texture loadTexture(const std::string& filePath);

private:
    unsigned char* loadPngImage(const std::string& filePath, unsigned int& width, unsigned int& height);

    std::unordered_map<std::string, Texture> textureCache;
};