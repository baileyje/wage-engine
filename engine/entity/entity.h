#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>

#include "engine/math/transform.h"

namespace wage {

  // TODO: Move to network safe ids
  typedef unsigned long EntityId;

  #define InvalidEntityId 0

  class Entity {

  public:

    Entity() : id(nextId()) {} 

    virtual ~Entity();

    // Copy
    Entity(Entity&& src) {
      id = std::move(src.id);
      transform = std::move(src.transform);
    }

    // Move
    Entity& operator=(Entity&& src) {
      id = std::move(src.id);
      transform = std::move(src.transform);
      return *this;
    }

    inline EntityId getId() { return id; }

    inline void setId(EntityId id) { this->id = id; }
    
    inline Transform& getTransform() { return transform; }

    inline void setTransform(Transform transform) { this->transform = transform; }

    // inline void setupComponent(Component* component) {
    //   component->setTransform(&transform);
    //   if (component->isDynamic()) {
    //     DynamicComponent* asDynamic = dynamic_cast<DynamicComponent*>(component);
    //     dynamicComponents.push_back(asDynamic);
    //   }
    // }

    // template <typename T, typename... Args>
    // inline Entity* add(Args... args) {
    //   auto component = components.add<T>(args...);
    //   Component* asComponent = dynamic_cast<Component*>(component);
    //   setupComponent(asComponent);
    //   return this;
    // }

    // template <typename T, typename... Args>
    // inline Entity* create(Args... args) {
    //   auto component = components.add<T>(args...);
    //   Component* asComponent = dynamic_cast<Component*>(component);
    //   setupComponent(asComponent);
    //   return this;
    // }

    // template <class T>
    // inline Entity* add(T* component) { 
    //   components.addComponent<T>(component);
    //   Component* asComponent = dynamic_cast<Component*>(component);
    //   setupComponent(asComponent);
    //   return this;
    // }

    // Entity* onUpdate(ComponentCallback func);

    // template <typename T>
    // inline T* get() { 
    //   return components.get<T>();
    // }

    // inline ComponentMap<Component>* getComponents() { return &components; }
    
    // void start(EntityContext* context);

    // void update(EntityContext* context);

    // void fixedUpdate(EntityContext* context);

    // void stop(EntityContext* context);

  private:
    
    static EntityId nextId() {
      return CurrentId++;
    }

    EntityId id;

    Transform transform;

    // ComponentMap<Component> components;

    // std::vector<DynamicComponent*> dynamicComponents;

    static EntityId CurrentId;

  };

}

#endif //ENTITY_H