#ifndef RENDERER_SHADER_H
#define RENDERER_SHADER_H

#include <string>

class Shader {

public:

  Shader(std::string vertexSource, std::string fragmentSource);

  ~Shader();

  void bind() const;

  void unbind() const;

  void compile();

  void link();

  void unlink();

private:

  unsigned int vertexId;

  unsigned int fragmentId;

  unsigned int id;

  std::string vertexSource;

  std::string fragmentSource;
};

#endif //RENDERER_SHADER_H