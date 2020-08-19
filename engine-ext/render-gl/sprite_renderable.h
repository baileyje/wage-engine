#pragma once

#include "glm/gtc/matrix_transform.hpp"

#include "render/renderable.h"
#include "render/context.h"

#include "render-gl/material.h"
#include "render-gl/texture.h"

namespace wage {
  namespace render {

    class GlSpriteRenderable : public Renderable {

    public:
      GlSpriteRenderable(assets::Manager* assetManager, math::Vector2 position, math::Vector2 size, math::Color color, Texture texture)
          : _assetManager(assetManager), _position(position), _size(size), _color(color), _texture(texture) {}

      virtual math::Vector position() {
        return _position;
      }

      virtual math::Vector size() const {
        return _size;
      }

      virtual math::BoundingBox boundingBox() {
        return math::BoundingBox(position(), math::Vector(1000, 1000, 1000));
      }

      virtual math::BoundingSphere boundingSphere() {
        auto box = boundingBox();
        float radius = sqrt(box.halfDim.x * box.halfDim.x + box.halfDim.y * box.halfDim.y + box.halfDim.z * box.halfDim.z);
        return math::BoundingSphere(box.position, radius);
      }

      virtual void render(RenderContext* context) {
        math::Matrix projection = math::Matrix::orthographic(0.0f, context->screenSize().x, 0.0f, context->screenSize().y);

        // Configure VAO/VBO
        GLuint VBO, VAO;

        // VertexVector {
        //       Vector(0.5,  0.5, 0.0f),  // top right
        //       Vector(0.5f, -0.5f, 0.0f),  // bottom right
        //       Vector(-0.5f, -0.5f, 0.0f),  // bottom left
        //       Vector(-0.5f,  0.5f, 0.0f)   // top left
        //     },
        //     VertexVector {
        //       Vector(0.0f,  0.0f, -1.0f),  // top right
        //       Vector(0.0f,  0.0f, -1.0f),  // bottom right
        //       Vector(0.0f,  0.0f, -1.0f),  // bottom left
        //       Vector(0.0f,  0.0f, -1.0f)   // top left
        //     },
        //     Vertex2Vector {
        //       Vector2(1.0f,  0.0f),  // top right
        //       Vector2(1.0f,  1.0f),  // bottom right
        //       Vector2(0.0f,  1.0f),  // bottom left
        //       Vector2(0.0f,  0.0f)   // top left
        //     },

        GLfloat vertices[] = {
            // Pos      // Tex
            1.0f, 1.0f, 1.0f, 0.0f,
            1.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 0.0f, 0.0f, 1.0f,

            0.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f};

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(VAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        GlProgram* program = GlProgram::Sprite;
        if (!program->loaded()) {
          return;
        }
        program->bind();

        math::Matrix translation = math::Matrix(1).translate(position());
        math::Matrix scale = math::Matrix(1).scale(math::Vector(_size.x, _size.y, 1));
        math::Matrix rotate = math::Matrix(1);
        auto model = translation * rotate * scale;

        glUniformMatrix4fv(glGetUniformLocation(program->id(), "projection"), 1, GL_FALSE, glm::value_ptr(projection.glm()));
        glUniformMatrix4fv(glGetUniformLocation(program->id(), "model"), 1, GL_FALSE, glm::value_ptr(model.glm()));
        glUniform3f(glGetUniformLocation(program->id(), "spriteColor"), _color.r, _color.g, _color.b);
        // glActiveTexture(GL_TEXTURE0);

        GlTexture* glTexture = _assetManager->load<GlTexture>(_texture);
        glTexture->bind();

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
      }

    private:
      assets::Manager* _assetManager;

      math::Vector2 _position;

      math::Vector2 _size;

      math::Color _color;

      Texture _texture;
    };

  }
}
