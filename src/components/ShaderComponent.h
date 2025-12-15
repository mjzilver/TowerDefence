#pragma once

#include <string>

#include "../ecs/Component.h"

struct ShaderComponent : public Component {
    std::string name = "default";
};