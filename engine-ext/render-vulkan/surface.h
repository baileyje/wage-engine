#pragma once

#include "render-vulkan/common.h"

namespace wage {
  namespace render {

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
}