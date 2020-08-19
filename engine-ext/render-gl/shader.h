#pragma once

#include <string>
#include <glad/glad.h>

#include "assets/manager.h"

#include "render-gl/util.h"

namespace wage {
  namespace render {
    
    class GlShaderSpec : public assets::AssetSpec {
    public:
      GlShaderSpec(std::string key, GLenum shaderType) : assets::AssetSpec({"shader", key}), shaderType(shaderType) {
      }

      GLenum shaderType;

    };

    class GlShader : public assets::Asset {

    public:
      GlShader(GlShaderSpec spec) : Asset(spec), shaderType(spec.shaderType) {
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

      void onLoad(memory::Buffer buffer) {
        shaderLength = buffer.length();

        // TODO: Check allocator usage. This seems error prone..
        shaderText = (char*)memory::Allocator::Permanent()->allocate(shaderLength);
        memcpy(shaderText, buffer.data(), shaderLength);
      }

      void compile();

    private:
      unsigned int _id;

      GLenum shaderType;

      bool _compiled;

      char* shaderText;

      GLint shaderLength;
    };

  }
}