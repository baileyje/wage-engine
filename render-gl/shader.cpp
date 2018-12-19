#include "render-gl/shader.h"

#include "core/logger.h"
#include <vector>

namespace wage {

  void GlShader::compile() {
    GL_FAIL_CHECK(id_ = glCreateShader(shaderType));
    const char* temp = reinterpret_cast<const char*>(buffer()->data());
    const GLint tempLength = static_cast<const GLint>(buffer()->length());
    GL_FAIL_CHECK(glShaderSource(id_, 1, &temp, &tempLength));
    GL_FAIL_CHECK(glCompileShader(id_));

    GLint result = GL_FALSE;
    int infoLogLength;
    GL_FAIL_CHECK(glGetShaderiv(id_, GL_COMPILE_STATUS, &result));
    GL_FAIL_CHECK(glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &infoLogLength));
    if ( infoLogLength > 0 ){
      std::vector<char> errrorMessage(infoLogLength+1);
      GL_FAIL_CHECK(glGetShaderInfoLog(id_, infoLogLength, NULL, &errrorMessage[0]));
      Logger::error("%s\n", &errrorMessage[0]);
    }
  }

  void GlShader::onLoad(Buffer* buffer) {
    // Don't like this....
    // GL_FAIL_CHECK(id_ = glCreateShader(shaderType));
    // compile(buffer);
  }

}
