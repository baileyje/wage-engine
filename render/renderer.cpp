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
#include "render/mesh/cube.h"
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
  // screenProjection = glm::ortho(0.0f, (float)102.4, 0.0f, (float)76.8, -10.0f, 10.0f);  
  screenProjection = glm::perspective(glm::radians(45.0), 1024.0 / 768.0, 0.1, 100.0);
}

void Renderer::start(Context* context) {
  printf("Starting Renderer.\n");
  glfwMakeContextCurrent(window);
  gladLoadGL();
  glfwSwapInterval(1);
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);
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
    draw(context->getCamera(), entity);    
  }

  FAIL_CHECK(glfwSwapBuffers(window));
  FAIL_CHECK(glfwPollEvents());
}

void Renderer::draw(Camera* camera, Entity* entity) {
  Cube cube;
  Material material(Shader::Default);
  // TODO: Set position!
  Transform* transform = entity->getTransform();
  glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(transform->getPosition()->x, transform->getPosition()->y, transform->getPosition()->z));
  glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(transform->getScale()->x, transform->getScale()->y, transform->getScale()->z));
  glm::quat rotation(glm::vec3(transform->getRotation()->x, transform->getRotation()->y, transform->getRotation()->z));
  glm::mat4 rotate = glm::toMat4(rotation);

  glm::mat4 model = translation * rotate * scale;
   
  // Camera
  Transform* cameraTrans = camera->getTransform();
  glm::vec3 camPos(cameraTrans->getPosition()->x, cameraTrans->getPosition()->y, cameraTrans->getPosition()->z);
  glm::quat camRotation(glm::vec3(cameraTrans->getRotation()->x, cameraTrans->getRotation()->y, cameraTrans->getRotation()->z));
  // glm::vec3 camPos(0,0,-100);
  glm::vec3 camFront(
    2 * (camRotation.x * camRotation.z + camRotation.w * camRotation.y), 
    2 * (camRotation.y * camRotation.z - camRotation.w * camRotation.x),
    1 - 2 * (camRotation.x * camRotation.x + camRotation.y * camRotation.y)
  );
  glm::vec3 camUp(
    2 * (camRotation.x * camRotation.y - camRotation.w * camRotation.z), 
    1 - 2 * (camRotation.x * camRotation.x + camRotation.z * camRotation.z),
    2 * (camRotation.y * camRotation.z + camRotation.w * camRotation.x)
  );
  glm::mat4 view = glm::lookAt(camPos, camPos + camFront, camUp);

  material.setMat4("model", &model, 1);
  material.setMat4("view", &view, 1); // TODO Add camera
  material.setMat4("projection", &screenProjection, 1);

  glm::vec3 lightPos(0.0f, 0.0f, -10.0f);
  glm::vec3 objColor(1.0f, 0.5f, 0.31f);
  glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
  material.setVec3("objectColor", &objColor, 1);
  material.setVec3("lightColor", &lightColor, 1);
  material.setVec3("lightPos", &lightPos, 1);
  
  material.setVec3("viewPos", &camPos, 1);

  draw(&cube, &material);
}

void Renderer::draw(Mesh* mesh, Material* material) {
  VertexArray vao;
  vao.bind();  
  material->bind();
  mesh->bind(&vao);
  // FAIL_CHECK(glDrawElements(GL_TRIANGLES, mesh->getElementCount(), GL_UNSIGNED_INT, 0));
  FAIL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 36));
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
