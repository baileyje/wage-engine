#pragma once

#include <string>
#include <glad/glad.h>

#include "assets/manager.h"

#include "render-gl/util.h"

namespace wage {
  namespace render {

    class GlShader : public assets::Asset {

    public:
      GlShader(std::string path, GLenum shaderType) : Asset("shader", path), shaderType(shaderType) {
      }

      virtual ~GlShader() {
        GL_FAIL_CHECK(glDeleteShader(_id));
      }

      inline unsigned int id() {
        return _id;
      }

      inline bool compiled() {
        return _compiled;
      }

      void onLoad();

      void compile();

    private:
      unsigned int _id;

      GLenum shaderType;

      bool _compiled;
    };

  }
}