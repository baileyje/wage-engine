#ifndef MESH_RENDERABLE_H
#define MESH_RENDERABLE_H

#include <sstream>
#include <cmath>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/ext.hpp"

#include "math/matrix.h"

#include "memory/allocator.h"

#include "render/renderable.h"
#include "render-gl/material.h"
#include "render/context.h"

#include "render/mesh.h"
#include "render/material.h"
#include "entity/component/lighting/directional_light.h"
#include "entity/component/lighting/point_light.h"
#include "entity/component/lighting/spotlight.h"


namespace wage {

  class GlMeshRenderable : public Renderable {
    
  public:

    GlMeshRenderable(VaoManager* vaoManager, GlTextureManager* textureManager, Transform transform, Mesh* mesh, Material* material) 
      : vaoManager_(vaoManager), textureManager_(textureManager), transform(transform), mesh(mesh), material(material) {}

    inline VaoManager* vaoManager() {
      return vaoManager_;
    }

    virtual Vector position() {
      return transform.getPosition();
    }

    virtual BoundingBox boundingBox() {
      Vector maxDims = mesh->getMaxDim();
      Vector scale = transform.getScale();
      Vector scaledMaxHalfDim(
        maxDims.x * scale.x,
        maxDims.y * scale.y,
        maxDims.z * scale.z
      );
      return BoundingBox(position(), scaledMaxHalfDim);
    }

    virtual BoundingSphere boundingSphere() {
      auto box = boundingBox();
      float radius = sqrt(box.halfDim.x * box.halfDim.x + box.halfDim.y * box.halfDim.y + box.halfDim.z * box.halfDim.z);
      // printf("HalfDim: %f:%f:%f\n", box.halfDim.x, box.halfDim.y, box.halfDim.z);
      // printf("Radius: %f\n", radius);
      return BoundingSphere(box.position, radius);
    }

    void setupMaterial(GlMaterial& glMaterial, RenderContext* context) {
      glMaterial.bind();
      glMaterial.setMat4("model", transform.worldPorjection());
      glMaterial.setMat4("view", context->viewProjection());
      glMaterial.setMat4("projection", context->screenProjection());
      glMaterial.setVec3("viewPos", context->cameraPosition());
      
      glMaterial.setInt("numDirLights", context->dirLights().size());
      int idx = 0;
      for (auto light : context->dirLights()) {
        std::stringstream base;
        base << "dirLights[" << idx++ << "]";
        Vector cameraEulers = glm::eulerAngles(light->getTransform()->getRotation());
        glMaterial.setVec3(base.str() + ".direction", directionFromEulers(cameraEulers));
        glMaterial.setVec3(base.str() + ".ambient", vec3From(light->getAmbient()));
        glMaterial.setVec3(base.str() + ".diffuse", vec3From(light->getDiffuse()));
        glMaterial.setVec3(base.str() + ".specular", vec3From(light->getSpecular()));
      }
      
      glMaterial.setInt("numPointLights", context->pointLights().size());
      idx = 0;
      for (auto light : context->pointLights()) {
        std::stringstream base;
        base << "pointLights[" << idx++ << "]";
        glMaterial.setVec3(base.str() + ".position", light->getTransform()->getPosition());
        glMaterial.setVec3(base.str() + ".ambient", vec3From(light->getAmbient()));
        glMaterial.setVec3(base.str() + ".diffuse", vec3From(light->getDiffuse()));
        glMaterial.setVec3(base.str() + ".specular", vec3From(light->getSpecular()));
        glMaterial.setFloat(base.str() + ".constant", light->getConstant());
        glMaterial.setFloat(base.str() + ".linear", light->getLinear());
        glMaterial.setFloat(base.str() + ".quadratic", light->getQuadratic());
      }

      glMaterial.setInt("numSpotLights", context->spotlights().size());
      idx = 0;
      for (auto light : context->spotlights()) {
        std::stringstream base;
        base << "spotLights[" << idx++ << "]";
        glMaterial.setVec3(base.str() + ".position", light->getTransform()->getPosition());
        Vector cameraEulers = glm::eulerAngles(light->getTransform()->getRotation());
        glMaterial.setVec3(base.str() + ".direction", directionFromEulers(cameraEulers));
        glMaterial.setVec3(base.str() + ".ambient", vec3From(light->getAmbient()));
        glMaterial.setVec3(base.str() + ".diffuse", vec3From(light->getDiffuse()));
        glMaterial.setVec3(base.str() + ".specular", vec3From(light->getSpecular()));
        glMaterial.setFloat(base.str() + ".constant", light->getConstant());
        glMaterial.setFloat(base.str() + ".linear", light->getLinear());
        glMaterial.setFloat(base.str() + ".quadratic", light->getQuadratic());
        glMaterial.setFloat(base.str() + ".cutOff", glm::cos(glm::radians(light->getCutOff())));
        glMaterial.setFloat(base.str() + ".outerCutoff", glm::cos(glm::radians(light->getOuterCutOff())));
      }
      Texture* texture = Texture::Default;
      if (material != nullptr) {
        texture = material->getTexture();
        if (material->getTexture() != nullptr) {
          texture = material->getTexture();
        }
      }
      GlTexture* glTexture = textureManager_->load(texture);
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
      GL_FAIL_CHECK(glDrawElements(GL_TRIANGLES, mesh->getElementCount(), GL_UNSIGNED_INT, 0));
      vao->unbind();
      // glTexture->unbind();  TODO: How do we get this thing..
      material.unbind();
    }

  private:

    VaoManager* vaoManager_;

    GlTextureManager* textureManager_;

    Transform transform;

    Mesh* mesh;

    Material* material;

  };

}

#endif //MESH_RENDERABLE_H