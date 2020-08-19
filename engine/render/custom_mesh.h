#pragma once

#include <string>

#include "render/mesh.h"

#define MeshComponent 30

namespace wage {
  namespace render {

    class CustomMesh : public Mesh {

    public:
      CustomMesh() {}

      CustomMesh(std::string path) : Mesh(path, MeshType::custom), _path(path) {
      }

      virtual CustomMesh::~CustomMesh() {
      }

      inline std::string path() const {
        return _path;
      }

      static Mesh* Cube;

      static Mesh* Sphere;

      static Mesh* Quad;

    protected:
      std::string _path;
    };
  }
}