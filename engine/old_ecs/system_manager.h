#ifndef ECS_SYSTEM_MANAGER_H
#define ECS_SYSTEM_MANAGER_H

#include <typeindex>
#include <vector>

#include "core/service.h"
#include "old_ecs/system.h"
#include "old_ecs/system_context.h"
#include "old_ecs/entity_manager.h"

namespace wage {

  typedef std::vector<std::unique_ptr<System>>::iterator SystemIterator;

  class SystemManager : public Service {

  private:
  public:
    SystemManager() : Service("SystemManager") {
    }

    void start() override {
      // TODO: What if added after start
      SystemContext context(Core::Instance->get<EntityManager>(), Core::Instance->frame());
      for (auto system : systems) {
        system->init(&context);
        system->start(&context);
      }
      Core::Instance->onUpdate([&](const Frame& frame) {
        SystemContext context(Core::Instance->get<EntityManager>(), Core::Instance->frame());
        for (auto system : systems) {
          system->update(&context);
        }
      });
      Core::Instance->onFixedUpdate([&](const Frame& frame) {
        SystemContext context(Core::Instance->get<EntityManager>(), Core::Instance->frame());
        for (auto system : systems) {
          system->fixedUpdate(&context);
        }
      });
    }

    template <typename T, typename I, typename... Args>
    I* create(Args... args) {
      auto instance = make<I>(args...);
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

#endif // ECS_SYSTEM_MANAGER_H