#pragma once
#include <cstdint>

using Entity = std::uint16_t;
static const Entity INVALID_ENTITY = UINT16_MAX;

class Component {
public:
    virtual ~Component() = default;
};