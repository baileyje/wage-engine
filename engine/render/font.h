#pragma once

#include <string>

namespace wage { namespace render {

  class Font {

  public:
    Font(std::string path, int size) : _path(path), _size(size) {}

    Font() {
    }

    inline std::string path() {
      return _path;
    }

    inline int size() {
      return _size;
    }

  private:
    std::string _path;

    int _size;
  };

} }