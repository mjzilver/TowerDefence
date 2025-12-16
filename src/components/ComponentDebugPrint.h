#pragma once
#include <ostream>
#include "CollisionComponent.h"
#include "PositionComponent.h"
#include "SizeComponent.h"
#include "SpeedComponent.h"
#include "TextureComponent.h"
#include "TextComponent.h"
#include "PathfindingComponent.h"
#include "HealthComponent.h"
#include "DeathComponent.h"
#include "VelocityComponent.h"


inline std::ostream& operator<<(std::ostream& os, const PositionComponent& pos) {
    os << "(" << pos.x << "," << pos.y << ") z: " << (int)pos.zIndex;
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const SizeComponent& size) {
    os << "w:" << size.w << " h:" << size.h;
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const VelocityComponent& vel) {
    os << "vx:" << vel.x << " vy:" << vel.y;
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const CollisionComponent& col) {
    os << "Collision(x:" << col.x << ", y:" << col.y 
       << ", w:" << col.w << ", h:" << col.h 
       << ", solid:" << col.solid << ", colliding:" << col.colliding << ")";
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const SpeedComponent& speed) {
    os << "Speed(" << speed.speed << ")";
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const TextureComponent& tex) {
    os << "Texture(coords: [" 
       << tex.coords.x << "," << tex.coords.y << "," 
       << tex.coords.z << "," << tex.coords.w << "], flipped:" << tex.flipped << ")";
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const TextComponent& text) {
    os << "Text(\"" << text.text << "\", color: [" 
       << text.color.r << "," << text.color.g << "," << text.color.b << "])";
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const PathfindingComponent& path) {
    os << "Pathfinding(x:" << path.x << ", y:" << path.y
       << ", currentIndex:" << path.currentIndex
       << ", reachedGoal:" << path.reachedGoal
       << ", randomOffset:[" << path.randomOffset.x << "," << path.randomOffset.y << "])";
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const HealthComponent& health) {
    os << "Health(" << health.health << ")";
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const DeathComponent& death) {
    os << "Death(remainingTime:" << death.remainingTime
       << ", hasDied:" << death.hasDied << ")";
    return os;
}
