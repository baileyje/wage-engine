#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "core/system.h"
#include "render/shader.h"
#include "render/vertex_buffer.h"
#include "render/index_buffer.h"
#include "render/vertex_array.h"
#include "render/material.h"
#include "render/mesh.h"

class Renderer : public System {

public:

  Renderer() : System("renderer") { }

  virtual ~Renderer();

  void init(Context* context);

  void start(Context* context);

  void update(Context* context);

  void stop(Context* context);

  void deinit(Context* context);

  void draw(VertexArray* vao, Mesh* mesh, Material* material);

private:

  GLFWwindow* window;
  
  Shader* defaultShaderProgram;

  Shader* loadJankyShader();

  glm::mat4 screenProjection;
  
  // TODO: These will be per entity
  Mesh* mesh;

  Material* material;

  VertexArray* vao;
};

#endif //RENDERER_H