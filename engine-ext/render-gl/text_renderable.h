#pragma once

#include <string>

#include "render/renderable.h"
#include "render/context.h"

#include "render-gl/material.h"
#include "render-gl/font.h"
#include "render-gl/font_manager.h"

namespace wage { namespace render {

  class GlTextRenderable : public Renderable {

  public:
    GlTextRenderable(GlFontManager* fontManager, math::Vector2 position, std::string text, Font font, math::Color color)
        : _fontManager(fontManager), _position(position), _text(text), _font(font), _color(color) {}

    virtual math::Vector position() {
      return _position;
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
      GlFont* font = _fontManager->load(_font);
      if (!font->loaded()) {
        return;
      }

      math::Matrix projection = math::Matrix::orthographic(0.0f, context->screenSize().x, 0.0f, context->screenSize().y);

      GLuint VAO, VBO;
      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);
      glBindVertexArray(VAO);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);

      GlProgram* program = GlProgram::Font;
      program->bind();
      glUniformMatrix4fv(glGetUniformLocation(program->id(), "projection"), 1, GL_FALSE, glm::value_ptr(projection.glm()));
      glUniform3f(glGetUniformLocation(program->id(), "textColor"), _color.r, _color.g, _color.b);
      glActiveTexture(GL_TEXTURE0);
      glBindVertexArray(VAO);

      float scale = 1;
      float x = _position.x;

      // Iterate through all characters
      std::string::const_iterator c;
      for (c = _text.begin(); c != _text.end(); c++) {
        GlCharacter* ch = font->characterFor(*c);
        ch->bind();

        GLfloat xpos = x + ch->bearing.x * scale;
        GLfloat ypos = _position.y - (ch->size.y - ch->bearing.y) * scale;

        GLfloat w = ch->size.x * scale;
        GLfloat h = ch->size.y * scale;

        // Update VBO for each character
        GLfloat vertices[6][4] = {
            {xpos, ypos + h, 0.0, 0.0},
            {xpos, ypos, 0.0, 1.0},
            {xpos + w, ypos, 1.0, 1.0},

            {xpos, ypos + h, 0.0, 0.0},
            {xpos + w, ypos, 1.0, 1.0},
            {xpos + w, ypos + h, 1.0, 0.0}};
        // Render glyph texture over quad
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch->advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
      }
      glBindVertexArray(0);
      glBindTexture(GL_TEXTURE_2D, 0);
    }

  private:
    GlFontManager* _fontManager;

    math::Vector2 _position;

    std::string _text;

    Font _font;

    math::Color _color;
  };

} }
