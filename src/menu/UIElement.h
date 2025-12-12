#pragma once

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <functional>
#include <string>

class UIElement {
public:
    void setPos(glm::vec4 newPos) { pos = newPos; }
    glm::vec4 getPos() const { return pos; }

    void setText(const std::string& newText) { text = newText; }
    const std::string& getText() const { return text; }

    void setCallback(std::function<void()> cb) { callback = cb; }
    void onClick() { if (callback) callback(); }

    void setBgColor(const glm::vec3& c) { bgColor = c; }
    glm::vec3 getBgColor() const { return bgColor; }

    void setTextColor(const glm::vec3& c) { textColor = c; }
    glm::vec3 getTextColor() const { return textColor; }

    void setHoverColor(const glm::vec3& c) { hoverColor = c; }
    glm::vec3 getHoverColor() const { return hoverColor; }

    void setHovered(bool h) { isHovered = h; }
    bool hovered() const { return isHovered; }

private:
    glm::vec4 pos{};
    std::function<void()> callback{};
    std::string text;

    glm::vec3 bgColor {0.4f, 0.4f, 0.4f};
    glm::vec3 textColor {0.1f, 0.1f, 0.1f};
    glm::vec3 hoverColor {0.6f, 0.6f, 0.6f};
    bool isHovered = false;
};
