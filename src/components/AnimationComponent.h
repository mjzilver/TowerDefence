#pragma once
#include "../ecs/Component.h"
#include "../utils/Direction.h"
#include "../utils/State.h"

#include <glm/glm.hpp>
#include <map>

struct AnimationComponent : public Component {
    int frame;
    
    float frameDuration;
    float time;

    glm::vec2 baseTextureCoords;
    glm::vec2 frameSize;

    // TODO: make state a map not a hardcoded enum
    State state;

    bool loop = true;

    std::map<State, int> frameCount;
    std::map<std::pair<State, Direction>, int> stateDirectionRowMap;

    int getFrameCount() {
        return frameCount[state];
    }

    // TODO: make it so a row doesnt need a direction
    int getRow(Direction dir) {
        auto it = stateDirectionRowMap.find({state, dir});
        if (it != stateDirectionRowMap.end()) {
            return it->second;
        } else {
            return 0;
        }
    }
};