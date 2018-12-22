#ifndef RENDERER_FONT_H
#define RENDERER_FONT_H

#include <map>
#include <iostream>
#include <glad/glad.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "math/vector.h"
#include "assets/manager.h"
#include "assets/memory_buffer.h"

#include "render-gl/util.h"

namespace wage {

  class GlCharacter {
  
  public:

    GlCharacter(Vector2 size, Vector2 bearing, unsigned int advance) : textureId(0), size(size), bearing(bearing), advance(advance), bound(false), pushed(false) {}


    inline void bind() {
      if (bound) {
        return;
      }
      if (!pushed) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
        printf("OnLoad5\n");
          // Generate texture
        GL_FAIL_CHECK(glGenTextures(1, &textureId));
        printf("OnLoad6\n");
        GL_FAIL_CHECK(glBindTexture(GL_TEXTURE_2D, textureId));
        printf("OnLoad7\n");
        printf("%s\n", buffer.data());
        GL_FAIL_CHECK(glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            size.x,
            size.y,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            buffer.data()
        ));
        printf("OnLoad8\n");
        // Set texture options
        GL_FAIL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_FAIL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GL_FAIL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_FAIL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        printf("OnLoad9\n");
        pushed = true;
      }
      GL_FAIL_CHECK(glBindTexture(GL_TEXTURE_2D, textureId));
    }


    unsigned int textureId;

    MemoryBuffer buffer;
    
    Vector2 size;
    
    Vector2 bearing;
    
    unsigned int advance;

    bool bound;

    bool pushed;
  
  };


  class GlFont : public Asset {

  public:

    static GlFont* JankyFont;

    GlFont(std::string path, int size = 48) : Asset(path), size(size) {      
    }

    inline GlCharacter* characterFor(char c) {
      auto found = characters.find(c);
      if (found != characters.end()) {
        return &found->second;
      }
      return nullptr;    
    }

    void onLoad(Buffer* buffer) {
      printf("OnLoad1\n");
      FT_Library freeType;
      if (FT_Init_FreeType(&freeType))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
      printf("OnLoad2\n");
      FT_Face face;
      if (FT_New_Memory_Face(freeType, buffer->data(), buffer->length(),  0, &face))
          std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
      printf("OnLoad3\n");
      FT_Set_Pixel_Sizes(face, 0, size);
      printf("OnLoad4\n");
      for (GLubyte c = 0; c < 128; c++) {
        // Load character glyph 
        FT_Error error = FT_Load_Char(face, c, FT_LOAD_RENDER);
        if (error) {
            printf("Error: %d\n", error);
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        GlCharacter character(            
            Vector2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            Vector2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        );
        character.buffer.fill(face->glyph->bitmap.buffer, sizeof(GL_UNSIGNED_BYTE) * character.size.x * character.size.y);        
        characters.insert(std::pair<char, GlCharacter>(c, character));        
      }
      FT_Done_Face(face);
      FT_Done_FreeType(freeType );
      // exit(0);
    }

  private:

    int size;

    std::map<char, GlCharacter> characters;

  };

}

#endif //RENDERER_FONT_H
