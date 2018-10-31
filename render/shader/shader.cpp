#include "render/shader/shader.h"

#include "core/logger.h"
#include <glad/glad.h>
#include <vector>

#include "render/util.h"

void compileGLShader(unsigned int shaderId, const char* source) {
	GL_FAIL_CHECK(glShaderSource(shaderId, 1, &source , NULL));
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

Shader* Shader::Default;

void Shader::initDefault(FileSystem* fileSystem) {
  std::string* vs = fileSystem->read("resources/shaders/default.vs");
  std::string* fs = fileSystem->read("resources/shaders/default.fs");
  Default = new Shader(*vs, *fs);    
  Default->compile();
  Default->link();
  delete vs;
  delete fs;
}


Shader::Shader(std::string vertexSource, std::string fragmentSource) 
  : vertexSource(vertexSource), fragmentSource(fragmentSource) {  
  GL_FAIL_CHECK(vertexId = glCreateShader(GL_VERTEX_SHADER));
	GL_FAIL_CHECK(fragmentId = glCreateShader(GL_FRAGMENT_SHADER));
  id = glCreateProgram();
}

Shader::~Shader() {
  GL_FAIL_CHECK(glDeleteShader(vertexId));
	GL_FAIL_CHECK(glDeleteShader(fragmentId));
  GL_FAIL_CHECK(glDeleteProgram(id));
}

void Shader::bind() const {
  GL_FAIL_CHECK(glUseProgram(id));
}

void Shader::unbind() const {
  GL_FAIL_CHECK(glUseProgram(0));
}

void Shader::compile() {
  GL_FAIL_CHECK(compileGLShader(vertexId, vertexSource.c_str()));
  GL_FAIL_CHECK(compileGLShader(fragmentId, fragmentSource.c_str()));
}

void Shader::link() {
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

void Shader::unlink() {
  GL_FAIL_CHECK(glDetachShader(id, vertexId));
	GL_FAIL_CHECK(glDetachShader(id, fragmentId));
}
