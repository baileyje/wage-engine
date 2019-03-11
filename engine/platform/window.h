#ifndef PLATFORM_WINDOW_H
#define PLATFORM_WINDOW_H

namespace wage {

  class Window {

  public:
    Window(void* wrapped, int width, int height) : wrapped(wrapped), _width(width), _height(height) {
    }

    virtual ~Window() {}

    template <typename T>
    T* as() {
      return reinterpret_cast<T*>(wrapped);
    }

    inline int width() {
      return _width;
    }

    inline int height() {
      return _height;
    }

  private:
    void* wrapped;

    int _width;

    int _height;
  };
}

#endif //PLATFORM_WINDOW_H
