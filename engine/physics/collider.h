#ifndef ENTITY_COMPONENT_COLLIDER_H
#define ENTITY_COMPONENT_COLLIDER_H

namespace wage {

  enum class ColliderType { box, sphere, mesh };

  class Collider {
    
  public:

    Collider() : Collider(ColliderType::box) {    
    }

    Collider(ColliderType type) : _type(type) {    
    }

    ~Collider() {}

    inline ColliderType type() const {
      return _type;
    }

    static Collider Box;

    static Collider Sphere;

  private:

    ColliderType _type;

  };

}

#endif //ENTITY_COMPONENT_COLLIDER_H