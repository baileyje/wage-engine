
#include "render/mesh/manager.h"
#include "render/mesh/mesh.h"

namespace wage {
  namespace render {

    void MeshManager::generatePrimitives() {
      auto cube = new Mesh(MeshSpec::Cube, 
      {
        //front
        {{-0.5, -0.5, 0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
        {{0.5, -0.5, 0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{0.5, 0.5, 0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{-0.5, 0.5, 0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        //right
        {{0.5, 0.5, 0.5}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        {{0.5, 0.5, -0.5}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{0.5, -0.5, -0.5}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5, -0.5, 0.5}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        //back
        {{-0.5, -0.5, -0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
        {{0.5, -0.5, -0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
        {{0.5, 0.5, -0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
        {{-0.5, 0.5, -0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
        //left
        {{-0.5, -0.5, -0.5}, {1.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        {{-0.5, -0.5, 0.5}, {1.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5, 0.5, 0.5}, {1.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{-0.5, 0.5, -0.5}, {1.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        //upper
        {{0.5, 0.5, 0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
        {{-0.5, 0.5, 0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5, 0.5, -0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5, 0.5, -0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        //bottom
        {{-0.5, -0.5, -0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
        {{0.5, -0.5, -0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
        {{0.5, -0.5, 0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
        {{-0.5, -0.5, 0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, { 0.0f, 0.0f }}
      }, 
      {
        0, 1, 2, 0, 2, 3,       //front
        4, 5, 6, 4, 6, 7,       //right
        8, 9, 10, 8, 10, 11,    //back
        12, 13, 14, 12, 14, 15, //left
        16, 17, 18, 16, 18, 19, //upper
        20, 21, 22, 20, 22, 23  // bottom
      }
      );
      primitives[cube->spec().key()] = cube;

      auto quad = new Mesh(
          MeshSpec::Quad,
          {
            { { 0.5, 0.5, 0.0f }, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f} , {1.0f, 0.0f} },     // top right
            { { 0.5f, -0.5f, 0.0f }, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f} , {1.0f, 1.0f} },  // bottom right
            { { -0.5f, -0.5f, 0.0f }, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f} , {0.0f, 1.0f} }, // bottom left
            { { -0.5f, 0.5f, 0.0f }, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f} , {0.0f, 0.0f} }   // top left
          },
          {
              0, 1, 3, // first Triangle
              1, 2, 3  // second Triangle
          });

      primitives[quad->spec().key()] = quad;

      std::vector<Vertex> vertices;
      std::vector<uint32_t> indices;

      float radius = 0.5;
      unsigned int rings = 12;
      unsigned int sectors = 24;

      float const R = 1. / (float)(rings - 1);
      float const S = 1. / (float)(sectors - 1);
      int r, s;

      vertices.resize(rings * sectors);

      std::vector<Vertex>::iterator v = vertices.begin();
      for (r = 0; r < rings; r++)
        for (s = 0; s < sectors; s++) {
          float const y = sin(-M_PI_2 + M_PI * r * R);
          float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
          float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);          
          *v++ = {{x * radius, y * radius, z * radius}, {1.0f, 1.0f, 1.0f}, {x, y, z}, {s * S, r * R}};
        }
      indices.resize(rings * sectors * 6);
      std::vector<uint32_t>::iterator i = indices.begin();
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
      auto sphere = new Mesh(MeshSpec::Sphere, vertices, indices);
      primitives[sphere->spec().key()] = sphere;
    }
  }
}