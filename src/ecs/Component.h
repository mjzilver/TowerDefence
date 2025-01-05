#pragma once
#include <cstdint>

using Entity = std::uint32_t;
const Entity INVALID_ENTITY = -1; // underflow

// Base Component class
class Component {
public:
    virtual ~Component() = default;
};