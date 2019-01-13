#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>
#include <cstdint>

#include "math/transform.h"

namespace wage {

  typedef uint32_t EntityId;

  #define InvalidEntityId 0

  class Entity {

  public:

    Entity() : _id(nextId()) {} 

    virtual ~Entity();

    // Copy
    Entity(Entity&& src) {
      _id = std::move(src._id);
      _transform = std::move(src._transform);
    }

    // Move
    Entity& operator=(Entity&& src) {
      _id = std::move(src._id);
      _transform = std::move(src._transform);
      return *this;
    }

    inline EntityId id() { return _id; }

    inline void id(EntityId id) { _id = id; }
    
    inline Transform& transform() { return _transform; }

    inline void transform(Transform transform) { _transform = transform; }

  private:
    
    static EntityId nextId() {
      return CurrentId++;
    }

    EntityId _id;

    Transform _transform;

    static EntityId CurrentId;

  };

}

#endif //ENTITY_H