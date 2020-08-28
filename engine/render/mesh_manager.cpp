
#include "render/mesh_manager.h"
#include "render/mesh_data.h"

namespace wage {
  namespace render {

    void MeshManager::generatePrimitives() {
      auto cube = new MeshData(MeshSpec::Cube,
                               VertexVector{
                                   //front
                                   math::Vector(-0.5, -0.5, 0.5),
                                   math::Vector(0.5, -0.5, 0.5),
                                   math::Vector(0.5, 0.5, 0.5),
                                   math::Vector(-0.5, 0.5, 0.5),
                                   //right
                                   math::Vector(0.5, 0.5, 0.5),
                                   math::Vector(0.5, 0.5, -0.5),
                                   math::Vector(0.5, -0.5, -0.5),
                                   math::Vector(0.5, -0.5, 0.5),
                                   //back
                                   math::Vector(-0.5, -0.5, -0.5),
                                   math::Vector(0.5, -0.5, -0.5),
                                   math::Vector(0.5, 0.5, -0.5),
                                   math::Vector(-0.5, 0.5, -0.5),
                                   //left
                                   math::Vector(-0.5, -0.5, -0.5),
                                   math::Vector(-0.5, -0.5, 0.5),
                                   math::Vector(-0.5, 0.5, 0.5),
                                   math::Vector(-0.5, 0.5, -0.5),
                                   //upper
                                   math::Vector(0.5, 0.5, 0.5),
                                   math::Vector(-0.5, 0.5, 0.5),
                                   math::Vector(-0.5, 0.5, -0.5),
                                   math::Vector(0.5, 0.5, -0.5),
                                   //bottom
                                   math::Vector(-0.5, -0.5, -0.5),
                                   math::Vector(0.5, -0.5, -0.5),
                                   math::Vector(0.5, -0.5, 0.5),
                                   math::Vector(-0.5, -0.5, 0.5)},
                               VertexVector{
                                   //front
                                   math::Vector(0.0f, 0.0f, 1.0f),
                                   math::Vector(0.0f, 0.0f, 1.0f),
                                   math::Vector(0.0f, 0.0f, 1.0f),
                                   math::Vector(0.0f, 0.0f, 1.0f),
                                   //right
                                   math::Vector(1.0f, 0.0f, 0.0f),
                                   math::Vector(1.0f, 0.0f, 0.0f),
                                   math::Vector(1.0f, 0.0f, 0.0f),
                                   math::Vector(1.0f, 0.0f, 0.0f),
                                   //back
                                   math::Vector(0.0f, 0.0f, -1.0f),
                                   math::Vector(0.0f, 0.0f, -1.0f),
                                   math::Vector(0.0f, 0.0f, -1.0f),
                                   math::Vector(0.0f, 0.0f, -1.0f),
                                   //left
                                   math::Vector(-1.0f, 0.0f, 0.0f),
                                   math::Vector(-1.0f, 0.0f, 0.0f),
                                   math::Vector(-1.0f, 0.0f, 0.0f),
                                   math::Vector(-1.0f, 0.0f, 0.0f),
                                   //upper
                                   math::Vector(0.0f, 1.0f, 0.0f),
                                   math::Vector(0.0f, 1.0f, 0.0f),
                                   math::Vector(0.0f, 1.0f, 0.0f),
                                   math::Vector(0.0f, 1.0f, 0.0f),
                                   //bottom
                                   math::Vector(0.0f, -1.0f, 0.0f),
                                   math::Vector(0.0f, -1.0f, 0.0f),
                                   math::Vector(0.0f, -1.0f, 0.0f),
                                   math::Vector(0.0f, -1.0f, 0.0f),
                               },
                               Vertex2Vector{
                                   math::Vector2(0.0f, 1.0f), // bottom left
                                   math::Vector2(1.0f, 1.0f), // bottom right
                                   math::Vector2(1.0f, 0.0f), // top right
                                   math::Vector2(0.0f, 0.0f), // top left
                                   math::Vector2(0.0f, 1.0f), // bottom left
                                   math::Vector2(1.0f, 1.0f), // bottom right
                                   math::Vector2(1.0f, 0.0f), // top right
                                   math::Vector2(0.0f, 0.0f), // top left
                                   math::Vector2(0.0f, 1.0f), // bottom left
                                   math::Vector2(1.0f, 1.0f), // bottom right
                                   math::Vector2(1.0f, 0.0f), // top right
                                   math::Vector2(0.0f, 0.0f), // top left
                                   math::Vector2(0.0f, 1.0f), // bottom left
                                   math::Vector2(1.0f, 1.0f), // bottom right
                                   math::Vector2(1.0f, 0.0f), // top right
                                   math::Vector2(0.0f, 0.0f), // top left
                                   math::Vector2(0.0f, 1.0f), // bottom left
                                   math::Vector2(1.0f, 1.0f), // bottom right
                                   math::Vector2(1.0f, 0.0f), // top right
                                   math::Vector2(0.0f, 0.0f), // top left
                                   math::Vector2(0.0f, 1.0f), // bottom left
                                   math::Vector2(1.0f, 1.0f), // bottom right
                                   math::Vector2(1.0f, 0.0f), // top right
                                   math::Vector2(0.0f, 0.0f), // top left
                                   math::Vector2(0.0f, 1.0f), // bottom left
                                   math::Vector2(1.0f, 1.0f), // bottom right
                                   math::Vector2(1.0f, 0.0f), // top right
                                   math::Vector2(0.0f, 0.0f), // top left
                                   math::Vector2(0.0f, 1.0f), // bottom left
                                   math::Vector2(1.0f, 1.0f), // bottom right
                                   math::Vector2(1.0f, 0.0f), // top right
                                   math::Vector2(0.0f, 0.0f), // top left
                               },
                               IndexVector{
                                   0, 1, 2, 0, 2, 3,       //front
                                   4, 5, 6, 4, 6, 7,       //right
                                   8, 9, 10, 8, 10, 11,    //back
                                   12, 13, 14, 12, 14, 15, //left
                                   16, 17, 18, 16, 18, 19, //upper
                                   20, 21, 22, 20, 22, 23  // bottom
                               });
      primitives[cube->spec().key()] = cube;

      auto quad = new MeshData(
          MeshSpec::Quad,
          VertexVector{
              math::Vector(0.5, 0.5, 0.0f),     // top right
              math::Vector(0.5f, -0.5f, 0.0f),  // bottom right
              math::Vector(-0.5f, -0.5f, 0.0f), // bottom left
              math::Vector(-0.5f, 0.5f, 0.0f)   // top left
          },
          VertexVector{
              math::Vector(0.0f, 0.0f, -1.0f), // top right
              math::Vector(0.0f, 0.0f, -1.0f), // bottom right
              math::Vector(0.0f, 0.0f, -1.0f), // bottom left
              math::Vector(0.0f, 0.0f, -1.0f)  // top left
          },
          Vertex2Vector{
              math::Vector2(1.0f, 0.0f), // top right
              math::Vector2(1.0f, 1.0f), // bottom right
              math::Vector2(0.0f, 1.0f), // bottom left
              math::Vector2(0.0f, 0.0f)  // top left
          },
          IndexVector{
              0, 1, 3, // first Triangle
              1, 2, 3  // second Triangle
          });

      primitives[quad->spec().key()] = quad;

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
          *t++ = math::Vector2(s * S, r * R);

          *v++ = math::Vector(x * radius, y * radius, z * radius);

          *n++ = math::Vector(x, y, z);
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
      auto sphere = new MeshData(MeshSpec::Sphere, vertices, normals, texts, indices);
      primitives[sphere->spec().key()] = sphere;
    }

  }
}