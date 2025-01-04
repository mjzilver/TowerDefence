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

    // get row for current state and direction
    int getRow(Direction dir) {
        std::map<std::pair<State, Direction>, int> stateDirectionRowMap = {
            {{Idle, Direction::South}, 0},
            {{Idle, Direction::North}, 1},
            {{Idle, Direction::East}, 2},
            {{Idle, Direction::West}, 2},
            {{Walking, Direction::South}, 3},
            {{Walking, Direction::North}, 4},
            {{Walking, Direction::East}, 5},
            {{Walking, Direction::West}, 5},
            {{Dead, Direction::South}, 6},
            {{Dead, Direction::North}, 7},
            {{Dead, Direction::East}, 8},
            {{Dead, Direction::West}, 8}
        };

        auto it = stateDirectionRowMap.find({state, dir});
        if (it != stateDirectionRowMap.end()) {
            return it->second;
        } else {
            return 0;
        }
    }
};