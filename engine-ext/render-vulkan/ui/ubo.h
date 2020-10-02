#pragma once

// TODO: Move to engine math
#include <glm/glm.hpp>

namespace wage {

  namespace render {

    struct UiUniformBuffer {
      alignas(16) glm::mat4 proj;
    };
  }
}