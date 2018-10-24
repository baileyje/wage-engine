#include "render/renderer.h"

#include <vector>
#include <sstream>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/ext.hpp"

#include "platform/platform.h"

#include "render/vertex_buffer.h"
#include "render/index_buffer.h"
#include "render/vertex_array.h"
#include "render/util.h"
#include "render/material.h"
#include "render/texture.h"
#include "render/shader/shader.h"

#include "entity/component/mesh.h"
#include "entity/component/material.h"
#include "entity/component/camera.h"
#include "entity/component/directional_light.h"
#include "entity/component/point_light.h"
#include "entity/component/spotlight.h"


Renderer::~Renderer() {
}

void Renderer::init(Context* context)  {
  printf("Initializing Renderer.\n");

  // TODO: Move to camera
  // screenProjection = glm::ortho(0.0f, (float)102.4, 0.0f, (float)76.8, -10.0f, 10.0f);  
  screenProjection = glm::perspective(glm::radians(45.0), 1024.0 / 768.0, 0.1, 100.0);
}

void Renderer::start(Context* context) {
  printf("Starting Renderer.\n");
  Platform* platform = static_cast<Platform*>(context->get("platform"));
  window = platform->getWindow();
  glfwMakeContextCurrent(window);
  gladLoadGL();
  glfwSwapInterval(1);
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  FAIL_CHECK(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
  Shader::initDefault(context->getFileSystem());  
  
  // TODO: Listen for changes to entities and figure out how to get these!!!
  dirLights = context->getEntitiesWith("DirectionalLight");
  pointLights = context->getEntitiesWith("PointLight");
  spotlights = context->getEntitiesWith("Spotlight");
}

void Renderer::fixedUpdate(Context* context) {
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  FAIL_CHECK(glViewport(0, 0, width, height));
  FAIL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
  Entity* camera = context->getCamera();
  Transform* cameraTransform = camera->getTransform();
  glm::vec3 cameraPosition = vec3From(cameraTransform->getPosition());
  glm::mat4 cameraProjection = viewProjectionFrom(camera);
  for (auto entity : *context->getEntities()) {
    draw(cameraPosition, cameraProjection, entity);    
  }
  FAIL_CHECK(glfwSwapBuffers(window));
  FAIL_CHECK(glfwPollEvents());
}

glm::mat4 Renderer::viewProjectionFrom(Entity* camera) {
  Transform* cameraTrans = camera->getTransform();
  glm::vec3 camPos = vec3From(cameraTrans->getPosition());
  glm::quat camRotation(quatFromEulers(cameraTrans->getRotation()));
  glm::vec3 camFront(
    2 * (camRotation.x * camRotation.z + camRotation.w * camRotation.y), 
    2 * (camRotation.y * camRotation.z - camRotation.w * camRotation.x),
    1 - 2 * (camRotation.x * camRotation.x + camRotation.y * camRotation.y)
  );
  glm::vec3 camUp(
    2 * (camRotation.x * camRotation.y - camRotation.w * camRotation.z), 
    1 - 2 * (camRotation.x * camRotation.x - camRotation.z * camRotation.z),
    2 * (camRotation.y * camRotation.z + camRotation.w * camRotation.x)
  );
  return  glm::lookAt(camPos, camPos + camFront, camUp);
}

glm::mat4 Renderer::modelProjectionFrom(Entity* entity) {
  Transform* transform = entity->getTransform();
  glm::mat4 translation = glm::translate(glm::mat4(1), vec3From(transform->getPosition()));
  glm::mat4 scale = glm::scale(glm::mat4(1), vec3From(transform->getScale()));
  glm::quat rotation = quatFromEulers(transform->getRotation());
  glm::mat4 rotate = glm::toMat4(rotation);
  return translation * rotate * scale;
}

void Renderer::draw(glm::vec3 cameraPosition, glm::mat4 cameraProjection, Entity* entity) {
  Mesh* mesh = (Mesh*)entity->getComponent("Mesh");
  if (!mesh) {
    return;
  }
  GlMaterial material(Shader::Default);
  material.bind();
  
  glm::mat4 model = modelProjectionFrom(entity);
  
  material.setMat4("model", model);
  material.setMat4("view", cameraProjection);
  material.setMat4("projection", screenProjection);  
  
  material.setVec3("viewPos", cameraPosition);
  material.setInt("numDirLights", dirLights.size());

  int idx = 0;
  for (auto dirLightEnt : dirLights) {
    std::stringstream base;
    base << "dirLights[" << idx++ << "]";
    DirectionalLight* light = (DirectionalLight*)dirLightEnt->getComponent("DirectionalLight");
    glm::vec3 lightDir = directionFromEulers(dirLightEnt->getTransform()->getRotation());
    material.setVec3(base.str() + ".direction", directionFromEulers(dirLightEnt->getTransform()->getRotation()));
    material.setVec3(base.str() + ".ambient", vec3From(light->getAmbient()));
    material.setVec3(base.str() + ".diffuse", vec3From(light->getDiffuse()));
    material.setVec3(base.str() + ".specular", vec3From(light->getSpecular()));
  }
  
  material.setInt("numPointLights", pointLights.size());
  idx = 0;
  for (auto lightEnt : pointLights) {
    std::stringstream base;
    base << "pointLights[" << idx++ << "]";
    PointLight* light = (PointLight*)lightEnt->getComponent("PointLight");
    material.setVec3(base.str() + ".position", vec3From(lightEnt->getTransform()->getPosition()));
    material.setVec3(base.str() + ".ambient", vec3From(light->getAmbient()));
    material.setVec3(base.str() + ".diffuse", vec3From(light->getDiffuse()));
    material.setVec3(base.str() + ".specular", vec3From(light->getSpecular()));
    material.setFloat(base.str() + ".constant", light->getConstant());
    material.setFloat(base.str() + ".linear", light->getLinear());
    material.setFloat(base.str() + ".quadratic", light->getQuadratic());
  }

  material.setInt("numSpotLights", spotlights.size());
  idx = 0;
  for (auto lightEnt : spotlights) {
    std::stringstream base;
    base << "spotLights[" << idx++ << "]";
    Spotlight* light = (Spotlight*)lightEnt->getComponent("Spotlight");
    material.setVec3(base.str() + ".position", vec3From(lightEnt->getTransform()->getPosition()));
    material.setVec3(base.str() + ".direction", directionFromEulers(lightEnt->getTransform()->getRotation()));
    material.setVec3(base.str() + ".ambient", vec3From(light->getAmbient()));
    material.setVec3(base.str() + ".diffuse", vec3From(light->getDiffuse()));
    material.setVec3(base.str() + ".specular", vec3From(light->getSpecular()));
    material.setFloat(base.str() + ".constant", light->getConstant());
    material.setFloat(base.str() + ".linear", light->getLinear());
    material.setFloat(base.str() + ".quadratic", light->getQuadratic());
    material.setFloat(base.str() + ".cutOff", glm::cos(glm::radians(light->getCutOff())));
    material.setFloat(base.str() + ".outerCutoff", glm::cos(glm::radians(light->getOuterCutOff())));
  }

  GlTexture::Default.bind();
  material.setInt("material.diffuse", 0);
  material.setFloat("material.shininess ", 32.0f);
  draw(mesh, &material);
}

void Renderer::draw(Entity* camera, Entity* entity) {  
  Transform* cameraTransform = camera->getTransform();
  glm::vec3 cameraPosition = vec3From(cameraTransform->getPosition());  
  draw(cameraPosition, viewProjectionFrom(camera), entity);
}

void Renderer::draw(Mesh* mesh, GlMaterial* material) {
  VertexArray vao;
  vao.bind();  
  // material->bind();

  // TODO: I think this could be cached for sure.  
  // Create Verts Buff
  VertexBuffer verts(mesh->getVertices()->data(), mesh->getVertices()->size() * 3 * sizeof(float));
  verts.getLayout()->pushFloat(3);
  vao.addBuffer(&verts);
  // Create Norms Buff
  VertexBuffer norms(mesh->getNormals()->data(), mesh->getNormals()->size() * 3 * sizeof(float));
  norms.getLayout()->pushFloat(3);
  vao.addBuffer(&norms);

  VertexBuffer uvs(mesh->getUvs()->data(), mesh->getUvs()->size() * 3 * sizeof(float));
  uvs.getLayout()->pushFloat(3);
  vao.addBuffer(&uvs);  
  // Create Index Buff
  IndexBuffer indices((const unsigned int*)mesh->getIndices()->data(), mesh->getIndices()->size());
  indices.bind();
  FAIL_CHECK(glDrawElements(GL_TRIANGLES, mesh->getElementCount(), GL_UNSIGNED_INT, 0));
  // FAIL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 36));
  indices.unbind();
  norms.unbind();
  verts.unbind();
  material->unbind();
  vao.unbind();
}

void Renderer::stop(Context* context) {
  printf("Stopping Renderer.\n");
}

void Renderer::deinit(Context* context) {
  printf("Deinitializing Renderer.\n");
}
