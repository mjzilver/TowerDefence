#include "SystemManager.h"

#include <iostream>

#include "../utils/Globals.h"

void SystemManager::generateBatches() {
    batches.clear();

    for (auto& [type, sys] : systems) {
        if (sys->excludeFromBatching) continue;

        bool placed = false;

        for (auto& batch : batches) {
            bool conflict = false;

            for (System* other : batch) {
                for (auto& w : sys->writes) {
                    if (std::find(other->writes.begin(), other->writes.end(), w) != other->writes.end() ||
                        std::find(other->reads.begin(), other->reads.end(), w) != other->reads.end()) {
                        conflict = true;
                        break;
                    }
                }
                for (auto& r : sys->reads) {
                    if (std::find(other->writes.begin(), other->writes.end(), r) != other->writes.end()) {
                        conflict = true;
                        break;
                    }
                }
                if (conflict) break;
            }

            if (!conflict) {
                batch.push_back(sys);
                placed = true;
                break;
            }
        }

        if (!placed) {
            batches.push_back({sys});
        }
    }

    if constexpr (DEBUG_ENABLED) {
        debugPrintBatches();
    }
}

void SystemManager::debugPrintBatches() {
    std::cout << "System batches:\n";
    for (size_t i = 0; i < batches.size(); ++i) {
        std::cout << "Batch " << i << ": ";
        for (System* sys : batches[i]) {
            std::cout << typeid(*sys).name() << " ";
        }
        std::cout << "\n";
    }
}
