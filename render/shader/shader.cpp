#include "render/shader/shader.h"

#include <glad/glad.h>
#include <vector>

#include "render/util.h"

void compileGLShader(unsigned int shaderId, const char* source) {
	FAIL_CHECK(glShaderSource(shaderId, 1, &source , NULL));
	FAIL_CHECK(glCompileShader(shaderId));

  GLint result = GL_FALSE;
	int infoLogLength;
  FAIL_CHECK(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result));
	FAIL_CHECK(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength));
	if ( infoLogLength > 0 ){
		std::vector<char> errrorMessage(infoLogLength+1);
		FAIL_CHECK(glGetShaderInfoLog(shaderId, infoLogLength, NULL, &errrorMessage[0]));
		printf("%s\n", &errrorMessage[0]);
	}
}

Shader* Shader::Default;

void Shader::initDefault() {
  Default = new Shader(
    std::string(
      "#version 330 core\n"
      "layout(location = 0) in vec3 vertPos;\n"
      "layout(location = 1) in vec3 normalIn;\n"
      "out vec3 fragPos;\n"
      "out vec3 normal;\n"
      "uniform mat4 model;\n"
      "uniform mat4 view;\n"
      "uniform mat4 projection;\n"
      "void main() {\n"
        "fragPos = vec3(model * vec4(vertPos, 1.0));\n"
        "normal = mat3(transpose(inverse(model))) * normalIn;\n"
        "gl_Position = projection * view * vec4(fragPos, 1.0);\n"
      "}"
    ),
    std::string(
      "#version 330 core\n"
      "out vec4 fragColor;\n"
      "in vec3 normal;\n"
      "in vec3 fragPos;\n"
      "uniform vec3 lightPos;\n"
      "uniform vec3 viewPos;\n"
      "uniform vec3 lightColor;\n"
      "uniform vec3 objectColor;\n"
      "void main() {\n"
          "float ambientStrength = 0.1;\n"
          "vec3 ambient = ambientStrength * lightColor;\n"
          "vec3 norm = normalize(normal);\n"
          "vec3 lightDir = normalize(lightPos - fragPos);\n"
          "float diff = max(dot(norm, lightDir), 0.0);\n"
          "vec3 diffuse = diff * lightColor;\n"
          "float specularStrength = 0.5;\n"
          "vec3 viewDir = normalize(viewPos - fragPos);\n"
          "vec3 reflectDir = reflect(-lightDir, norm);\n"
          "float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
          "vec3 specular = specularStrength * spec * lightColor;\n"              
          "vec3 result = (ambient + diffuse + specular) * objectColor;\n"
          "fragColor = vec4(result, 1.0);\n"
      "}" 
    )
  );
  Default->compile();
  Default->link();
}


Shader::Shader(std::string vertexSource, std::string fragmentSource) 
  : vertexSource(vertexSource), fragmentSource(fragmentSource) {  
  FAIL_CHECK(vertexId = glCreateShader(GL_VERTEX_SHADER));
	FAIL_CHECK(fragmentId = glCreateShader(GL_FRAGMENT_SHADER));
  id = glCreateProgram();
}

Shader::~Shader() {
  FAIL_CHECK(glDeleteShader(vertexId));
	FAIL_CHECK(glDeleteShader(fragmentId));
  FAIL_CHECK(glDeleteProgram(id));
}

void Shader::bind() const {
  FAIL_CHECK(glUseProgram(id));
}

void Shader::unbind() const {
  FAIL_CHECK(glUseProgram(0));
}

void Shader::compile() {
  FAIL_CHECK(compileGLShader(vertexId, vertexSource.c_str()));
  FAIL_CHECK(compileGLShader(fragmentId, fragmentSource.c_str()));
}

void Shader::link() {
  GLint result = GL_FALSE;
  int infoLogLength;

  FAIL_CHECK(glAttachShader(id, vertexId));
	FAIL_CHECK(glAttachShader(id, fragmentId));
	FAIL_CHECK(glLinkProgram(id));

  FAIL_CHECK(glGetProgramiv(id, GL_LINK_STATUS, &result));
	FAIL_CHECK(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength));
	if ( infoLogLength > 0 ){
		std::vector<char> errorMessage(infoLogLength+1);
		FAIL_CHECK(glGetProgramInfoLog(id, infoLogLength, NULL, &errorMessage[0]));
		printf("%s\n", &errorMessage[0]);
	}
}

void Shader::unlink() {
  FAIL_CHECK(glDetachShader(id, vertexId));
	FAIL_CHECK(glDetachShader(id, fragmentId));
}
