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
      GL_FAIL_CHECK(glDeleteShader(_id));
    }

    inline unsigned int id() {
      return _id;
    }

    void onLoad();

    void compile();

  private:

    unsigned int _id;

    GLenum shaderType;
  };

}

#endif //RENDERER_SHADER_H