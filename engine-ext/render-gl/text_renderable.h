#ifndef TEXT_RENDERABLE_H
#define TEXT_RENDERABLE_H

#include <string>

#include "engine/render/renderable.h"
#include "engine/render/context.h"

#include "engine-ext/render-gl/material.h"
#include "engine-ext/render-gl/font.h"
#include "engine-ext/render-gl/font_manager.h"

namespace wage {

  class GlTextRenderable : public Renderable {
    
  public:

    GlTextRenderable(GlFontManager* fontManager, Vector position, std::string text, Font font, Color color) 
      : fontManager_(fontManager), position_(position), text_(text), font_(font), color_(color) {}

    virtual Vector position() {
      return position_;
    }

    virtual BoundingBox boundingBox() {      
      return BoundingBox(position(), Vector3(1000, 1000, 1000));
    }

    virtual BoundingSphere boundingSphere() {
      auto box = boundingBox();
      float radius = sqrt(box.halfDim.x * box.halfDim.x + box.halfDim.y * box.halfDim.y + box.halfDim.z * box.halfDim.z);      
      return BoundingSphere(box.position, radius);
    }

    virtual void render(RenderContext* context) {      
      GlFont* font = fontManager_->load(font_);
      if (!font->loaded()) {
        return;
      }

      Matrix projection = glm::ortho(0.0f, context->screenSize().x, 0.0f, context->screenSize().y);

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
      glUniformMatrix4fv(glGetUniformLocation(program->id(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
      glUniform3f(glGetUniformLocation(program->id(), "textColor"), color_.r, color_.g, color_.b);
      glActiveTexture(GL_TEXTURE0);
      glBindVertexArray(VAO);

      float scale = 1;
      float x = position_.x;

      
      // Iterate through all characters
      std::string::const_iterator c;
      for (c = text_.begin(); c != text_.end(); c++) {
          GlCharacter* ch = font->characterFor(*c);
          ch->bind();

          GLfloat xpos = x + ch->bearing.x * scale;
          GLfloat ypos = position_.y - (ch->size.y - ch->bearing.y) * scale;

          GLfloat w = ch->size.x * scale;
          GLfloat h = ch->size.y * scale;

          // Update VBO for each character
          GLfloat vertices[6][4] = {
              { xpos,     ypos + h,   0.0, 0.0 },            
              { xpos,     ypos,       0.0, 1.0 },
              { xpos + w, ypos,       1.0, 1.0 },

              { xpos,     ypos + h,   0.0, 0.0 },
              { xpos + w, ypos,       1.0, 1.0 },
              { xpos + w, ypos + h,   1.0, 0.0 }           
          };
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

    GlFontManager* fontManager_;

    Vector position_;

    std::string text_;

    Font font_;

    Color color_;

  };

}

#endif //MESH_RENDERABLE_H