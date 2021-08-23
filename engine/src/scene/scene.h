#pragma once

#include "ecs/manager.h"
#include "ecs/system_manager.h"

namespace wage {
  namespace scene {

    /**
     * A scene represents a set of entities an systems that make up a portion of the. All data related to a scene will be 
     * cleaned up when transitioning to a new scene.
     */
    class Scene {
    public:
      /**
       * Get access to the currently executing scene.
       */
      static Scene& current();

      /**
       * Get acces to the scene's entity manager to add/remove or interact with entities in the scene.
       */
      inline ecs::EntityManager& entities() {
        return _entities;
      }

      /**
      * Get access to the scene's system manager to add/remove systems from scene.
      */
      inline ecs::SystemManager& systems() {
        return _systems;
      }

    private:
      ecs::EntityManager _entities;
      ecs::SystemManager _systems;
    };

  }
}