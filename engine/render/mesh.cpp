#include "render/mesh.h"

#include <vector>
#include <iostream>

#include "memory/allocator.h"

namespace wage {
  namespace render {

    Mesh* Mesh::Cube = new Mesh("CubeMesh", MeshType::cube);
    Mesh* Mesh::Sphere = new Mesh("SphereMesh", MeshType::sphere);
    Mesh* Mesh::Quad = new Mesh("QuadMesh", MeshType::quad);

  }
}