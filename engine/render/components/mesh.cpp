#include "render/components/mesh.h"

#include <vector>
#include <iostream>

#include "memory/allocator.h"

namespace wage {
  namespace render {

    asset::AssetSpec::Key MeshSpec::Cube = "CubeMesh";
    asset::AssetSpec::Key MeshSpec::Sphere = "SphereMesh";
    asset::AssetSpec::Key MeshSpec::Quad = "QuadMesh";

    MeshSpec::MeshSpec() : MeshSpec(Cube) {}

    MeshSpec::MeshSpec(std::string key) : asset::AssetSpec("mesh", key) {}

    MeshSpec::~MeshSpec() {}
  }
}