#pragma once

#include <string>

namespace wage { namespace render {

  // TODO:  De-jank this
  class Texture {

  public:
    static Texture Default;

    Texture() : _path("****INVALID****") {
    }

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

} }