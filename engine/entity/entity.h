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

    Entity() : id_(nextId()) {} 

    virtual ~Entity();

    // Copy
    Entity(Entity&& src) {
      id_ = std::move(src.id_);
      transform_ = std::move(src.transform_);
    }

    // Move
    Entity& operator=(Entity&& src) {
      id_ = std::move(src.id_);
      transform_ = std::move(src.transform_);
      return *this;
    }

    inline EntityId id() { return id_; }

    inline void setId(EntityId id) { id_ = id; }
    
    inline Transform& transform() { return transform_; }

    inline void setTransform(Transform transform) { transform_ = transform; }

  private:
    
    static EntityId nextId() {
      return CurrentId++;
    }

    EntityId id_;

    Transform transform_;

    static EntityId CurrentId;

  };

}

#endif //ENTITY_H