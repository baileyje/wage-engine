#ifndef RENDERER_SHADER_H
#define RENDERER_SHADER_H

#include <string>
#include "fs/file_system.h"

namespace wage {

  class Shader {

  public:

    static Shader* Default;

    static void initDefault(FileSystem* fileSystem);

    Shader(File* vertexSource, File* fragmentSource);

    ~Shader();

    void bind();

    void unbind();

    void compile();

    void link();

    void unlink();

    inline unsigned int getId() {
      return id;
    }

    inline bool isBound() {
      return bound;
    }

  private:

    unsigned int vertexId;

    unsigned int fragmentId;

    unsigned int id;

    File* vertexSource;

    File* fragmentSource;

    bool bound;
  };

}

#endif //RENDERER_SHADER_H