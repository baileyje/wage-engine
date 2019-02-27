#ifndef ECS_SYSTEM_MANAGER_H
#define ECS_SYSTEM_MANAGER_H

#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "core/service.h"
#include "ecs/system.h"
#include "core/system/context.h"
#include "util/map.h"

namespace wage {

  typedef Map<std::type_index, System*>::ValueIterator SystemtIterator;

  class SystemManager : public Service {

  private:

  public:

    SystemManager() : Service("SystemManager") {
    }

    void start() override {
      // TODO: What if added after start
      SystemContext context(Core::Instance);
      for (auto system : *this) {
        system->init(&context);
        system->start(&context);
      }
      Core::Instance->onUpdate([&](const Frame& frame) {
        SystemContext context(Core::Instance);
        for (auto system : *this) {
          system->update(&context);
        }
      });
      Core::Instance->onFixedUpdate([&](const Frame& frame) {
        SystemContext context(Core::Instance);
        for (auto system : *this) {
          system->fixedUpdate(&context);
        }
      });
    }

    template <typename T, typename I, typename... Args>
    I* create(Args... args) {  
      auto instance = make<I>(args...);
      add<T>(instance);
      return instance;
    }

    template <typename T, typename... Args>
    T* create(Args... args) {  
      return create<T, T>(args...);
    }

    template <typename T>
    inline void add(T* system) {
      map[typeid(system)] = system;
      SystemContext context(Core::Instance);      
    }

    inline SystemtIterator begin() {
      return map.valuesBegin();
    }

    inline SystemtIterator end() {
      return map.valuesEnd();
    }
    
    template <typename T>
    inline T* get() {
      auto itr = map.find(typeid(T*));
      if (itr != map.end()) {
        System* system = itr->second;
        if (system) {
          return dynamic_cast<T*>(system);
        }
      }
      return nullptr;
    }

  private: 
    
    Map<std::type_index, System*> map;

  };

}

#endif // ECS_SYSTEM_MANAGER_H