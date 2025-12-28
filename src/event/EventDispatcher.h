#pragma once

#include <functional>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "Event.h"

class EventDispatcher {
public:
    using EventListener = std::function<void(const Event&)>;

    void addListener(EventType type, EventListener listener) { listeners[type].push_back(std::move(listener)); }

    void dispatch(const Event& event) {
        std::lock_guard<std::mutex> lock(queueMutex);
        writeQueue.push_back(event);
    }

    void swapQueues() {
        std::lock_guard<std::mutex> lock(queueMutex);
        readQueue.swap(writeQueue);
        writeQueue.clear();
    }

    void deliver() {
        for (const auto& event : readQueue) {
            auto it = listeners.find(event.type);
            if (it != listeners.end()) {
                for (const auto& listener : it->second) {
                    listener(event);
                }
            }
        }
        readQueue.clear();
    }

private:
    std::unordered_map<EventType, std::vector<EventListener>> listeners;

    std::vector<Event> writeQueue;
    std::vector<Event> readQueue;
    std::mutex queueMutex;
};
