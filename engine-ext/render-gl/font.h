#pragma once

#include <map>
#include <iostream>
#include <glad/glad.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "math/vector.h"
#include "assets/manager.h"
#include "memory/buffer.h"

#include "render-gl/util.h"

namespace wage { namespace render {

  class GlCharacter {

  public:
    GlCharacter(math::Vector2 size, math::Vector2 bearing, unsigned int advance) : textureId(0), size(size), bearing(bearing), advance(advance), bound(false), pushed(false) {}

    inline void bind() {
      if (bound) {
        return;
      }
      if (!pushed) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        GL_FAIL_CHECK(glGenTextures(1, &textureId));
        GL_FAIL_CHECK(glBindTexture(GL_TEXTURE_2D, textureId));
        GL_FAIL_CHECK(glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            size.x,
            size.y,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            buffer.data()));
        // Set texture options
        GL_FAIL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_FAIL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GL_FAIL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_FAIL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        pushed = true;
      }
      GL_FAIL_CHECK(glBindTexture(GL_TEXTURE_2D, textureId));
    }

    unsigned int textureId;

    memory::Buffer buffer;

    math::Vector2 size;

    math::Vector2 bearing;

    unsigned int advance;

    bool bound;

    bool pushed;
  };

  class GlFont : public assets::Asset {

  public:
    GlFont(std::string path, int size = 48) : Asset(path), size(size) {
    }

    inline GlCharacter* characterFor(char c) {
      auto found = characters.find(c);
      if (found != characters.end()) {
        return &found->second;
      }
      return nullptr;
    }

    void onLoad() {
      FT_Library freeType;
      if (FT_Init_FreeType(&freeType))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
      FT_Face face;
      if (FT_New_Memory_Face(freeType, buffer->data(), buffer->length(), 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
      FT_Set_Pixel_Sizes(face, 0, size);
      for (GLubyte c = 0; c < 128; c++) {
        // Load character glyph
        FT_Error error = FT_Load_Char(face, c, FT_LOAD_RENDER);
        if (error) {
          printf("Error: %d\n", error);
          std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
          continue;
        }

        GlCharacter character(
            math::Vector2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            math::Vector2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x);
        character.buffer.fill(face->glyph->bitmap.buffer, sizeof(GL_UNSIGNED_BYTE) * character.size.x * character.size.y);
        characters.insert(std::pair<char, GlCharacter>(c, character));
      }
      FT_Done_Face(face);
      FT_Done_FreeType(freeType);
    }

  private:
    int size;

    std::map<char, GlCharacter> characters;
  };

} }