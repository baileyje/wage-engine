#ifndef ENTITY_TEXTURE_H
#define ENTITY_TEXTURE_H

#include <string>

namespace wage {

  // TODO:  De-jank this
  class Texture {

  public:

    Texture(std::string path) : path(path) {
    }

    ~Texture() {}

    inline std::string getPath() {
      return path;
    }

    inline std::string getId() {
      return getPath();
    }

  private:

    std::string path;

  };

}

#endif //ENTITY_TEXTURE_H
