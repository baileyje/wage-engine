#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>

#include "math/transform.h"
#include "entity/component.h"
#include "entity/component/map.h"
#include "entity/component/dynamic.h"
#include "entity/context.h"
#include "entity/component/func_component.h"

namespace wage {

  typedef unsigned long EntityId;

  #define InvalidEntityId 0

  class Entity {

  public:

    static EntityId nextId() {
      return CurrentId++;
    }

    Entity() {} 

    virtual ~Entity();

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
    
    inline Transform& getTransform() { return transform; }

    inline void setTransform(Transform transform) { this->transform = transform; }

    template <class T>
    inline Entity* add(T* component) { 
      components.add<T>(component);
      Component* asComponent = dynamic_cast<Component*>(component);
      if (asComponent->isDynamic()) {
        DynamicComponent* asDynamic = dynamic_cast<DynamicComponent*>(component);
        dynamicComponents.push_back(asDynamic);
      }
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

    inline ComponentMap<Component>* getComponents() { return &components; }
    
    void start(EntityContext* context);

    void update(EntityContext* context);

    void fixedUpdate(EntityContext* context);

    void stop(EntityContext* context);

  private:
    
    // Entity(EntityId id, Transform transform);

    // virtual ~Entity();

    EntityId id;

    Transform transform;

    ComponentMap<Component> components;

    std::vector<DynamicComponent*> dynamicComponents;

    // std::vector<EntityReference> children;
    
    static EntityId CurrentId;

  };

}

#endif //ENTITY_H