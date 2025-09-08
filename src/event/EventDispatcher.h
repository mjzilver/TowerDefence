#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

#include "Event.h"

class EventDispatcher {
public:
    using EventListener = std::function<void(const Event&)>;

    static EventDispatcher& getInstance() {
        static EventDispatcher instance;
        return instance;
    }

    void addListener(EventType type, EventListener listener) {
        listeners[type].push_back(std::move(listener));
    }

    void dispatch(const Event& event) {
        if (listeners.find(event.type) != listeners.end()) {
            for (const auto& listener : listeners[event.type]) {
                listener(event);
            }
        }
    }

private:
    std::unordered_map<EventType, std::vector<EventListener>> listeners;

    EventDispatcher() = default;
    ~EventDispatcher() = default;

    EventDispatcher(const EventDispatcher&) = delete;
    EventDispatcher& operator=(const EventDispatcher&) = delete;
    EventDispatcher(EventDispatcher&&) = delete;
    EventDispatcher& operator=(EventDispatcher&&) = delete;
};
