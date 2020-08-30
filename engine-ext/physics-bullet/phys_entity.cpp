#include "physics-bullet/phys_entity.h"

#include "memory/allocator.h"

namespace wage {
  namespace physics {

    btCollisionShape* PhysicsEntity::shapeFor(ecs::Entity entity) {
      if (!entity.has(ColliderComponent)) {
        return memory::make<btEmptyShape>();
      }
      auto scale = entity.get<math::Transform>(TransformComponent)->scale();
      switch (entity.get<Collider>(ColliderComponent)->type()) {
      // TODO: Support more shapes
      case ColliderType::box: {
        btVector3 halfExtents = btVector3(scale.x / 2.0, scale.y / 2.0, scale.z / 2.0);
        return memory::make<btBoxShape>(halfExtents);
      }
      case ColliderType::sphere: {
        return memory::make<btSphereShape>(scale.x / 2.0);
      }
      default:
        return nullptr;
      }
    }
  }
}