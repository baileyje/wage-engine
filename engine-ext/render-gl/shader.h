#ifndef RENDERER_SHADER_H
#define RENDERER_SHADER_H

#include <string>
#include <glad/glad.h>

#include "assets/manager.h"

#include "render-gl/util.h"

namespace wage {

  class GlShader : public Asset {

  public:

    GlShader(std::string path, GLenum shaderType) : Asset(path), shaderType(shaderType) {      
    }

    virtual ~GlShader() {
      GL_FAIL_CHECK(glDeleteShader(id_));
    }

    inline unsigned int id() {
      return id_;
    }

    void onLoad(Buffer* buffer);

    void compile();

  private:

    unsigned int id_;

    GLenum shaderType;
  };

}

#endif //RENDERER_SHADER_H