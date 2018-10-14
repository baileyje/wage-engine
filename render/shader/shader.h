#ifndef RENDERER_SHADER_H
#define RENDERER_SHADER_H

#include <string>

class Shader {

public:

  static Shader* Default;

  static void initDefault();

  Shader(std::string vertexSource, std::string fragmentSource);

  ~Shader();

  void bind() const;

  void unbind() const;

  void compile();

  void link();

  void unlink();

  inline unsigned int getId() {
    return id;
  }

private:

  unsigned int vertexId;

  unsigned int fragmentId;

  unsigned int id;

  std::string vertexSource;

  std::string fragmentSource;
};

#endif //RENDERER_SHADER_H