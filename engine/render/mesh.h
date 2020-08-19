#pragma once

#include <string>

#define MeshComponent 30

namespace wage {
  namespace render {

    enum class MeshType { cube,
                          sphere,
                          quad,
                          custom };

    class Mesh {

    public:
      Mesh() {}

      Mesh(std::string key, MeshType type) : _key(key), _type(type) {
      }

      Mesh(Mesh* templateMesh) : _key(templateMesh->key()), _type(templateMesh->type()) {}

      virtual ~Mesh() {
      }

      inline MeshType type() {
        return _type;
      }

      inline std::string key() const {
        return _key;
      }

      static Mesh* Cube;

      static Mesh* Sphere;

      static Mesh* Quad;

    protected:
      std::string _key;

      MeshType _type = MeshType::cube;
    };
  }
}