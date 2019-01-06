#ifndef COMPONENT_UI_FONT_H
#define COMPONENT_UI_FONT_H

#include <string>

namespace wage {

  class Font {
  
  public:

    Font(std::string path, int size) : path_(path), size_(size) {}

    Font() {      
    }

    inline std::string path() {
      return path_;
    }
    
    inline int size() {
      return size_;
    }

  private:

    std::string path_;
    
    int size_;

  };

}

#endif //COMPONENT_UI_FONT_H