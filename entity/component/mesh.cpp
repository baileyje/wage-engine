#include "entity/component/mesh.h"


Mesh::~Mesh() {
}

Mesh Mesh::Cube(
  "CubeMesh",
  VertexVector {
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
    Vector(-0.5, -0.5, 0.5)
  },
  VertexVector {
    //front
    Vector(0.0f,  0.0f, 1.0f),
    Vector(0.0f,  0.0f, 1.0f),
    Vector(0.0f,  0.0f, 1.0f),
    Vector(0.0f,  0.0f, 1.0f),  
    //right
    Vector(1.0f,  0.0f, 0.0f),
    Vector(1.0f,  0.0f, 0.0f),
    Vector(1.0f,  0.0f, 0.0f),
    Vector(1.0f,  0.0f, 0.0f),
    //back
    Vector(0.0f,  0.0f, -1.0f),
    Vector(0.0f,  0.0f, -1.0f),
    Vector(0.0f,  0.0f, -1.0f),
    Vector(0.0f,  0.0f, -1.0f),
    //left
    Vector(-1.0f,  0.0f, 0.0f),
    Vector(-1.0f,  0.0f, 0.0f),
    Vector(-1.0f,  0.0f, 0.0f),
    Vector(-1.0f,  0.0f, 0.0f),
    //upper
    Vector(0.0f,  1.0f, 0.0f),
    Vector(0.0f,  1.0f, 0.0f),
    Vector(0.0f,  1.0f, 0.0f),
    Vector(0.0f,  1.0f, 0.0f),
    //bottom
    Vector(0.0f,  -1.0f, 0.0f),
    Vector(0.0f,  -1.0f, 0.0f),
    Vector(0.0f,  -1.0f, 0.0f),
    Vector(0.0f,  -1.0f, 0.0f),
  },
  IndexVector {
    0, 1, 2, 0, 2, 3, //front
    4, 5, 6, 4, 6, 7, //right
    8, 9, 10, 8, 10, 11, //back
    12, 13, 14, 12, 14, 15, //left
    16, 17, 18, 16, 18, 19, //upper
    20, 21, 22, 20, 22, 23 // bottom
  }
);

Mesh Mesh::Quad(
  "QuadMesh",
  VertexVector {
    Vector(0.5,  0.5, 0.0f),  // top right
    Vector(0.5f, -0.5f, 0.0f),  // bottom right
    Vector(-0.5f, -0.5f, 0.0f),  // bottom left
    Vector(-0.5f,  0.5f, 0.0f)   // top left 
  },
  VertexVector {
    Vector(0.0f,  0.0f, -1.0f),  // top right
    Vector(0.0f,  0.0f, -1.0f),  // bottom right
    Vector(0.0f,  0.0f, -1.0f),  // bottom left
    Vector(0.0f,  0.0f, -1.0f)   // top left 
  },
  IndexVector {
    0, 1, 3,  // first Triangle
    1, 2, 3   // second Triangle
  }
);
