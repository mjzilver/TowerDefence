#include "ComponentManager.h"

void ComponentManager::reorder(Entity entity, ZLayer newLayer) {
    for (auto& [layer, entities] : em.layeredEntities) {
        const auto it = std::find(entities.begin(), entities.end(), entity);
        if (it != entities.end()) {
            entities.erase(it);
        }
    }

    auto& vec = em.layeredEntities[newLayer];
    const auto* positions = getArray<PositionComponent>();

    const auto insertPos = std::upper_bound(vec.begin(), vec.end(), entity, [&](Entity a, Entity b) {
        const auto* pa = positions->get(a);
        const auto* pb = positions->get(b);

        if (!pa || !pb) return a < b;
        return pa->y > pb->y;
    });

    vec.insert(insertPos, entity);
}
