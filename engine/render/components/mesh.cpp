#include "render/components/mesh.h"

#include <vector>
#include <iostream>

#include "memory/allocator.h"

namespace wage {
  namespace render {

    assets::AssetSpec::Key MeshSpec::Cube = "CubeMesh";
    assets::AssetSpec::Key MeshSpec::Sphere = "SphereMesh";
    assets::AssetSpec::Key MeshSpec::Quad = "QuadMesh";
  }
}