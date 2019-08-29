#include "render/mesh.h"

#include <vector>
#include <iostream>

#include "memory/allocator.h"
#include "tiny_obj_loader.h"

namespace wage {

  Mesh::~Mesh() {
  }

  Mesh* Mesh::Cube = new Mesh("CubeMesh",
                              VertexVector{
                                  //front
                                  Vector(-0.5, -0.5, 0.5),
                                  Vector(0.5, -0.5, 0.5),
                                  Vector(0.5, 0.5, 0.5),
                                  Vector(-0.5, 0.5, 0.5),
                                  //right
                                  Vector(0.5, 0.5, 0.5),
                                  Vector(0.5, 0.5, -0.5),
                                  Vector(0.5, -0.5, -0.5),
                                  Vector(0.5, -0.5, 0.5),
                                  //back
                                  Vector(-0.5, -0.5, -0.5),
                                  Vector(0.5, -0.5, -0.5),
                                  Vector(0.5, 0.5, -0.5),
                                  Vector(-0.5, 0.5, -0.5),
                                  //left
                                  Vector(-0.5, -0.5, -0.5),
                                  Vector(-0.5, -0.5, 0.5),
                                  Vector(-0.5, 0.5, 0.5),
                                  Vector(-0.5, 0.5, -0.5),
                                  //upper
                                  Vector(0.5, 0.5, 0.5),
                                  Vector(-0.5, 0.5, 0.5),
                                  Vector(-0.5, 0.5, -0.5),
                                  Vector(0.5, 0.5, -0.5),
                                  //bottom
                                  Vector(-0.5, -0.5, -0.5),
                                  Vector(0.5, -0.5, -0.5),
                                  Vector(0.5, -0.5, 0.5),
                                  Vector(-0.5, -0.5, 0.5)},
                              VertexVector{
                                  //front
                                  Vector(0.0f, 0.0f, 1.0f),
                                  Vector(0.0f, 0.0f, 1.0f),
                                  Vector(0.0f, 0.0f, 1.0f),
                                  Vector(0.0f, 0.0f, 1.0f),
                                  //right
                                  Vector(1.0f, 0.0f, 0.0f),
                                  Vector(1.0f, 0.0f, 0.0f),
                                  Vector(1.0f, 0.0f, 0.0f),
                                  Vector(1.0f, 0.0f, 0.0f),
                                  //back
                                  Vector(0.0f, 0.0f, -1.0f),
                                  Vector(0.0f, 0.0f, -1.0f),
                                  Vector(0.0f, 0.0f, -1.0f),
                                  Vector(0.0f, 0.0f, -1.0f),
                                  //left
                                  Vector(-1.0f, 0.0f, 0.0f),
                                  Vector(-1.0f, 0.0f, 0.0f),
                                  Vector(-1.0f, 0.0f, 0.0f),
                                  Vector(-1.0f, 0.0f, 0.0f),
                                  //upper
                                  Vector(0.0f, 1.0f, 0.0f),
                                  Vector(0.0f, 1.0f, 0.0f),
                                  Vector(0.0f, 1.0f, 0.0f),
                                  Vector(0.0f, 1.0f, 0.0f),
                                  //bottom
                                  Vector(0.0f, -1.0f, 0.0f),
                                  Vector(0.0f, -1.0f, 0.0f),
                                  Vector(0.0f, -1.0f, 0.0f),
                                  Vector(0.0f, -1.0f, 0.0f),
                              },
                              Vertex2Vector{
                                  Vector2(0.0f, 1.0f), // bottom left
                                  Vector2(1.0f, 1.0f), // bottom right
                                  Vector2(1.0f, 0.0f), // top right
                                  Vector2(0.0f, 0.0f), // top left
                                  Vector2(0.0f, 1.0f), // bottom left
                                  Vector2(1.0f, 1.0f), // bottom right
                                  Vector2(1.0f, 0.0f), // top right
                                  Vector2(0.0f, 0.0f), // top left
                                  Vector2(0.0f, 1.0f), // bottom left
                                  Vector2(1.0f, 1.0f), // bottom right
                                  Vector2(1.0f, 0.0f), // top right
                                  Vector2(0.0f, 0.0f), // top left
                                  Vector2(0.0f, 1.0f), // bottom left
                                  Vector2(1.0f, 1.0f), // bottom right
                                  Vector2(1.0f, 0.0f), // top right
                                  Vector2(0.0f, 0.0f), // top left
                                  Vector2(0.0f, 1.0f), // bottom left
                                  Vector2(1.0f, 1.0f), // bottom right
                                  Vector2(1.0f, 0.0f), // top right
                                  Vector2(0.0f, 0.0f), // top left
                                  Vector2(0.0f, 1.0f), // bottom left
                                  Vector2(1.0f, 1.0f), // bottom right
                                  Vector2(1.0f, 0.0f), // top right
                                  Vector2(0.0f, 0.0f), // top left
                                  Vector2(0.0f, 1.0f), // bottom left
                                  Vector2(1.0f, 1.0f), // bottom right
                                  Vector2(1.0f, 0.0f), // top right
                                  Vector2(0.0f, 0.0f), // top left
                                  Vector2(0.0f, 1.0f), // bottom left
                                  Vector2(1.0f, 1.0f), // bottom right
                                  Vector2(1.0f, 0.0f), // top right
                                  Vector2(0.0f, 0.0f), // top left
                              },
                              IndexVector{
                                  0, 1, 2, 0, 2, 3,       //front
                                  4, 5, 6, 4, 6, 7,       //right
                                  8, 9, 10, 8, 10, 11,    //back
                                  12, 13, 14, 12, 14, 15, //left
                                  16, 17, 18, 16, 18, 19, //upper
                                  20, 21, 22, 20, 22, 23  // bottom
                              });

  Mesh* Mesh::Quad = new Mesh(
      "QuadMesh",
      VertexVector{
          Vector(0.5, 0.5, 0.0f),     // top right
          Vector(0.5f, -0.5f, 0.0f),  // bottom right
          Vector(-0.5f, -0.5f, 0.0f), // bottom left
          Vector(-0.5f, 0.5f, 0.0f)   // top left
      },
      VertexVector{
          Vector(0.0f, 0.0f, -1.0f), // top right
          Vector(0.0f, 0.0f, -1.0f), // bottom right
          Vector(0.0f, 0.0f, -1.0f), // bottom left
          Vector(0.0f, 0.0f, -1.0f)  // top left
      },
      Vertex2Vector{
          Vector2(1.0f, 0.0f), // top right
          Vector2(1.0f, 1.0f), // bottom right
          Vector2(0.0f, 1.0f), // bottom left
          Vector2(0.0f, 0.0f)  // top left
      },
      IndexVector{
          0, 1, 3, // first Triangle
          1, 2, 3  // second Triangle
      });

  Mesh* Mesh::Sphere = new Mesh("SphereMesh", {}, {}, {}, {});

  void generateSphere() {
    VertexVector vertices;
    VertexVector normals;
    Vertex2Vector texts;
    IndexVector indices;

    float radius = 0.5;
    unsigned int rings = 12;
    unsigned int sectors = 24;

    float const R = 1. / (float)(rings - 1);
    float const S = 1. / (float)(sectors - 1);
    int r, s;

    vertices.resize(rings * sectors);
    normals.resize(rings * sectors);
    texts.resize(rings * sectors);

    VertexVector::iterator v = vertices.begin();
    VertexVector::iterator n = normals.begin();
    Vertex2Vector::iterator t = texts.begin();
    for (r = 0; r < rings; r++)
      for (s = 0; s < sectors; s++) {
        float const y = sin(-M_PI_2 + M_PI * r * R);
        float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
        float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);
        *t++ = Vector2(s * S, r * R);

        *v++ = Vector(x * radius, y * radius, z * radius);

        *n++ = Vector(x, y, z);
      }

    indices.resize(rings * sectors * 6);
    IndexVector::iterator i = indices.begin();
    for (r = 0; r < rings; r++)
      for (s = 0; s < sectors; s++) {
        int curRow = r * sectors;
        int nextRow = (r + 1) * sectors;
        int nextS = (s + 1) % sectors;
        *i++ = curRow + s;
        *i++ = nextRow + s;
        *i++ = nextRow + nextS;

        *i++ = curRow + s;
        *i++ = nextRow + nextS;
        *i++ = curRow + nextS;
      }
    Mesh::Sphere = new Mesh("SphereMesh", vertices, normals, texts, indices);
  }

  void Mesh::generatePrimitives() {
    generateSphere();
  }

  static Vector calculateNormals(Vector vertex1, Vector vertex2, Vector vertex3) {
    return Vector3::cross(
      vertex3 - vertex1, vertex2 - vertex1
    ).normalized();
  }

  Mesh* Mesh::load(std::string path) {
    std::vector<Vector> vertices;
    std::vector<Vector> normals;
    std::vector<Vector2> texts;
    std::vector<unsigned int> indices;

    std::string err;
    std::string warn;
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str());
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
          if (vertZ < minZ ) {
            minZ = vertZ;
          }
          if (vertZ > maxZ) {
            maxZ = vertZ;
          }
          vertices.push_back({vertX, vertY, vertZ});
          if (!attrib.normals.empty()) {
            normals.push_back({
              attrib.normals[3 * idx.normal_index + 0],
              attrib.normals[3 * idx.normal_index + 1],
              attrib.normals[3 * idx.normal_index + 2]
            });
          } else {
            normals.push_back(Vector3::Zero);
          }
          if (!attrib.texcoords.empty()) {
            texts.push_back({
              attrib.texcoords[2 * idx.texcoord_index + 0],
              attrib.texcoords[2 * idx.texcoord_index + 1]
            });
          } else {
            texts.push_back(Vector2(0.5, 0.5));
          }
          indices.push_back(index++);
        }
        index_offset += fv;
        auto norm = calculateNormals(vertices[index - 3], vertices[index - 2], vertices[index - 1]);
        normals[index - 3] = norm;
        normals[index - 2] = norm;
        normals[index - 1] = norm;
      }
    }
    // Determine object scale to force unit s
    float offsetX = 0.5f * (minX + maxX);
    float offsetY = 0.5f * (minY + maxY);
    float offsetZ = 0.5f * (minZ + maxZ);
    float scaleX = abs( minX - offsetX ) > abs( maxX - offsetX ) ? abs( minX - offsetX ) : abs( maxX - offsetX );
    float scaleY = abs( minY - offsetY ) > abs( maxY - offsetY ) ? abs( minY - offsetY ) : abs( maxY - offsetY );
    float scaleZ = abs( minZ - offsetZ ) > abs( maxZ - offsetZ ) ? abs( minZ - offsetZ ) : abs( maxZ - offsetZ );
    float scale = scaleX > scaleY ? scaleX : scaleY;
    scale = scaleZ > scale ? 1.0f / scaleZ : 1.0f / scale;

    for (auto& vert : vertices) {
      vert.x = scale * (vert.x - offsetX);
      vert.y = scale * (vert.y - offsetY);
      vert.z = scale * (vert.z - offsetZ);
    }

    // TODO: Some kind of id genner
    std::cout << vertices.size() << std::endl;
    std::cout << normals.size() << std::endl;
    std::cout << texts.size() << std::endl;
    std::cout << indices.size() << std::endl;
    return make<Mesh>("GennedMesh", vertices, normals, texts, indices);
  }
}