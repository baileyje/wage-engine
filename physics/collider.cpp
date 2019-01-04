#include "physics/collider.h"

namespace wage {

  Collider Collider::Box = Collider(ColliderType::box);

  Collider Collider::Sphere = Collider(ColliderType::sphere);

}