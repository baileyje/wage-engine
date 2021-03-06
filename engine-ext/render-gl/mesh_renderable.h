#pragma once

#include <sstream>
#include <cmath>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/ext.hpp"

#include "asset/manager.h"
#include "math/matrix.h"
#include "memory/allocator.h"
#include "render/renderable.h"
#include "render/context.h"
#include "render/components/mesh.h"
#include "render/components/material.h"
#include "render/components/lighting/directional_light.h"
#include "render/components/lighting/point_light.h"
#include "render/components/lighting/spotlight.h"

#include "render-gl/material.h"

namespace wage {
  namespace render {

    class GlMeshRenderable : public Renderable {

    public:
      GlMeshRenderable(asset::Manager* assetManager, VaoManager* vaoManager, math::Transform transform, Mesh* meshData, MaterialSpec material)
          : _assetManager(assetManager), _vaoManager(vaoManager), transform(transform), meshData(meshData), material(material) {}

      inline VaoManager* vaoManager() {
        return _vaoManager;
      }

      virtual math::Vector position() {
        return transform.localPosition;
      }

      virtual math::BoundingBox boundingBox() {
        if (!meshData->loaded()) {
          return math::BoundingBox(position(), {0, 0, 0});
        }
        math::Vector maxDims = meshData->maxDim;
        math::Vector scale = transform.localScale;
        math::Vector scaledMaxHalfDim(
            maxDims.x * scale.x,
            maxDims.y * scale.y,
            maxDims.z * scale.z);
        return math::BoundingBox(position(), scaledMaxHalfDim);
      }

      virtual math::BoundingSphere boundingSphere() {
        auto box = boundingBox();
        float radius = sqrt(box.halfDim.x * box.halfDim.x + box.halfDim.y * box.halfDim.y + box.halfDim.z * box.halfDim.z);
        return math::BoundingSphere(box.position, radius);
      }

      void setupMaterial(GlMaterial& glMaterial, RenderContext* context) {
        glMaterial.bind();
        glMaterial.setMat4("model", transform.worldProjection());
        glMaterial.setMat4("view", context->viewProjection());
        glMaterial.setMat4("projection", context->screenProjection());
        glMaterial.setVec3("viewPos", context->cameraPosition());

        // glMaterial.setInt("numDirLights", context->dirLights().size());
        int idx = 0;
        // for (auto lightEntity : context->dirLights()) {
        //   auto light = lightEntity.get<DirectionalLight>(DirectionalLightComponent);
        //   std::stringstream base;
        //   base << "dirLights[" << idx++ << "]";
        //   auto lightTransform = lightEntity.get<math::Transform>(TransformComponent);
        //   auto cameraEulers = lightTransform->rotation().eulerAngles();
        //   glMaterial.setVec3(base.str() + ".direction", directionFromEulers(cameraEulers));
        //   glMaterial.setVec3(base.str() + ".ambient", vec3From(light->ambient()));
        //   glMaterial.setVec3(base.str() + ".diffuse", vec3From(light->diffuse()));
        //   glMaterial.setVec3(base.str() + ".specular", vec3From(light->specular()));
        // }
        // TEMP: Remove when lighting data is clean for render
        glMaterial.setInt("numDirLights", 2);
        glMaterial.setVec3("dirLights[0].direction", directionFromEulers(math::Vector(0, -1, 0)));
        glMaterial.setVec3("dirLights[0].ambient", math::Vector(0.4, 0.4, 0.4));
        glMaterial.setVec3("dirLights[0].diffuse", math::Vector(0.7, 0.7, 0.7));
        glMaterial.setVec3("dirLights[0].specular", math::Vector(0.5f, 0.5f, 0.5f));

        glMaterial.setVec3("dirLights[1].direction", directionFromEulers(math::Vector(0, 1, 0)));
        glMaterial.setVec3("dirLights[1].ambient", math::Vector(0.4, 0.4, 0.4));
        glMaterial.setVec3("dirLights[1].diffuse", math::Vector(0.7, 0.7, 0.9));
        glMaterial.setVec3("dirLights[1].specular", math::Vector(0.5f, 0.5f, 0.5f));

        glMaterial.setInt("numPointLights", 0);
        // glMaterial.setInt("numPointLights", context->pointLights().size());
        // idx = 0;
        // for (auto lightEntity : context->pointLights())
        // {
        //   auto light = lightEntity.get<PointLight>(PointLightComponent);
        //   std::stringstream base;
        //   base << "pointLights[" << idx++ << "]";
        //   auto lightTransform = lightEntity.get<math::Transform>(TransformComponent);
        //   glMaterial.setVec3(base.str() + ".position", lightTransform->position());
        //   glMaterial.setVec3(base.str() + ".ambient", vec3From(light->ambient()));
        //   glMaterial.setVec3(base.str() + ".diffuse", vec3From(light->diffuse()));
        //   glMaterial.setVec3(base.str() + ".specular", vec3From(light->specular()));
        //   glMaterial.setFloat(base.str() + ".constant", light->constant());
        //   glMaterial.setFloat(base.str() + ".linear", light->linear());
        //   glMaterial.setFloat(base.str() + ".quadratic", light->quadratic());
        // }

        glMaterial.setInt("numSpotLights", 0);
        // glMaterial.setInt("numSpotLights", context->spotlights().size());
        // idx = 0;
        // for (auto lightEntity : context->spotlights())
        // {
        //   auto light = lightEntity.get<Spotlight>(SpotlightComponent);
        //   std::stringstream base;
        //   base << "spotLights[" << idx++ << "]";
        //   auto lightTransform = lightEntity.get<math::Transform>(TransformComponent);
        //   glMaterial.setVec3(base.str() + ".position", lightTransform->position());
        //   auto cameraEulers = lightTransform->rotation().eulerAngles();
        //   glMaterial.setVec3(base.str() + ".direction", directionFromEulers(cameraEulers));
        //   glMaterial.setVec3(base.str() + ".ambient", vec3From(light->ambient()));
        //   glMaterial.setVec3(base.str() + ".diffuse", vec3From(light->diffuse()));
        //   glMaterial.setVec3(base.str() + ".specular", vec3From(light->specular()));
        //   glMaterial.setFloat(base.str() + ".constant", light->constant());
        //   glMaterial.setFloat(base.str() + ".linear", light->linear());
        //   glMaterial.setFloat(base.str() + ".quadratic", light->quadratic());
        //   glMaterial.setFloat(base.str() + ".cutOff", glm::cos(glm::radians(light->cutOff())));
        //   glMaterial.setFloat(base.str() + ".outerCutoff", glm::cos(glm::radians(light->outerCutOff())));
        // }
        GlTexture* glTexture = _assetManager->load<GlTexture>(material.texture());
        glTexture->bind();
        glMaterial.setInt("material.diffuse", 0);
        glMaterial.setFloat("material.shininess", 32.0f);
      }

      virtual void render(RenderContext* context) {
        if (!meshData->loaded()) {
          return;
        }
        auto vao = vaoManager()->load(meshData);
        if (vao == nullptr) {
          return;
        }
        vao->bind();
        auto program = GlProgram::Default;
        if (!program->loaded()) {
          return;
        }
        GlMaterial material(program);
        setupMaterial(material, context);
        material.bind();
        GL_FAIL_CHECK(glDrawElements(GL_TRIANGLES, meshData->elementCount(), GL_UNSIGNED_INT, 0));
        vao->unbind();
        // glTexture->unbind();  TODO: How do we get this thing..
        material.unbind();
      }

    private:
      asset::Manager* _assetManager;

      VaoManager* _vaoManager;

      math::Transform transform;

      Mesh* meshData;

      MaterialSpec material;
    };

  } // namespace render
} // namespace wage
