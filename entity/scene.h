#ifndef SCENE_H
#define SCENE_H

#include "entity/entity.h"
#include "entity/manager.h"

namespace wage {

  class Scene {

  public:

    inline EntityReference getCamera() {
      // auto found = entities.with("Camera");
      // if (!found.empty()) {
      //   return found[0];
      // }
      return entityManager->with("Camera")[0];
    }

  private:

    EntityManager* entityManager;

  };

}

#endif //SCENE_H