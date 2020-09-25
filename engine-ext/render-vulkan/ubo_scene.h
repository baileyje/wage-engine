#pragma once

// TODO: Move to engine math
#include <glm/glm.hpp>

namespace wage {

  namespace render {

    struct UniformBufferScene {      
      alignas(16) glm::mat4 view;
      alignas(16) glm::mat4 proj;
    };
  }
}