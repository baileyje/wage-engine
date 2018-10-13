#include "render/mesh.h"

Mesh::Mesh(const VertexBuffer* vbo, const IndexBuffer* ibo, const VertexBufferLayout* layout) 
  : vbo(vbo), ibo(ibo), layout(layout) {
}

Mesh::~Mesh() {

}

void Mesh::bind(VertexArray* vao) {
  vao->bind();
  vao->addBuffer(vbo, layout);
  ibo->bind();
}