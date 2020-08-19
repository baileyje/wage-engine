#include "render-gl/shader.h"

#include <vector>

#include "core/logger.h"

namespace wage {
  namespace render {

    void GlShader::compile() {
      GL_FAIL_CHECK(_id = glCreateShader(shaderType));
      GL_FAIL_CHECK(glShaderSource(_id, 1, &shaderText, &shaderLength));
      GL_FAIL_CHECK(glCompileShader(_id));

      GLint result = GL_FALSE;
      int infoLogLength;
      GL_FAIL_CHECK(glGetShaderiv(_id, GL_COMPILE_STATUS, &result));
      GL_FAIL_CHECK(glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &infoLogLength));
      if (infoLogLength > 0) {
        std::vector<char> errrorMessage(infoLogLength + 1);
        GL_FAIL_CHECK(glGetShaderInfoLog(_id, infoLogLength, NULL, &errrorMessage[0]));
        core::Logger::error("Compile Error: ", &errrorMessage[0]);
      }
      _compiled = true;
    }
  }
}
