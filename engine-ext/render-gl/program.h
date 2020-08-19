#pragma once

#include <vector>
#include <glad/glad.h>

#include "core/logger.h"

#include "render-gl/shader.h"

namespace wage {
  namespace render {

    class GlProgram {

    public:
      static GlProgram* Default;

      static GlProgram* Font;

      static GlProgram* Sprite;

      GlProgram(std::string vertexPath, std::string fragmentPath) : vertexShaderSpec(vertexPath, GL_VERTEX_SHADER), fragmentShaderSpec(fragmentPath, GL_FRAGMENT_SHADER), _bound(false), _linked(false) {
      }

      ~GlProgram() {
        GL_FAIL_CHECK(glDeleteProgram(_id));
      }

      void load(assets::Manager* assetManager) {
        vertexShader = assetManager->load<GlShader>(vertexShaderSpec);
        fragmentShader = assetManager->load<GlShader>(fragmentShaderSpec);
        _id = glCreateProgram();
      }

      void link() {

        // vertexShader.compile();
        // fragmentShader.compile();

        GLint result = GL_FALSE;
        int infoLogLength;

        GL_FAIL_CHECK(glAttachShader(_id, vertexShader->id()));
        GL_FAIL_CHECK(glAttachShader(_id, fragmentShader->id()));
        GL_FAIL_CHECK(glLinkProgram(_id));

        GL_FAIL_CHECK(glGetProgramiv(_id, GL_LINK_STATUS, &result));
        GL_FAIL_CHECK(glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &infoLogLength));
        if (infoLogLength > 0) {
          std::vector<char> errorMessage(infoLogLength + 1);
          GL_FAIL_CHECK(glGetProgramInfoLog(_id, infoLogLength, NULL, &errorMessage[0]));
          core::Logger::error("Link error:", &errorMessage[0]);
        }
        _linked = true;
      }

      void bind() {
        if (!_linked) {
          if (!loaded()) {
            return;
          }
          if (!compiled()) {
            vertexShader->compile();
            fragmentShader->compile();
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
        GL_FAIL_CHECK(glDetachShader(_id, vertexShader->id()));
        GL_FAIL_CHECK(glDetachShader(_id, fragmentShader->id()));
      }

      inline unsigned int id() {
        return _id;
      }

      inline bool loaded() {
        return vertexShader->loaded() && fragmentShader->loaded();
      }

      inline bool linked() {
        return _linked;
      }

      inline bool compiled() {
        return vertexShader->compiled() && fragmentShader->compiled();
      }

    private:
      unsigned int _id;

      GlShaderSpec vertexShaderSpec;

      GlShaderSpec fragmentShaderSpec;

      GlShader* vertexShader;

      GlShader* fragmentShader;

      bool _bound;

      bool _linked;
    };

  }
}
