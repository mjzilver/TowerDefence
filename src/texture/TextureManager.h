#pragma once

#include <glad/glad.h>

#include <string>
#include <unordered_map>
#include <vector>

#include "Texture.h"

class TextureManager {
public:
    TextureManager() = default;
    ~TextureManager() = default;

    void preload(const std::string& filePath);
    void preload(const std::vector<std::string>& files);
    const Texture& get(const std::string& filePath) const;

private:
    const std::string texturePath = "resources/images/";
    unsigned char* loadPngImage(const std::string& filePath, unsigned int& width, unsigned int& height);

    std::unordered_map<std::string, Texture> textureCache;
};