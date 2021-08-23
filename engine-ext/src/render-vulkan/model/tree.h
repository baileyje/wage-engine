#pragma once

#include <vector>
#include <map>

#include "math/transform.h"
#include "render/components/mesh.h"
#include "render/components/texture.h"

namespace wage::render::vulkan {

  class VulkanRenderContext;

  class ModelTree {

  public:
    void add(TextureSpec texture, MeshSpec mesh, math::Matrix modelMatrix);

    void render(VulkanRenderContext* context);

  private:
    struct MeshGroup {

      MeshSpec meshSpec;

      std::vector<math::Matrix> modelMatrices;
    };

    struct TextureGroup {

      TextureSpec textureSpec;

      std::map<std::string, MeshGroup> meshes;
    };

    std::map<std::string, TextureGroup> groups;
  };
}