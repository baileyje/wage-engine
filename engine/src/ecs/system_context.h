#pragma once

#include <string>
#include <vector>

#include "core/frame.h"
#include "ecs/manager.h"

namespace wage {
  namespace ecs {

    class System;

    class SystemContext {

    public:
      SystemContext(const EntityManager* entityManager, const core::Frame& frame) : _entityManager(entityManager), frame(frame) {}

      virtual ~SystemContext() {}

      inline double fixedTimeStep() const {
        return frame.fixedTimeStep();
      }

      inline double time() const {
        return frame.time();
      }

      inline double deltaTime() const {
        return frame.deltaTime();
      }

      inline const EntityManager* entityManager() const {
        return _entityManager;
      }

    private:
      const EntityManager* _entityManager;

      const core::Frame frame;
    };

  }
}
