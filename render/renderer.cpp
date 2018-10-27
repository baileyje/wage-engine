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
#include "entity/component/perspective_camera.h"
#include "entity/component/orthographic_camera.h"
#include "entity/component/directional_light.h"
#include "entity/component/point_light.h"
#include "entity/component/spotlight.h"


Renderer::~Renderer() {
}

void Renderer::init(Context* context)  {
  printf("Initializing Renderer.\n");

  // TODO: Move to camera
  // screenProjection = glm::ortho(0.0f, (float)102.4, 0.0f, (float)76.8, -10.0f, 10.0f);  
  // screenProjection = glm::perspective(glm::radians(45.0), 1024.0 / 768.0, 0.1, 1000.0);
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
  dirLights = context->getScene()->getEntities()->with("DirectionalLight");
  pointLights = context->getScene()->getEntities()->with("PointLight");
  spotlights = context->getScene()->getEntities()->with("Spotlight");

  glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
}

void Renderer::update(Context* context) {  
  FAIL_CHECK(glViewport(0, 0, screenWidth, screenHeight));
  FAIL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
  Entity* camera = context->getScene()->getCamera();
  Transform* cameraTransform = camera->getTransform();
  glm::vec3 cameraPosition = vec3From(cameraTransform->getPosition());
  glm::mat4 cameraProjection = viewProjectionFrom(cameraTransform);
  glm::mat4 screenProjection = screenProjectionFrom((Camera*)camera->getComponents()->get("Camera"));
  for (auto entity : *context->getScene()->getEntities()) {
    draw(screenProjection, cameraPosition, cameraProjection, entity);    
  }
  FAIL_CHECK(glfwSwapBuffers(window));
  FAIL_CHECK(glfwPollEvents());
}


void Renderer::draw(glm::mat4 screenProjection, glm::vec3 cameraPosition, glm::mat4 cameraProjection, Entity* entity) {
  Mesh* mesh = entity->get<Mesh>();
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
    DirectionalLight* light = dirLightEnt->get<DirectionalLight>();
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
    PointLight* light = lightEnt->get<PointLight>();
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
    Spotlight* light = lightEnt->get<Spotlight>();
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


// void Renderer::draw(Entity* camera, Entity* entity) {  
//   Transform* cameraTransform = camera->getTransform();
//   glm::vec3 cameraPosition = vec3From(cameraTransform->getPosition());  
//   draw(cameraPosition, viewProjectionFrom(camera), entity);
// }

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

//------

glm::mat4 Renderer::modelProjectionFrom(Entity* entity) {
  Transform* transform = entity->getTransform();
  glm::mat4 translation = glm::translate(glm::mat4(1), vec3From(transform->getPosition()));
  glm::mat4 scale = glm::scale(glm::mat4(1), vec3From(transform->getScale()));
  glm::quat rotation = quatFromEulers(transform->getRotation());
  glm::mat4 rotate = glm::toMat4(rotation);
  return translation * rotate * scale;
}


glm::mat4 Renderer::screenProjectionFrom(Camera* camera) {
  if (camera->getType() == perspective) {
    PerspectiveCamera* perspectiveCam = static_cast<PerspectiveCamera*>(camera);
    return glm::perspective(glm::radians(perspectiveCam->getFov()), (float)screenWidth / (float)screenHeight, perspectiveCam->getNearClip(), perspectiveCam->getFarClip());
  } else if (camera->getType() == orthographic) {
    OrthographicCamera* orthographicCamera = static_cast<OrthographicCamera*>(camera);
    printf("Right: %f\n", orthographicCamera->getLeft());
    return glm::ortho(
      orthographicCamera->getLeft(), orthographicCamera->getRight(),
      orthographicCamera->getTop(), orthographicCamera->getBottom(),
      orthographicCamera->getNearClip(), orthographicCamera->getFarClip()
    );
  }
  return glm::mat4(1.0);
}

glm::mat4 Renderer::viewProjectionFrom(Transform* cameraTransform) {
  glm::vec3 camPos = vec3From(cameraTransform->getPosition());
  glm::quat camRotation(quatFromEulers(cameraTransform->getRotation()));
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

//------

void Renderer::stop(Context* context) {
  printf("Stopping Renderer.\n");
}

void Renderer::deinit(Context* context) {
  printf("Deinitializing Renderer.\n");
}

