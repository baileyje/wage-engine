#include "entity/store.h"



  EntityList EntityStore::with(std::string componentName) {
    EntityList found;
    for (auto entity : pool) {
      if (entity->getComponents()->get(componentName)) {
        found.push_back(entity);
      }
    }
    return found;
  }