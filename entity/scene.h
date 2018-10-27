#ifndef SCENE_H
#define SCENE_H

#include "entity/entity.h"
#include "entity/store.h"

class Scene {

public:

  inline Scene* add(Entity* entity) {
    entities.add(entity);
    return this;
  }

  inline Entity* getCamera() {
    auto found = entities.with("Camera");
    if (!found.empty()) {
      return found[0];
    }
    return nullptr;
  }

  inline EntityStore* getEntities() {
    return &entities;
  }

private:

  EntityStore entities;

};


#endif //SCENE_H