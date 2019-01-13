#ifndef ENTITY_TEXTURE_H
#define ENTITY_TEXTURE_H

#include <string>

namespace wage {

  // TODO:  De-jank this
  class Texture {

  public:

    static Texture* Default;

    Texture(std::string path) : _path(path) {
    }

    ~Texture() {}

    inline std::string path() {
      return _path;
    }

    inline std::string id() {
      return path();
    }

  private:

    std::string _path;

  };

}

#endif //ENTITY_TEXTURE_H
