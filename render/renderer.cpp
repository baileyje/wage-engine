#include "render/renderer.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <vector>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/quaternion.hpp>
#include "glm/ext.hpp"

#include "render/vertex_buffer.h"
#include "render/index_buffer.h"
#include "render/vertex_array.h"
#include "render/util.h"
#include "render/material.h"
#include "render/mesh/mesh.h"
#include "render/mesh/quad.h"
#include "render/shader/shader.h"


void errorCallback(int error, const char* description) {
  printf("Error: %s\n", description);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

Renderer::~Renderer() {
}

void Renderer::init(Context* context)  {
  printf("Initializing Renderer.\n");
  glfwSetErrorCallback(errorCallback);
  if (!glfwInit()) {
    printf("GLFW Init Failed\n");
  }  
  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(1024, 768, "Wage Bro!", NULL, NULL);
  glfwSetKeyCallback(window, keyCallback);   
  screenProjection = glm::ortho(0.0f, (float)1024, 0.0f, (float)768, -1.0f, 1.0f);  
}

void Renderer::start(Context* context) {
  printf("Starting Renderer.\n");
  glfwMakeContextCurrent(window);
  gladLoadGL();
  glfwSwapInterval(1);
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  FAIL_CHECK(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
  Shader::initDefault();
  
}


void Renderer::fixedUpdate(Context* context) {
  // printf("Updating Renderer.\n");
  if (glfwWindowShouldClose(window)) {
    printf("Close!\n");
    context->shouldStop();
    return;
  }
  // float ratio;
  // int width, height;
  // glfwGetFramebufferSize(window, &width, &height);
  // ratio = width / (float) height;
  // FAIL_CHECK(glViewport(0, 0, width, height));
  FAIL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));



  for (auto entity : *context->getEntities()) {
    draw(entity);    
  }

  FAIL_CHECK(glfwSwapBuffers(window));
  FAIL_CHECK(glfwPollEvents());
}

void Renderer::draw(Entity* entity) {
  Quad quad;
  Material material(Shader::Default);
  // TODO: Set position!
  Transform* transform = entity->getTransform();
  glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(transform->getPosition()->getX(), transform->getPosition()->getY(), transform->getPosition()->getZ()));
  glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(transform->getScale()->getX(), transform->getScale()->getY(), transform->getScale()->getZ()));

  glm::quat rotation(glm::vec3(transform->getRotation()->getX(), transform->getRotation()->getY(), transform->getRotation()->getZ()));
  glm::mat4 rotate = glm::toMat4(rotation);

  glm::mat4 model = translation * rotate * scale;
  
  glm::mat4 mvp = screenProjection * model;  
  material.setMat4("mvp", &mvp, 1);
  draw(&quad, &material);
}

void Renderer::draw(Mesh* mesh, Material* material) {
  VertexArray vao;
  vao.bind();  
  material->bind();
  mesh->bind(&vao);
  FAIL_CHECK(glDrawElements(GL_TRIANGLES, mesh->getElementCount(), GL_UNSIGNED_INT, 0));
  mesh->unbind();
  material->unbind();
  vao.unbind();  
}

void Renderer::stop(Context* context) {
  printf("Stopping Renderer.\n");
}

void Renderer::deinit(Context* context) {
  printf("Deinitializing Renderer.\n");
  glfwDestroyWindow(window);
  glfwTerminate();
}
