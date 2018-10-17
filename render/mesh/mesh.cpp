#include "render/mesh/mesh.h"

Mesh::Mesh(const VertexBuffer* vbo, const IndexBuffer* ibo, const VertexBufferLayout* layout)  : vbo(vbo), ibo(ibo), layout(layout) {
}

Mesh::~Mesh() {
}

void Mesh::bind(VertexArray* vao) const {
  vao->bind();
  vao->addBuffer(vbo, layout);
  // ibo->bind();  TODO: Figure out if we are going tod do this.....
}

void Mesh::unbind() const {
  // ibo->unbind();
  vbo->unbind();
}

