#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>

#include "math/transform.h"
#include "entity/component_map.h"
#include "entity/component.h"
#include "entity/context.h"
#include "entity/component/func_component.h"

class Entity {

public:

  static Entity* create();

  static Entity* create(Transform transform);

  inline long getId() { return id; }
  
  inline Transform* getTransform() { return &transform; }

  inline void setTransform(Transform transform) { this->transform = transform; }

  template <typename T>
  inline Entity* add(T* component) { 
    components.add<T>(component);
    return this;
  }

  Entity* add(ComponentCallback func);
  
  template <typename T>
  inline T* get() { 
    return components.get<T>();
  }

  inline void add(Entity* child) {
    child->transform.setParent(&transform);
    children.push_back(child);
  }

  inline std::vector<Entity*>* getChildren() {
    return &children;
  }

  inline ComponentMap* getComponents() { return &components; }
  
  void start(EntityContext* context);

  void update(EntityContext* context);

  void fixedUpdate(EntityContext* context);

  void stop(EntityContext* context);

private:
  
  Entity(long id, Transform transform);

  virtual ~Entity();

  long id;

  Transform transform;

  ComponentMap components;

  std::vector<Entity*> children;
  
  static long CurrentId;

};

#endif //ENTITY_H