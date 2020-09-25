#pragma once

#include <vector>

#include "math/vector.h"
#include "asset/asset.h"
#include "render/components/mesh.h"
#include "render/mesh/vertex.h"

namespace wage {

  namespace memory {
    class InputStream;
  }

  namespace render {

    /**
     * Mesh data storage container.
     */
    class Mesh : public asset::Asset {

    public:
      /**
       * The vector of vertices for this mesh.
       */
      std::vector<Vertex> vertices;

      /**
      * The vector of indices for this mesh.
       */
      std::vector<uint32_t> indices;

      /**
       * The max dimensions for this mesh.
       */
      math::Vector3 maxDim;

      /**
       * Create a mesh data object with a mesh spec.
       */
      Mesh(MeshSpec mesh);

      /**
       * Create a mesh data object with the pre-produced mesh data.
       */
      Mesh(std::string key, std::vector<Vertex> vertices, std::vector<uint32_t> indices);

      virtual ~Mesh();

      /**
       * Get the number of elements in the mesh.
       */
      unsigned int elementCount();

      /**
       * Called by the asset manager when the input stream is made availble. This will load mesh data from the input stream provided.
       */
      bool onLoad(memory::InputStream* stream, memory::Allocator* allocator);

      private:
        math::Vector calculateNormals(math::Vector vertex1, math::Vector vertex2, math::Vector vertex3);
    };
  }
}
