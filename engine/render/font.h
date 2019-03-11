#ifndef COMPONENT_UI_FONT_H
#define COMPONENT_UI_FONT_H

#include <string>

namespace wage {

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
}

#endif //COMPONENT_UI_FONT_H