#ifndef ENTITY_COMPONENT_MAP_H
#define ENTITY_COMPONENT_MAP_H

#include <unordered_map>

class ComponentMap {



private: 
  
  std::unordered_map<int, Component*> map;

};

#endif //ENTITY_COMPONENT_MAP_H
