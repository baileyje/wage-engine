#ifndef ENTITY_TEXTURE_H
#define ENTITY_TEXTURE_H

#include <string>

namespace wage {

  // TODO:  De-jank this
  class Texture {

  public:

    static Texture* Default;

    Texture(std::string path) : path_(path) {
    }

    ~Texture() {}

    inline std::string path() {
      return path_;
    }

    inline std::string id() {
      return path();
    }

  private:

    std::string path_;

  };

}

#endif //ENTITY_TEXTURE_H
