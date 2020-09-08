#pragma once

#include "ecs/manager.h"
#include "ecs/system_manager.h"

namespace wage {
  namespace scene {

    class Scene {
    public:
      static Scene& current();

      inline ecs::EntityManager& entities() {
        return _entities;
      }

      inline ecs::SystemManager& systems() {
        return _systems;
      }

    private:
      ecs::EntityManager _entities;
      ecs::SystemManager _systems;
    };

  }
}