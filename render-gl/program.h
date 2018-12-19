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

    GlProgram(std::string vertexPath, std::string fragmentPath) :
      vertexShader(vertexPath, GL_VERTEX_SHADER), fragmentShader(fragmentPath, GL_FRAGMENT_SHADER), linked_(false) {
    }

    ~GlProgram() {
      GL_FAIL_CHECK(glDeleteProgram(id_));
    }
    
    void load(AssetManager* assetManager) {
      assetManager->load(&vertexShader);
      assetManager->load(&fragmentShader);    
      id_ = glCreateProgram();
    }

    void link() {

      vertexShader.compile();
      fragmentShader.compile();

      GLint result = GL_FALSE;
      int infoLogLength;

      GL_FAIL_CHECK(glAttachShader(id_, vertexShader.id()));
      GL_FAIL_CHECK(glAttachShader(id_, fragmentShader.id()));
      GL_FAIL_CHECK(glLinkProgram(id_));

      GL_FAIL_CHECK(glGetProgramiv(id_, GL_LINK_STATUS, &result));
      GL_FAIL_CHECK(glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &infoLogLength));
      if ( infoLogLength > 0 ){
        std::vector<char> errorMessage(infoLogLength+1);
        GL_FAIL_CHECK(glGetProgramInfoLog(id_, infoLogLength, NULL, &errorMessage[0]));
        Logger::error("%s\n", &errorMessage[0]);
      }
      linked_ = true;
    }

    void bind() {
      if (!linked_) {
        if (!loaded()) {
          return;
        }
        link();
      }
      GL_FAIL_CHECK(glUseProgram(id_));
      bound_ = true;
    }

    void unbind() {      
      GL_FAIL_CHECK(glUseProgram(0));
      bound_ = false;
    }

    void unlink() {
      GL_FAIL_CHECK(glDetachShader(id_, vertexShader.id()));
      GL_FAIL_CHECK(glDetachShader(id_, fragmentShader.id()));
    }

    inline unsigned int id() {
      return id_;
    }

    inline bool loaded() {
      return vertexShader.loaded() && fragmentShader.loaded();
    }

    inline bool linked() {
      return linked_;
    }
  
  private:
    
    unsigned int id_;

    GlShader vertexShader;
    
    GlShader fragmentShader;

    bool bound_;

    bool linked_;
  };

}

#endif //RENDERER_PROGRAM_H
