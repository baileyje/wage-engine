#ifndef RENDERER_PROGRAM_H
#define RENDERER_PROGRAM_H

#include <vector>
#include <glad/glad.h>

#include "core/logger.h"

#include "render-gl/shader.h"

namespace wage {

  class GlProgram {

  public:

    static GlProgram* Default;

    static GlProgram* Font;

    GlProgram(std::string vertexPath, std::string fragmentPath) :
      vertexShader(vertexPath, GL_VERTEX_SHADER), fragmentShader(fragmentPath, GL_FRAGMENT_SHADER), _bound(false), _linked(false) {
    }

    ~GlProgram() {
      GL_FAIL_CHECK(glDeleteProgram(_id));
    }
    
    void load(AssetManager* assetManager) {
      assetManager->load(&vertexShader);
      assetManager->load(&fragmentShader);    
      _id = glCreateProgram();
    }

    void link() {

      // vertexShader.compile();
      // fragmentShader.compile();

      GLint result = GL_FALSE;
      int infoLogLength;

      GL_FAIL_CHECK(glAttachShader(_id, vertexShader.id()));
      GL_FAIL_CHECK(glAttachShader(_id, fragmentShader.id()));
      GL_FAIL_CHECK(glLinkProgram(_id));

      GL_FAIL_CHECK(glGetProgramiv(_id, GL_LINK_STATUS, &result));
      GL_FAIL_CHECK(glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &infoLogLength));
      if ( infoLogLength > 0 ){
        std::vector<char> errorMessage(infoLogLength+1);
        GL_FAIL_CHECK(glGetProgramInfoLog(_id, infoLogLength, NULL, &errorMessage[0]));
        Logger::error("Link error:", &errorMessage[0]);
      }
      _linked = true;
    }

    void bind() {
      if (!_linked) {
        if (!loaded()) {
          return;
        }
        link();
      }
      GL_FAIL_CHECK(glUseProgram(_id));
      _bound = true;
    }

    void unbind() {      
      GL_FAIL_CHECK(glUseProgram(0));
      _bound = false;
    }

    void unlink() {
      GL_FAIL_CHECK(glDetachShader(_id, vertexShader.id()));
      GL_FAIL_CHECK(glDetachShader(_id, fragmentShader.id()));
    }

    inline unsigned int id() {
      return _id;
    }

    inline bool loaded() {
      return vertexShader.loaded() && fragmentShader.loaded();
    }

    inline bool linked() {
      return _linked;
    }
  
  private:
    
    unsigned int _id;

    GlShader vertexShader;
    
    GlShader fragmentShader;

    bool _bound;

    bool _linked;
  };

}

#endif //RENDERER_PROGRAM_H
