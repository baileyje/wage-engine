#pragma once

#include "render-vulkan/core/common.h"

namespace wage::render::vulkan {

  class Instance;

  class Surface {
  public:
    VkSurfaceKHR wrapped;

    Surface(Instance* instance);

    void create(GLFWwindow* window);

    void destroy();

  private:
    Instance* instance;
  };
}