#pragma once

#include <vector>
#include <algorithm>
#include <cmath>

#include "tiny_obj_loader.h"

#include "math/vector.h"
#include "assets/asset.h"
#include "render/components/mesh.h"
#include "memory/input_stream.h"
#include "memory/buffer_stream.h"

namespace wage {
  namespace render {

    typedef std::vector<math::Vector> VertexVector;
    typedef std::vector<math::Vector2> Vertex2Vector;
    typedef std::vector<unsigned int> IndexVector;

    class MeshData : public assets::Asset {

    public:
      MeshData(MeshSpec mesh) : assets::Asset(mesh) {
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

      bool onLoad(memory::InputStream* stream) {
        std::string err;
        std::string warn;
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        auto bufferSize = stream->size();
        auto buffer = (memory::Byte*)malloc(bufferSize);
        stream->read(buffer, bufferSize);
        memory::BufferStream bufferStream(buffer, bufferSize);
        std::istream istream(&bufferStream);
        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, &istream);
        if (!err.empty()) {
          std::cerr << err << std::endl;
        }
        if (!warn.empty()) {
          std::cerr << warn << std::endl;
        }
        if (!ret) {
          std::cerr << "Failed to load .obj" << std::endl;
        }

        float minX = attrib.vertices[0];
        float maxX = attrib.vertices[0];
        float minY = attrib.vertices[1];
        float maxY = attrib.vertices[1];
        float minZ = attrib.vertices[2];
        float maxZ = attrib.vertices[2];

        unsigned int index = 0;
        // Loop over shapes
        for (size_t s = 0; s < 1; s++) {
          // Loop over faces(polygon)
          size_t index_offset = 0;
          for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
              // access to vertex
              tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

              float vertX = attrib.vertices[3 * idx.vertex_index + 0];
              float vertY = attrib.vertices[3 * idx.vertex_index + 1];
              float vertZ = attrib.vertices[3 * idx.vertex_index + 2];

              if (vertX < minX) {
                minX = vertX;
              }
              if (vertX > maxX) {
                maxX = vertX;
              }
              if (vertY < minY) {
                minY = vertY;
              }
              if (vertY > maxY) {
                maxY = vertY;
              }
              if (vertZ < minZ) {
                minZ = vertZ;
              }
              if (vertZ > maxZ) {
                maxZ = vertZ;
              }
              _vertices.push_back({vertX, vertY, vertZ});
              if (!attrib.normals.empty()) {
                _normals.push_back({attrib.normals[3 * idx.normal_index + 0],
                                    attrib.normals[3 * idx.normal_index + 1],
                                    attrib.normals[3 * idx.normal_index + 2]});
              } else {
                _normals.push_back(math::Vector3::Zero);
              }
              if (!attrib.texcoords.empty()) {
                _uvs.push_back({attrib.texcoords[2 * idx.texcoord_index + 0],
                                attrib.texcoords[2 * idx.texcoord_index + 1]});
              } else {
                _uvs.push_back(math::Vector2(0.5, 0.5));
              }
              _indices.push_back(index++);
            }
            index_offset += fv;
            auto norm = calculateNormals(_vertices[index - 3], _vertices[index - 2], _vertices[index - 1]);
            _normals[index - 3] = norm;
            _normals[index - 2] = norm;
            _normals[index - 1] = norm;
          }
        }
        // Determine object scale to force unit s
        float offsetX = 0.5f * (minX + maxX);
        float offsetY = 0.5f * (minY + maxY);
        float offsetZ = 0.5f * (minZ + maxZ);
        float scaleX = abs(minX - offsetX) > abs(maxX - offsetX) ? abs(minX - offsetX) : abs(maxX - offsetX);
        float scaleY = abs(minY - offsetY) > abs(maxY - offsetY) ? abs(minY - offsetY) : abs(maxY - offsetY);
        float scaleZ = abs(minZ - offsetZ) > abs(maxZ - offsetZ) ? abs(minZ - offsetZ) : abs(maxZ - offsetZ);
        float scale = scaleX > scaleY ? scaleX : scaleY;
        scale = scaleZ > scale ? 1.0f / scaleZ : 1.0f / scale;

        for (auto& vert : _vertices) {
          vert.x = scale * (vert.x - offsetX);
          vert.y = scale * (vert.y - offsetY);
          vert.z = scale * (vert.z - offsetZ);
        }
        return true;
      }

    protected:
      math::Vector calculateNormals(math::Vector vertex1, math::Vector vertex2, math::Vector vertex3) {
        return math::Vector3::cross(vertex3 - vertex1, vertex2 - vertex1).normalized();
      }

      VertexVector _vertices;

      VertexVector _normals;

      Vertex2Vector _uvs;

      IndexVector _indices;

      math::Vector _maxDim;
    };
  }
}