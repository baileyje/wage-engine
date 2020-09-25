#include "render/mesh/mesh.h"

#include <cmath>
#include <unordered_map>

#include <algorithm>

#include "tiny_obj_loader.h"

#include "memory/input_stream.h"
#include "memory/buffer_stream.h"
#include "math/vector.h"

namespace wage {
  namespace render {

    Mesh::Mesh(MeshSpec mesh) : asset::Asset(mesh) {}

    Mesh::Mesh(std::string key, std::vector<Vertex> vertices, std::vector<uint32_t> indices)
        : Asset({"mesh", key}), vertices(vertices), indices(indices), maxDim(0) {
      for (auto vertex : vertices) {
        maxDim.x = std::max(maxDim.x, std::abs(vertex.position.x));
        maxDim.y = std::max(maxDim.y, std::abs(vertex.position.y));
        maxDim.z = std::max(maxDim.z, std::abs(vertex.position.z));
      }
      state(State::loaded);
    }

     Mesh::~Mesh() {}

    inline unsigned int Mesh::elementCount() {
      return indices.size();
    }
    
    bool Mesh::onLoad(memory::InputStream* stream, memory::Allocator* allocator) {
      auto bufferSize = stream->size();
      auto buffer = (memory::Byte*)malloc(bufferSize);
      stream->read(buffer, bufferSize);
      memory::BufferStream bufferStream(buffer, bufferSize);
      std::istream istream(&bufferStream);
      tinyobj::attrib_t attrib;
      std::vector<tinyobj::shape_t> shapes;
      std::vector<tinyobj::material_t> materials;
      std::string warn, err;
      if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, &istream)) {
        throw std::runtime_error(warn + err);  // PUKE.......
      }
      float minX = attrib.vertices[0];
      float maxX = attrib.vertices[0];
      float minY = attrib.vertices[1];
      float maxY = attrib.vertices[1];
      float minZ = attrib.vertices[2];
      float maxZ = attrib.vertices[2];
      std::unordered_map<Vertex, uint32_t> uniqueVertices{};
      for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
          Vertex vertex{};
          vertex.position = {
            attrib.vertices[3 * index.vertex_index + 0],
            attrib.vertices[3 * index.vertex_index + 1],
            attrib.vertices[3 * index.vertex_index + 2]
          };
          vertex.textureCoord = {
            attrib.texcoords[2 * index.texcoord_index + 0],
            1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
          };
          vertex.color = math::Vector3::One;
          vertex.color = {1.0f, 1.0f, 1.0f};
          if (!attrib.normals.empty()) {
            vertex.normal = {
                attrib.normals[3 * index.normal_index + 0],
                attrib.normals[3 * index.normal_index + 1],
                attrib.normals[3 * index.normal_index + 2]
            };
          } else {
            // auto norm = calculateNormals(vertices[index - 3].position, vertices[index - 2].position, vertices[index - 1].position);
            // vertices[index - 3].normal = norm;
            // vertices[index - 2].normal = norm;
            // vertices[index - 1].normal = norm;
            vertex.normal = math::Vector3::Zero;
          }
          if (uniqueVertices.count(vertex) == 0) {
            uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
            vertices.push_back(vertex);
          }
          indices.push_back(uniqueVertices[vertex]);
          if (vertex.position.x < minX) minX = vertex.position.x;
          if (vertex.position.x > maxX) maxX = vertex.position.x;
          if (vertex.position.y < minY) minY = vertex.position.y;
          if (vertex.position.y > maxY) maxY = vertex.position.y;
          if (vertex.position.z < minZ) minZ = vertex.position.z;
          if (vertex.position.z > maxZ) maxZ = vertex.position.z;
          
        }
      }
      // // Determine object scale to force unit s

      std::cout << "MinMaxX " << minX  << ":" << maxX << "\n";
      std::cout << "MinMaxY " << minY << ":" << maxY << "\n";
      std::cout << "MinMaxZ " << minZ << ":" << maxZ << "\n";
      float offsetX = 0.5f * (minX + maxX);
      float offsetY = 0.5f * (minY + maxY);
      float offsetZ = 0.5f * (minZ + maxZ);
      std::cout << "OffsetX " << offsetX << "\n";
      std::cout << "OffsetY " << offsetY << "\n";
      std::cout << "OffsetZ " << offsetZ << "\n";
      float scaleX = abs(minX - offsetX) > abs(maxX - offsetX) ? abs(minX - offsetX) : abs(maxX - offsetX);
      float scaleY = abs(minY - offsetY) > abs(maxY - offsetY) ? abs(minY - offsetY) : abs(maxY - offsetY);
      float scaleZ = abs(minZ - offsetZ) > abs(maxZ - offsetZ) ? abs(minZ - offsetZ) : abs(maxZ - offsetZ);
      std::cout << "ScaleX " << scaleX << "\n";
      std::cout << "ScaleY " << scaleY << "\n";
      std::cout << "ScaleZ " << scaleZ << "\n";
      float scale = scaleX > scaleY ? scaleX : scaleY;
      scale = scaleZ > scale ? 1.0f / scaleZ : 1.0f / scale;
      scale /= 2;
      std::cout << "Scale (" << spec().key() << ") " << scale << "\n";
      for (auto& vertex : vertices) {
        vertex.position.x = scale * (vertex.position.x - offsetX);
        vertex.position.y = scale * (vertex.position.y - offsetY);
        vertex.position.z = scale * (vertex.position.z - offsetZ);
        maxDim.x = std::max(maxDim.x, std::abs(vertex.position.x));
        maxDim.y = std::max(maxDim.y, std::abs(vertex.position.y));
        maxDim.z = std::max(maxDim.z, std::abs(vertex.position.z));
      }
      std::cout << "Vert 1: " << vertices[0].position  << "\n";
      // std::string err;
      // std::string warn;
      // tinyobj::attrib_t attrib;
      // std::vector<tinyobj::shape_t> shapes;
      // std::vector<tinyobj::material_t> materials;
      // auto bufferSize = stream->size();
      // auto buffer = (memory::Byte*)malloc(bufferSize);
      // stream->read(buffer, bufferSize);
      // memory::BufferStream bufferStream(buffer, bufferSize);
      // std::istream istream(&bufferStream);
      // bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, &istream);
      // if (!err.empty()) {
      //   std::cerr << err << std::endl;
      // }
      // if (!warn.empty()) {
      //   std::cerr << warn << std::endl;
      // }
      // if (!ret) {
      //   std::cerr << "Failed to load .obj" << std::endl;
      // }
      // float minX = attrib.vertices[0];
      // float maxX = attrib.vertices[0];
      // float minY = attrib.vertices[1];
      // float maxY = attrib.vertices[1];
      // float minZ = attrib.vertices[2];
      // float maxZ = attrib.vertices[2];
      // unsigned int index = 0;
      // // Loop over shapes
      // for (size_t s = 0; s < 1; s++) {
      //   // Loop over faces(polygon)
      //   size_t index_offset = 0;
      //   for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
      //     int fv = shapes[s].mesh.num_face_vertices[f];

      //     // Loop over vertices in the face.
      //     for (size_t v = 0; v < fv; v++) {
      //       // access to vertex
      //       tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

      //       float vertX = attrib.vertices[3 * idx.vertex_index + 0];
      //       float vertY = attrib.vertices[3 * idx.vertex_index + 1];
      //       float vertZ = attrib.vertices[3 * idx.vertex_index + 2];

      //       if (vertX < minX) {
      //         minX = vertX;
      //       }
      //       if (vertX > maxX) {
      //         maxX = vertX;
      //       }
      //       if (vertY < minY) {
      //         minY = vertY;
      //       }
      //       if (vertY > maxY) {
      //         maxY = vertY;
      //       }
      //       if (vertZ < minZ) {
      //         minZ = vertZ;
      //       }
      //       if (vertZ > maxZ) {
      //         maxZ = vertZ;
      //       }
      //       _vertices.push_back({vertX, vertY, vertZ});
      //       if (!attrib.normals.empty()) {
      //         _normals.push_back({attrib.normals[3 * idx.normal_index + 0],
      //             attrib.normals[3 * idx.normal_index + 1],
      //             attrib.normals[3 * idx.normal_index + 2]});
      //       } else {
      //         _normals.push_back(math::Vector3::Zero);
      //       }
      //       if (!attrib.texcoords.empty()) {
      //         _uvs.push_back({attrib.texcoords[2 * idx.texcoord_index + 0],
      //             attrib.texcoords[2 * idx.texcoord_index + 1]});
      //       } else {
      //         _uvs.push_back(math::Vector2(0.5, 0.5));
      //       }
      //       _indices.push_back(index++);
      //     }
      //     index_offset += fv;
      //     auto norm = calculateNormals(_vertices[index - 3], _vertices[index - 2], _vertices[index - 1]);
      //     _normals[index - 3] = norm;
      //     _normals[index - 2] = norm;
      //     _normals[index - 1] = norm;
      //   }
      // }
      // // Determine object scale to force unit s
      // float offsetX = 0.5f * (minX + maxX);
      // float offsetY = 0.5f * (minY + maxY);
      // float offsetZ = 0.5f * (minZ + maxZ);
      // float scaleX = abs(minX - offsetX) > abs(maxX - offsetX) ? abs(minX - offsetX) : abs(maxX - offsetX);
      // float scaleY = abs(minY - offsetY) > abs(maxY - offsetY) ? abs(minY - offsetY) : abs(maxY - offsetY);
      // float scaleZ = abs(minZ - offsetZ) > abs(maxZ - offsetZ) ? abs(minZ - offsetZ) : abs(maxZ - offsetZ);
      // float scale = scaleX > scaleY ? scaleX : scaleY;
      // scale = scaleZ > scale ? 1.0f / scaleZ : 1.0f / scale;

      return true;
    }

    math::Vector Mesh::calculateNormals(math::Vector3 vertex1, math::Vector3 vertex2, math::Vector3 vertex3) {
      return math::Vector3::cross(vertex3 - vertex1, vertex2 - vertex1).normalized();
    }
  }
}
