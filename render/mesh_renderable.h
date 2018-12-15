#ifndef MESH_RENDERABLE_H
#define MESH_RENDERABLE_H

#include <sstream>
#include <cmath>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/ext.hpp"


#include "math/matrix.h"

#include "render/renderable.h"
#include "render/material.h"
#include "render/context.h"

#include "entity/component/render/mesh.h"
#include "entity/component/render/material.h"
#include "entity/component/lighting/directional_light.h"
#include "entity/component/lighting/point_light.h"
#include "entity/component/lighting/spotlight.h"

namespace wage {

  class MeshRenderable : public Renderable {
    
  public:

    MeshRenderable(Vector position, Vector scale, Mesh* mesh, Matrix model, Material* material) : position_(position), scale_(scale), mesh(mesh), model(model), material(material) {}

    virtual Vector position() {
      return position_;
    }

    virtual BoundingBox boundingBox() {
      Vector maxDims = mesh->getMaxDim();
      Vector scaledMaxHalfDim(
        maxDims.x * scale_.x,
        maxDims.y * scale_.y,
        maxDims.z * scale_.z
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

    GlMaterial* buildMaterial(RenderContext* context) {      
      GlMaterial* glMaterial = new GlMaterial(Shader::Default);
      glMaterial->bind();
      glMaterial->setMat4("model", model);
      glMaterial->setMat4("view", context->viewProjection());
      glMaterial->setMat4("projection", context->screenProjection());
      glMaterial->setVec3("viewPos", context->cameraPosition());
      
      glMaterial->setInt("numDirLights", context->dirLights().size());
      int idx = 0;
      for (auto light : context->dirLights()) {
        std::stringstream base;
        base << "dirLights[" << idx++ << "]";
        Vector cameraEulers = glm::eulerAngles(light->getTransform()->getRotation());
        glMaterial->setVec3(base.str() + ".direction", directionFromEulers(cameraEulers));
        glMaterial->setVec3(base.str() + ".ambient", vec3From(light->getAmbient()));
        glMaterial->setVec3(base.str() + ".diffuse", vec3From(light->getDiffuse()));
        glMaterial->setVec3(base.str() + ".specular", vec3From(light->getSpecular()));
      }
      
      glMaterial->setInt("numPointLights", context->pointLights().size());
      idx = 0;
      for (auto light : context->pointLights()) {
        std::stringstream base;
        base << "pointLights[" << idx++ << "]";
        glMaterial->setVec3(base.str() + ".position", light->getTransform()->getPosition());
        glMaterial->setVec3(base.str() + ".ambient", vec3From(light->getAmbient()));
        glMaterial->setVec3(base.str() + ".diffuse", vec3From(light->getDiffuse()));
        glMaterial->setVec3(base.str() + ".specular", vec3From(light->getSpecular()));
        glMaterial->setFloat(base.str() + ".constant", light->getConstant());
        glMaterial->setFloat(base.str() + ".linear", light->getLinear());
        glMaterial->setFloat(base.str() + ".quadratic", light->getQuadratic());
      }

      glMaterial->setInt("numSpotLights", context->spotlights().size());
      idx = 0;
      for (auto light : context->spotlights()) {
        std::stringstream base;
        base << "spotLights[" << idx++ << "]";
        glMaterial->setVec3(base.str() + ".position", light->getTransform()->getPosition());
        Vector cameraEulers = glm::eulerAngles(light->getTransform()->getRotation());
        glMaterial->setVec3(base.str() + ".direction", directionFromEulers(cameraEulers));
        glMaterial->setVec3(base.str() + ".ambient", vec3From(light->getAmbient()));
        glMaterial->setVec3(base.str() + ".diffuse", vec3From(light->getDiffuse()));
        glMaterial->setVec3(base.str() + ".specular", vec3From(light->getSpecular()));
        glMaterial->setFloat(base.str() + ".constant", light->getConstant());
        glMaterial->setFloat(base.str() + ".linear", light->getLinear());
        glMaterial->setFloat(base.str() + ".quadratic", light->getQuadratic());
        glMaterial->setFloat(base.str() + ".cutOff", glm::cos(glm::radians(light->getCutOff())));
        glMaterial->setFloat(base.str() + ".outerCutoff", glm::cos(glm::radians(light->getOuterCutOff())));
      }
      GlTexture* glTexture = GlTexture::Default;
      if (material != nullptr) {
        Texture* texture = material->getTexture();
        if (texture != nullptr) {
          glTexture = context->textureManager()->load(texture);
        }
      }
      glTexture->bind();
      glMaterial->setInt("material.diffuse", 0);
      glMaterial->setFloat("material.shininess", 32.0f);

      return glMaterial;
    }

    virtual void render(RenderContext* context) {      
      VertexArray* vao = context->vaoManager()->load(mesh);
      vao->bind();
      GlMaterial* material = buildMaterial(context);
      material->bind();
      GL_FAIL_CHECK(glDrawElements(GL_TRIANGLES, mesh->getElementCount(), GL_UNSIGNED_INT, 0));
      vao->unbind();
      // glTexture->unbind();  TODO: How do we get this thing..
      material->unbind();
      delete material;
    }

  private:

    Vector position_;

    Vector scale_;

    Mesh* mesh;

    Matrix model;

    Material* material;

  };

}

#endif //MESH_RENDERABLE_H