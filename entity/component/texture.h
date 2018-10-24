#ifndef ENTITY_TEXTURE_H
#define ENTITY_TEXTURE_H

#include <string>

// TODO:  De-jank this
class Texture {

public:

  Texture(std::string path) : path(path) {
  }

  ~Texture() {}

  inline std::string getPath() {
    return path;
  }

private:

  std::string path;

};

#endif //ENTITY_TEXTURE_H
