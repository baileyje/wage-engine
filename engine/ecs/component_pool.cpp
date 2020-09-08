#include "ecs/component_pool.h"

namespace wage {
  namespace ecs {

    ComponentPool* ComponentPool::empty = new ComponentPool(0);
  }
}