#pragma once

#include "UIElement.h"
#include <functional>

class UIButton : public UIElement {
public:
    void setCallback(std::function<void()> cb) { callback = cb; }
    void onClick() override { if (callback) callback(); }

    void setHoverColor(const glm::vec3& c) { hoverColor = c; }
    glm::vec3 getHoverColor() const override { return hoverColor; }

    void setHovered(bool h) override { isHovered = h; }
    bool hovered() const override { return isHovered; }

    bool isClickable() const override { return true; }

private:
    std::function<void()> callback{};
    glm::vec3 bgColor {0.4f, 0.4f, 0.4f};
    glm::vec3 hoverColor {0.6f, 0.6f, 0.6f};
    bool isHovered = false;
};
