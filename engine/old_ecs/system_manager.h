#pragma once

#include <typeindex>
#include <vector>

#include "core/service.h"
#include "ecs/system.h"
#include "ecs/system_context.h"
#include "ecs/manager.h"

namespace wage {
  namespace ecs {

    typedef std::vector<std::unique_ptr<System>>::iterator SystemIterator;

    class SystemManager : public core::Service {

    private:
    public:
      SystemManager() : Service("SystemManager") {
      }

      void start() override {
        // TODO: What if added after start
        SystemContext context(core::Core::Instance->get<EntityManager>(), core::Core::Instance->frame());
        for (auto system : systems) {
          system->init(context);
          system->start(context);
        }
        core::Core::Instance->onUpdate([&](const core::Frame& frame) {
          SystemContext context(core::Core::Instance->get<EntityManager>(), core::Core::Instance->frame());
          for (auto system : systems) {
            system->update(context);
          }
        });
        core::Core::Instance->onFixedUpdate([&](const core::Frame& frame) {
          SystemContext context(core::Core::Instance->get<EntityManager>(), core::Core::Instance->frame());
          for (auto system : systems) {
            system->fixedUpdate(context);
          }
        });
      }

      template <typename T, typename I, typename... Args>
      I* create(Args... args) {
        auto instance = memory::make<I>(args...);
        add(instance);
        return instance;
      }

      template <typename T, typename... Args>
      T* create(Args... args) {
        return create<T, T>(args...);
      }

      inline void add(System* system) {
        systems.push_back({system});
        // TODO: Start System if already running
      }

    private:
      std::vector<System*> systems;
    };

  }
}