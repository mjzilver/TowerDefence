#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>

class UIElement {
public:
    virtual ~UIElement() = default;

    void setPos(const glm::vec4& newPos) { pos = newPos; }
    glm::vec4 getPos() const { return pos; }

    void setText(const std::string& newText) { text = newText; }
    const std::string& getText() const { return text; }

    void setTextColor(const glm::vec3& c) { textColor = c; }
    glm::vec3 getTextColor() const { return textColor; }

    virtual void onClick() {}
    virtual void setHovered(bool) {}
    virtual bool hovered() const { return false; }

    void setBgColor(const glm::vec3& c) { bgColor = c; }
    glm::vec3 getBgColor() const { return bgColor; }

    virtual glm::vec3 getHoverColor() const { return {}; }

    virtual bool isClickable() const { return false; }

protected:
    glm::vec4 pos{};
    std::string text;
    glm::vec3 bgColor{0.4f, 0.4f, 0.4f};
    glm::vec3 textColor{0.1f, 0.1f, 0.1f};
};
