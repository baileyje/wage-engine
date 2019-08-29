#ifndef MESH_RENDERABLE_H
#define MESH_RENDERABLE_H

#include <sstream>
#include <cmath>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/ext.hpp"

#include "math/matrix.h"
#include "memory/allocator.h"
#include "render/renderable.h"
#include "render/context.h"
#include "render/mesh.h"
#include "render/material.h"
#include "component/lighting/directional_light.h"
#include "component/lighting/point_light.h"
#include "component/lighting/spotlight.h"

#include "render-gl/material.h"

namespace wage {

  class GlMeshRenderable : public Renderable {

  public:
    GlMeshRenderable(VaoManager* vaoManager, GlTextureManager* textureManager, Transform* transform, Mesh* mesh, Material* material)
        : _vaoManager(vaoManager), _textureManager(textureManager), transform(transform), mesh(mesh), material(material) {}

    inline VaoManager* vaoManager() {
      return _vaoManager;
    }

    virtual Vector position() {
      return transform->position();
    }

    virtual BoundingBox boundingBox() {
      Vector maxDims = mesh->maxDim();
      Vector scale = transform->scale();
      Vector scaledMaxHalfDim(
          maxDims.x * scale.x,
          maxDims.y * scale.y,
          maxDims.z * scale.z);
      return BoundingBox(position(), scaledMaxHalfDim);
    }

    virtual BoundingSphere boundingSphere() {
      auto box = boundingBox();
      float radius = sqrt(box.halfDim.x * box.halfDim.x + box.halfDim.y * box.halfDim.y + box.halfDim.z * box.halfDim.z);
      return BoundingSphere(box.position, radius);
    }

    void setupMaterial(GlMaterial& glMaterial, RenderContext* context) {
      glMaterial.bind();
      glMaterial.setMat4("model", transform->worldPorjection());
      glMaterial.setMat4("view", context->viewProjection());
      glMaterial.setMat4("projection", context->screenProjection());
      glMaterial.setVec3("viewPos", context->cameraPosition());

      glMaterial.setInt("numDirLights", context->dirLights().size());
      int idx = 0;
      for (auto lightEntity : context->dirLights()) {
        auto light = lightEntity.get<DirectionalLight>();
        std::stringstream base;
        base << "dirLights[" << idx++ << "]";
        Vector cameraEulers = lightEntity.get<Transform>()->rotation().eulerAngles();
        glMaterial.setVec3(base.str() + ".direction", directionFromEulers(cameraEulers));
        glMaterial.setVec3(base.str() + ".ambient", vec3From(light->ambient()));
        glMaterial.setVec3(base.str() + ".diffuse", vec3From(light->diffuse()));
        glMaterial.setVec3(base.str() + ".specular", vec3From(light->specular()));
      }

      glMaterial.setInt("numPointLights", context->pointLights().size());
      idx = 0;
      for (auto lightEntity : context->pointLights()) {
        auto light = lightEntity.get<PointLight>();
        std::stringstream base;
        base << "pointLights[" << idx++ << "]";
        glMaterial.setVec3(base.str() + ".position", lightEntity.get<Transform>()->position());
        glMaterial.setVec3(base.str() + ".ambient", vec3From(light->ambient()));
        glMaterial.setVec3(base.str() + ".diffuse", vec3From(light->diffuse()));
        glMaterial.setVec3(base.str() + ".specular", vec3From(light->specular()));
        glMaterial.setFloat(base.str() + ".constant", light->constant());
        glMaterial.setFloat(base.str() + ".linear", light->linear());
        glMaterial.setFloat(base.str() + ".quadratic", light->quadratic());
      }

      glMaterial.setInt("numSpotLights", context->spotlights().size());
      idx = 0;
      for (auto lightEntity : context->spotlights()) {
        auto light = lightEntity.get<Spotlight>();
        std::stringstream base;
        base << "spotLights[" << idx++ << "]";
        glMaterial.setVec3(base.str() + ".position", lightEntity.get<Transform>()->position());
        Vector cameraEulers = lightEntity.get<Transform>()->rotation().eulerAngles();
        glMaterial.setVec3(base.str() + ".direction", directionFromEulers(cameraEulers));
        glMaterial.setVec3(base.str() + ".ambient", vec3From(light->ambient()));
        glMaterial.setVec3(base.str() + ".diffuse", vec3From(light->diffuse()));
        glMaterial.setVec3(base.str() + ".specular", vec3From(light->specular()));
        glMaterial.setFloat(base.str() + ".constant", light->constant());
        glMaterial.setFloat(base.str() + ".linear", light->linear());
        glMaterial.setFloat(base.str() + ".quadratic", light->quadratic());
        glMaterial.setFloat(base.str() + ".cutOff", glm::cos(glm::radians(light->cutOff())));
        glMaterial.setFloat(base.str() + ".outerCutoff", glm::cos(glm::radians(light->outerCutOff())));
      }
      auto texture = Texture::Default;
      // if (material.valid()) {
        // TODO; Ref?
        texture = material->texture();
      // }
      GlTexture* glTexture = _textureManager->load(texture);
      glTexture->bind();
      glMaterial.setInt("material.diffuse", 0);
      glMaterial.setFloat("material.shininess", 32.0f);
    }

    virtual void render(RenderContext* context) {
      VertexArray* vao = vaoManager()->load(mesh);
      vao->bind();
      auto program = GlProgram::Default;
      if (!program->loaded()) {
        return;
      }
      GlMaterial material(program);
      setupMaterial(material, context);
      material.bind();
      GL_FAIL_CHECK(glDrawElements(GL_TRIANGLES, mesh->elementCount(), GL_UNSIGNED_INT, 0));
      vao->unbind();
      // glTexture->unbind();  TODO: How do we get this thing..
      material.unbind();
    }

  private:
    VaoManager* _vaoManager;

    GlTextureManager* _textureManager;

    Transform* transform;

    Mesh* mesh;

    Material* material;
  };
}

#endif //MESH_RENDERABLE_H