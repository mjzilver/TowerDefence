#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

#include "Event.h"

class EventDispatcher {
public:
    using EventListener = std::function<void(const Event&)>;

    void addListener(EventType type, EventListener listener) { listeners[type].push_back(std::move(listener)); }

    void dispatch(const Event& event) {
        if (listeners.find(event.type) != listeners.end()) {
            for (const auto& listener : listeners[event.type]) {
                listener(event);
            }
        }
    }

private:
    std::unordered_map<EventType, std::vector<EventListener>> listeners;
};
