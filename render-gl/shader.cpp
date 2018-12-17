#include "render-gl/shader.h"

#include "core/logger.h"
#include <glad/glad.h>
#include <vector>

#include "render-gl/util.h"

namespace wage {

  void compileGLShader(unsigned int shaderId, File* source) {
    const char* temp = reinterpret_cast<const char*>(source->data());
    const GLint tempLength = static_cast<const GLint>(source->length());
    GL_FAIL_CHECK(glShaderSource(shaderId, 1, &temp, &tempLength));
    GL_FAIL_CHECK(glCompileShader(shaderId));

    GLint result = GL_FALSE;
    int infoLogLength;
    GL_FAIL_CHECK(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result));
    GL_FAIL_CHECK(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength));
    if ( infoLogLength > 0 ){
      std::vector<char> errrorMessage(infoLogLength+1);
      GL_FAIL_CHECK(glGetShaderInfoLog(shaderId, infoLogLength, NULL, &errrorMessage[0]));
      Logger::error("%s\n", &errrorMessage[0]);
    }
  }

  GlShader* GlShader::Default;

  void GlShader::initDefault(FileSystem* fileSystem) {
    File* vs = fileSystem->read("resources/shaders/default.vs");
    File* fs = fileSystem->read("resources/shaders/default.fs");
    Default = new GlShader(vs, fs); 
    Default->compile();
    Default->link();
    delete vs;
    delete fs;
  }


  GlShader::GlShader(File* vertexSource, File* fragmentSource) 
    : vertexSource(vertexSource), fragmentSource(fragmentSource) {  
    GL_FAIL_CHECK(vertexId = glCreateShader(GL_VERTEX_SHADER));
    GL_FAIL_CHECK(fragmentId = glCreateShader(GL_FRAGMENT_SHADER));
    id = glCreateProgram();
  }

  GlShader::~GlShader() {
    GL_FAIL_CHECK(glDeleteShader(vertexId));
    GL_FAIL_CHECK(glDeleteShader(fragmentId));
    GL_FAIL_CHECK(glDeleteProgram(id));
  }

  void GlShader::bind() {
    GL_FAIL_CHECK(glUseProgram(id));
    bound = true;
  }

  void GlShader::unbind() {
    GL_FAIL_CHECK(glUseProgram(0));
    bound = false;
  }

  void GlShader::compile() {
    GL_FAIL_CHECK(compileGLShader(vertexId, vertexSource));
    GL_FAIL_CHECK(compileGLShader(fragmentId, fragmentSource));
  }

  void GlShader::link() {
    GLint result = GL_FALSE;
    int infoLogLength;

    GL_FAIL_CHECK(glAttachShader(id, vertexId));
    GL_FAIL_CHECK(glAttachShader(id, fragmentId));
    GL_FAIL_CHECK(glLinkProgram(id));

    GL_FAIL_CHECK(glGetProgramiv(id, GL_LINK_STATUS, &result));
    GL_FAIL_CHECK(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength));
    if ( infoLogLength > 0 ){
      std::vector<char> errorMessage(infoLogLength+1);
      GL_FAIL_CHECK(glGetProgramInfoLog(id, infoLogLength, NULL, &errorMessage[0]));
      Logger::error("%s\n", &errorMessage[0]);
    }
  }

  void GlShader::unlink() {
    GL_FAIL_CHECK(glDetachShader(id, vertexId));
    GL_FAIL_CHECK(glDetachShader(id, fragmentId));
  }

}
