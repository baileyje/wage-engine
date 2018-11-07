#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>

#include "math/transform.h"
#include "entity/component_map.h"
#include "entity/component.h"
#include "entity/context.h"
#include "entity/component/func_component.h"

typedef unsigned long EntityId;

#define InvalidEntityId 0

class Entity {

public:

  Entity() {} 

  virtual ~Entity();

  // static Entity* create();

  // static Entity* create(Transform transform);

  // Copy
  Entity(Entity&& src) {
    id = std::move(src.id);
    transform = std::move(src.transform);
    components = std::move(src.components);
  }

  // Move
  Entity& operator=(Entity&& src) {
    id = std::move(src.id);
    transform = std::move(src.transform);
    components = std::move(src.components);
    return *this;
  }

  inline EntityId getId() { return id; }

  inline void setId(EntityId id) { this->id = id; }
  
  inline Transform* getTransform() { return &transform; }

  inline void setTransform(Transform transform) { this->transform = transform; }

  template <typename T>
  inline Entity* add(T* component) { 
    components.add<T>(component);
    return this;
  }

  Entity* add(ComponentCallback func);

  // Entity* add(ComponentCallback& func);
  
  template <typename T>
  inline T* get() { 
    return components.get<T>();
  }

  // inline void add(EntityReference child) {
  //   child->transform.setParent(&transform);
  //   children.push_back(child);
  // }

  // inline std::vector<EntityReference>* getChildren() {
  //   return &children;
  // }

  inline ComponentMap* getComponents() { return &components; }
  
  void start(EntityContext* context);

  void update(EntityContext* context);

  void fixedUpdate(EntityContext* context);

  void stop(EntityContext* context);

private:
  
  // Entity(EntityId id, Transform transform);

  // virtual ~Entity();

  EntityId id;

  Transform transform;

  ComponentMap components;

  // std::vector<EntityReference> children;
  
  static EntityId CurrentId;

};

#endif //ENTITY_H