#pragma once

#include <vector>
#include <map>

#include "math/transform.h"
#include "render/components/mesh.h"
#include "render/components/texture.h"

namespace wage::render::vulkan {

  class VulkanRenderContext;
  
  struct MeshGroup {

    MeshSpec meshSpec;

    std::vector<math::Transform> transforms;
  };

  struct TextureGroup {

    TextureSpec textureSpec;

    std::map<std::string, MeshGroup> meshes;
  };

  class ModelTree {

  public:
    void add(TextureSpec texture, MeshSpec mesh, math::Transform transform);

    void render(VulkanRenderContext* context);

  private:
    std::map<std::string, TextureGroup> groups;
  };
}