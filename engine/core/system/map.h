#ifndef CORE_SYSTEM_MAP_H
#define CORE_SYSTEM_MAP_H

#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "ecs/system.h"
#include "util/map.h"

namespace wage {

  typedef Map<std::type_index, System*>::ValueIterator SystemtIterator;

  class SystemMap {

  private:

  public:

    SystemMap() {}

    ~SystemMap() {}

    template <typename T>
    inline void add(T* system) {
      map[typeid(system)] = system;
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

#endif //CORE_SYSTEM_MAP_H
