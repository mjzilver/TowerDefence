#pragma once

#include "../ecs/Component.h"
#include <string>

struct ShaderComponent : public Component {
    std::string name = "default";
};