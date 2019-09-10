#pragma once

#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "core/service.h"
#include "util/map.h"

namespace wage { namespace core {

  class ServiceMap {

  public:
    typedef std::vector<Service*>::iterator Iterator;

    template <typename T>
    inline void add(T* service) {
      services.push_back(service);
      byType[typeid(service)] = service;
    }

    inline Iterator begin() {
      return services.begin();
    }

    inline Iterator end() {
      return services.end();
    }

    template <typename T>
    inline T* get() {
      auto itr = byType.find(typeid(T*));
      if (itr != byType.end()) {
        Service* service = itr->second;
        if (service) {
          return dynamic_cast<T*>(service);
        }
      }
      return nullptr;
    }

  private:
    Map<std::type_index, Service*> byType;

    std::vector<Service*> services;
  };

} }
