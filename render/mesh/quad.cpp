#include "render/mesh/quad.h"

float quadVertices[] = {
  0.5f,  0.5f, 0.0f,  // top right
  0.5f, -0.5f, 0.0f,  // bottom right
  -0.5f, -0.5f, 0.0f,  // bottom left
  -0.5f,  0.5f, 0.0f   // top left 
};

unsigned int quadIndices[] = {  // note that we start from 0!
  0, 1, 3,  // first Triangle
  1, 2, 3   // second Triangle
};

Quad::Quad() : Mesh(
    new VertexBuffer(quadVertices, sizeof(quadVertices)),
    new IndexBuffer(quadIndices, 6),
    (new VertexBufferLayout())->pushFloat(3)    
  ) { }

Quad::~Quad() {
    // TODO:  Completly leaking the vbo, ib and vbl  
}