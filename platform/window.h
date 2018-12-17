#ifndef PLATFORM_WINDOW_H
#define PLATFORM_WINDOW_H

namespace wage {

  class Window {

  public:

    Window(void* wrapped, int width, int height) : wrapped(wrapped), width_(width), height_(height) {      
    }

    virtual ~Window() {}

    template <typename T>
    T* as() {
      return reinterpret_cast<T*>(wrapped);
    }

    inline int width() {
      return width_;
    }

    inline int height() {
      return height_;
    }

  private:
  
    void* wrapped;

    int width_;

    int height_;

  };

}

#endif //PLATFORM_WINDOW_H
