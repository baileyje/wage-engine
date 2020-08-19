#pragma once

#include <vector>
#include <algorithm>
#include <cmath>

#include "math/vector.h"
#include "assets/asset.h"
#include "render/mesh.h"

namespace wage {
  namespace render {

    typedef std::vector<math::Vector> VertexVector;
    typedef std::vector<math::Vector2> Vertex2Vector;
    typedef std::vector<unsigned int> IndexVector;

    class MeshData : public assets::Asset {

    public:
      MeshData(Mesh mesh) : assets::Asset(mesh) {
      }

      MeshData(std::string key, VertexVector vertices, VertexVector normals, Vertex2Vector uvs, IndexVector indices)
          : Asset({"mesh", key}), _vertices(vertices), _normals(normals), _uvs(uvs), _indices(indices), _maxDim(0) {
        for (auto vertex : _vertices) {
          _maxDim.x = std::max(_maxDim.x, std::abs(vertex.x));
          _maxDim.y = std::max(_maxDim.y, std::abs(vertex.y));
          _maxDim.z = std::max(_maxDim.z, std::abs(vertex.z));
        }
        state(AssetState::loaded);
      }

      virtual ~MeshData() {}

      inline unsigned int elementCount() {
        return _indices.size();
      }

      inline VertexVector& vertices() {
        return _vertices;
      }

      inline VertexVector& normals() {
        return _normals;
      }

      inline Vertex2Vector& uvs() {
        return _uvs;
      }

      inline IndexVector& indices() {
        return _indices;
      }

      inline math::Vector maxDim() {
        return _maxDim;
      }

      void onLoad(memory::Buffer buffer);

    protected:
      VertexVector _vertices;

      VertexVector _normals;

      Vertex2Vector _uvs;

      IndexVector _indices;

      math::Vector _maxDim;
    };
  }
}