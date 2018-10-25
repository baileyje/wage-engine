#ifndef ENTITY_COMPONENT_MAP_H
#define ENTITY_COMPONENT_MAP_H

#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "entity/component.h"

typedef std::vector<Component*>::iterator ComponentIterator;

class ComponentMap {

public:

  ComponentMap() {}

  ~ComponentMap() {}

  template <typename T>
  inline void add(T* component) {
    values.push_back(component);
    std::string name = component->getName();
    map[name].push_back(component);
    nameMap[typeid(component)] = name;
  }

  inline ComponentIterator begin() {
    return values.begin();
  }

  inline ComponentIterator end() {
    return values.end();
  }
  
  template <typename T>
  inline T* get() {
    auto itr = nameMap.find(typeid(T*));
    if (itr != nameMap.end()) {
      Component* comp = get(itr->second);
      if (comp) {
        return dynamic_cast<T*>(comp);
      }
    }
    return nullptr;
  }

  inline Component* get(std::string name) {
    auto itr = map.find(name);
    if (itr != map.end()) {
      auto byName = itr->second;
      if (!byName.empty()) {
        return byName[0];
      }
    }
    return nullptr;
  }

private: 
  
  std::vector<Component*> values;

  std::unordered_map<std::string, std::vector<Component*>> map;

  std::unordered_map<std::type_index, std::string> nameMap;

};

#endif //ENTITY_COMPONENT_MAP_H
