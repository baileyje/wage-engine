#pragma once

#include <vector>
#include <map>

#include "math/transform.h"
#include "render/components/mesh.h"
#include "render/components/texture.h"

namespace wage::render::vulkan {

  class VulkanRenderContext;


  class WireframeTree {

  public:
    void add(MeshSpec mesh, math::Matrix modelMatrix);

    void render(VulkanRenderContext* context);

  private:
    
    struct MeshGroup {

      MeshSpec meshSpec;

      std::vector<math::Matrix> modelMatrices;
    };

    std::map<std::string, MeshGroup> groups;
  };
}