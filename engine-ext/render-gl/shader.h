#pragma once

#include <string>
#include <glad/glad.h>

#include "asset/manager.h"

#include "render-gl/util.h"

namespace wage {
  namespace render {

    class GlShaderSpec : public asset::AssetSpec {
    public:
      GlShaderSpec(std::string key, GLenum shaderType) : asset::AssetSpec({"shader", key}), shaderType(shaderType) {
      }

      GLenum shaderType;
    };

    class GlShader : public asset::Asset {

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

      bool onLoad(memory::InputStream* stream, memory::Allocator* allocator) {
        shaderLength = stream->size();
        shaderText = (char*)memory::Allocator::Permanent()->allocate(shaderLength);
        stream->read((memory::Byte*)shaderText, shaderLength);
        return true;
      }

      void compile() {
        GL_FAIL_CHECK(_id = glCreateShader(shaderType));
        GL_FAIL_CHECK(glShaderSource(_id, 1, &shaderText, &shaderLength));
        GL_FAIL_CHECK(glCompileShader(_id));

        GLint result = GL_FALSE;
        int infoLogLength;
        GL_FAIL_CHECK(glGetShaderiv(_id, GL_COMPILE_STATUS, &result));
        GL_FAIL_CHECK(glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &infoLogLength));
        if (infoLogLength > 0) {
          std::vector<char> errorMessage(infoLogLength + 1);
          GL_FAIL_CHECK(glGetShaderInfoLog(_id, infoLogLength, NULL, &errorMessage[0]));
          core::Logger::error("Compile Error: ", &errorMessage[0]);
        }
        _compiled = true;
      }

    private:
      unsigned int _id;

      GLenum shaderType;

      bool _compiled;

      char* shaderText;

      GLint shaderLength;
    };

  }
}