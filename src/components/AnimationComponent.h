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

    // holds framecount per state
    std::map<State, int> frameCount;

    // get framecount for current state
    int getFrameCount() {
        return frameCount[state];
    }

    // get row for current state
    // TODO: make this a map too?
    int getRow() {
        switch (state) {
        case Idle:
            return 0;
        case Walking:
            return 3;
        case Attacking:
            return 6;
        default:
            return 0;
        }
    }
};