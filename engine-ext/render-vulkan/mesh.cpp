#include "render-vulkan/mesh.h"

namespace wage {
  namespace render {

  //   bool Mesh::onLoad(memory::InputStream* stream, memory::Allocator* allocator) {
  //     auto bufferSize = stream->size();
  //     auto buffer = (memory::Byte*)malloc(bufferSize);
  //     stream->read(buffer, bufferSize);
  //     memory::BufferStream bufferStream(buffer, bufferSize);
  //     std::istream istream(&bufferStream);

  //     tinyobj::attrib_t attrib;
  //     std::vector<tinyobj::shape_t> shapes;
  //     std::vector<tinyobj::material_t> materials;
  //     std::string warn, err;
  //     if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, &istream)) {
  //       throw std::runtime_error(warn + err);
  //     }
  //     std::unordered_map<Vertex, uint32_t> uniqueVertices{};
  //     for (const auto& shape : shapes) {
  //       for (const auto& index : shape.mesh.indices) {
  //         Vertex vertex{};
  //         vertex.pos = {
  //             attrib.vertices[3 * index.vertex_index + 0],
  //             attrib.vertices[3 * index.vertex_index + 1],
  //             attrib.vertices[3 * index.vertex_index + 2]
  //         };
  //         vertex.texCoord = {
  //             attrib.texcoords[2 * index.texcoord_index + 0],
  //             1.0f - attrib.texcoords[2 * index.texcoord_index + 1]};
  //         vertex.color = {1.0f, 1.0f, 1.0f};
  //         if (uniqueVertices.count(vertex) == 0) {
  //           uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
  //           vertices.push_back(vertex);
  //         }
  //         indices.push_back(uniqueVertices[vertex]);
  //         maxDim.x = std::max(maxDim.x, std::abs(vertex.pos.x));
  //         maxDim.y = std::max(maxDim.y, std::abs(vertex.pos.y));
  //         maxDim.z = std::max(maxDim.z, std::abs(vertex.pos.z));
  //       }
  //     }
  //     return true;
  //   }
  }
}