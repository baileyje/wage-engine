#include "physics/collider.h"

namespace wage { namespace physics {

  Collider Collider::Box = Collider(ColliderType::box);

  Collider Collider::Sphere = Collider(ColliderType::sphere);

} }