#pragma once

#include "../ecs/System.h"

class ClickSystem : public System {
public:
    ClickSystem(EngineContext& ctx);

    void update(float) override;

    void onClick(int button, int action, double x, double y);

    void onHover(double x, double y);

    void reset() override {};

private:
    enum class ClickEventType {
        HOVER_EVENT,
        CLICK_EVENT
    };

    struct ClickEvent {
        Entity entity;
        ClickEventType type;
    };

    std::mutex clickQueueMutex;
    std::vector<ClickEvent> clickQueue;
};